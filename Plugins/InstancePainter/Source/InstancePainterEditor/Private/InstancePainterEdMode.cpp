// Copyright (c) 2024 Boos501. All Rights Reserved.

#include "InstancePainterEdMode.h"
#include "InstancePainterComponent.h"
#include "SInstancePainterPanel.h"

#include "EditorModeManager.h"
#include "EditorViewportClient.h"
#include "LandscapeProxy.h"
#include "Components/BrushComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "SceneView.h"
#include "Toolkits/ToolkitManager.h"
#include "Engine/Texture2D.h"
#include "Math/UnrealMathUtility.h"

#define LOCTEXT_NAMESPACE "InstancePainterEdMode"

// ---------------------------------------------------------------------------
// Simple toolkit wrapper to host the Slate panel
// ---------------------------------------------------------------------------

class FInstancePainterEdModeToolkit : public FModeToolkit
{
public:
    explicit FInstancePainterEdModeToolkit(FInstancePainterEdMode* InMode)
        : EdMode(InMode)
    {}

    virtual FName GetToolkitFName() const override { return FName("InstancePainterToolkit"); }
    virtual FText GetBaseToolkitName() const override
    {
        return LOCTEXT("ToolkitName", "Instance Painter");
    }
    virtual FEdMode* GetEditorMode() const override { return EdMode; }
    virtual TSharedPtr<SWidget> GetInlineContent() const override { return PanelWidget; }

    virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode) override
    {
        PanelWidget = SNew(SInstancePainterPanel, EdMode);
        FModeToolkit::Init(InitToolkitHost, InOwningMode);
    }

private:
    FInstancePainterEdMode* EdMode = nullptr;
    TSharedPtr<SInstancePainterPanel> PanelWidget;
};

// ---------------------------------------------------------------------------
// FInstancePainterEdMode
// ---------------------------------------------------------------------------

FInstancePainterEdMode::FInstancePainterEdMode()
{
    BrushLocation = FVector::ZeroVector;
    BrushNormal = FVector::UpVector;
}

FInstancePainterEdMode::~FInstancePainterEdMode()
{}

void FInstancePainterEdMode::Enter()
{
    FEdMode::Enter();

    if (!Toolkit.IsValid())
    {
        Toolkit = MakeShareable(new FInstancePainterEdModeToolkit(this));
        Toolkit->Init(Owner->GetToolkitHost(), ModeInfo.EditorModeOwner.IsValid() ? ModeInfo.EditorModeOwner : nullptr);
    }
}

void FInstancePainterEdMode::Exit()
{
    if (Toolkit.IsValid())
    {
        FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
        Toolkit.Reset();
    }

    FEdMode::Exit();
}

// ---------------------------------------------------------------------------
// Render – draw the brush circle
// ---------------------------------------------------------------------------

void FInstancePainterEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
    FEdMode::Render(View, Viewport, PDI);

    if (!bBrushVisible)
    {
        return;
    }

    const float Radius = BrushSettings.BrushRadius;
    const float Falloff = BrushSettings.BrushFalloff;
    const float FalloffRadius = Radius * (1.0f - Falloff);
    const int32 NumSegments = 64;

    // Choose color based on active tool
    FLinearColor OuterColor = (ActiveTool == EInstancePainterTool::Erase)
        ? FLinearColor(1.0f, 0.0f, 0.0f, 1.0f)
        : FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);
    FLinearColor InnerColor = OuterColor * 0.5f;
    InnerColor.A = 1.0f;

    // Build a local coordinate frame from the brush normal
    FVector AxisX, AxisY;
    BrushNormal.FindBestAxisVectors(AxisX, AxisY);

    // Outer circle
    FVector PrevOuter = BrushLocation + AxisX * Radius;
    FVector PrevInner = BrushLocation + AxisX * FalloffRadius;
    for (int32 i = 1; i <= NumSegments; ++i)
    {
        const float Angle = (float)i / (float)NumSegments * 2.0f * PI;
        const FVector Dir = AxisX * FMath::Cos(Angle) + AxisY * FMath::Sin(Angle);
        const FVector CurOuter = BrushLocation + Dir * Radius;
        const FVector CurInner = BrushLocation + Dir * FalloffRadius;

        PDI->DrawLine(PrevOuter, CurOuter, OuterColor, SDPG_Foreground);
        PDI->DrawLine(PrevInner, CurInner, InnerColor, SDPG_Foreground);

        PrevOuter = CurOuter;
        PrevInner = CurInner;
    }
}

// ---------------------------------------------------------------------------
// Input handling
// ---------------------------------------------------------------------------

bool FInstancePainterEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                                       FKey Key, EInputEvent Event)
{
    if (Event == IE_Pressed || Event == IE_Repeat)
    {
        // [ shrinks radius, ] grows radius
        if (Key == EKeys::LeftBracket)
        {
            BrushSettings.BrushRadius = FMath::Clamp(BrushSettings.BrushRadius - 50.0f, 10.0f, 10000.0f);
            return true;
        }
        if (Key == EKeys::RightBracket)
        {
            BrushSettings.BrushRadius = FMath::Clamp(BrushSettings.BrushRadius + 50.0f, 10.0f, 10000.0f);
            return true;
        }
    }
    return FEdMode::InputKey(ViewportClient, Viewport, Key, Event);
}

bool FInstancePainterEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                                        int32 X, int32 Y)
{
    LastMouseX = X;
    LastMouseY = Y;
    UpdateBrushLocation(ViewportClient);

    if (bIsPainting)
    {
        if (ActiveTool == EInstancePainterTool::Paint)
        {
            DoPaint(ViewportClient);
        }
        else if (ActiveTool == EInstancePainterTool::Erase)
        {
            DoErase();
        }
    }

    return FEdMode::MouseMove(ViewportClient, Viewport, X, Y);
}

bool FInstancePainterEdMode::StartTracking(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
    if (ViewportClient->IsAltPressed())
    {
        return false;
    }

    bIsPainting = true;

    // Generate random mask rotation at start of stroke
    if (BrushSettings.bRandomizeMaskRotation)
    {
        ActiveMaskRotation = FMath::FRand() * 360.0f;
    }
    else
    {
        ActiveMaskRotation = BrushSettings.MaskRotation;
    }

    if (ActiveTool == EInstancePainterTool::Paint)
    {
        DoPaint(ViewportClient);
    }
    else if (ActiveTool == EInstancePainterTool::Erase)
    {
        DoErase();
    }
    else if (ActiveTool == EInstancePainterTool::SinglePlace)
    {
        DoSinglePlace(ViewportClient);
    }

    return true;
}

bool FInstancePainterEdMode::EndTracking(FEditorViewportClient* ViewportClient, FViewport* Viewport)
{
    bIsPainting = false;
    return FEdMode::EndTracking(ViewportClient, Viewport);
}

bool FInstancePainterEdMode::CapturedMouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                                                int32 InMouseX, int32 InMouseY)
{
    LastMouseX = InMouseX;
    LastMouseY = InMouseY;
    UpdateBrushLocation(ViewportClient);

    if (bIsPainting)
    {
        if (ActiveTool == EInstancePainterTool::Paint)
        {
            DoPaint(ViewportClient);
        }
        else if (ActiveTool == EInstancePainterTool::Erase)
        {
            DoErase();
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Update brush location
// ---------------------------------------------------------------------------

void FInstancePainterEdMode::UpdateBrushLocation(FEditorViewportClient* ViewportClient)
{
    FVector HitLocation, HitNormal;
    AActor* HitActor = nullptr;
    if (TraceFromScreen(ViewportClient, HitLocation, HitNormal, HitActor))
    {
        BrushLocation = HitLocation;
        BrushNormal = HitNormal;
        bBrushVisible = true;
    }
    else
    {
        bBrushVisible = false;
    }
}

// ---------------------------------------------------------------------------
// Trace helper
// ---------------------------------------------------------------------------

bool FInstancePainterEdMode::TraceFromScreen(FEditorViewportClient* ViewportClient,
                                              FVector& OutHitLocation, FVector& OutHitNormal,
                                              AActor*& OutHitActor) const
{
    if (!ViewportClient || !ViewportClient->Viewport)
    {
        return false;
    }

    FSceneViewFamilyContext ViewFamily(
        FSceneViewFamily::ConstructionValues(
            ViewportClient->Viewport,
            ViewportClient->GetScene(),
            ViewportClient->EngineShowFlags)
        .SetRealtimeUpdate(ViewportClient->IsRealtime()));

    FSceneView* SceneView = ViewportClient->CalcSceneView(&ViewFamily);
    if (!SceneView)
    {
        return false;
    }

    FVector RayOrigin, RayDirection;
    SceneView->DeprojectFVector2D(FVector2D((float)LastMouseX, (float)LastMouseY), RayOrigin, RayDirection);

    UWorld* World = ViewportClient->GetWorld();
    if (!World)
    {
        return false;
    }

    FHitResult HitResult;
    const bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        RayOrigin,
        RayOrigin + RayDirection * 100000.0f,
        ECC_WorldStatic
    );

    if (bHit)
    {
        OutHitLocation = HitResult.ImpactPoint;
        OutHitNormal = HitResult.ImpactNormal;
        OutHitActor = HitResult.GetActor();
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Paint
// ---------------------------------------------------------------------------

void FInstancePainterEdMode::DoPaint(FEditorViewportClient* ViewportClient)
{
    UInstancePainterComponent* Comp = TargetComponent.Get();
    if (!Comp || !bBrushVisible)
    {
        return;
    }

    const float Radius = BrushSettings.BrushRadius;
    const float Density = BrushSettings.PaintDensity;

    // Number of attempts proportional to brush area and density
    const int32 NumAttempts = FMath::Max(1, (int32)(PI * Radius * Radius * Density * 0.0001f));

    // Build a local coordinate frame from the brush normal
    FVector AxisX, AxisY;
    BrushNormal.FindBestAxisVectors(AxisX, AxisY);

    for (int32 i = 0; i < NumAttempts; ++i)
    {
        // Uniform distribution inside circle: r = sqrt(rand) * Radius
        const float R = FMath::Sqrt(FMath::FRand()) * Radius;
        const float Theta = FMath::FRand() * 2.0f * PI;

        const FVector2D LocalOffset(R * FMath::Cos(Theta), R * FMath::Sin(Theta));
        const FVector WorldOffset = AxisX * LocalOffset.X + AxisY * LocalOffset.Y;

        // Falloff weight check
        const float FalloffWeight = ComputeFalloffWeight(R);
        if (FMath::FRand() > FalloffWeight)
        {
            continue;
        }

        // Mask sample
        const FVector2D BrushUV = FVector2D((LocalOffset.X / Radius + 1.0f) * 0.5f,
                                             (LocalOffset.Y / Radius + 1.0f) * 0.5f);
        const float MaskValue = SampleMaskTexture(BrushUV);
        if (FMath::FRand() > MaskValue)
        {
            continue;
        }

        // Trace from above sample position
        UWorld* World = ViewportClient->GetWorld();
        if (!World)
        {
            break;
        }

        const FVector SamplePos = BrushLocation + WorldOffset;
        const FVector TraceStart = SamplePos + BrushNormal * 500.0f;
        const FVector TraceEnd   = SamplePos - BrushNormal * 500.0f;

        FHitResult HitResult;
        if (!World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic))
        {
            continue;
        }

        // Surface validity (type + slope)
        if (!IsSurfaceValid(HitResult.GetActor(), HitResult.ImpactNormal))
        {
            continue;
        }

        // Select an entry by weight
        float TotalWeight = 0.0f;
        for (const FInstancePaintEntry& E : Comp->PaintEntries)
        {
            TotalWeight += FMath::Max(0.0f, E.Weight);
        }
        if (TotalWeight <= 0.0f)
        {
            continue;
        }

        float Rnd = FMath::FRand() * TotalWeight;
        int32 SelectedEntry = 0;
        for (int32 EIdx = 0; EIdx < Comp->PaintEntries.Num(); ++EIdx)
        {
            Rnd -= FMath::Max(0.0f, Comp->PaintEntries[EIdx].Weight);
            if (Rnd <= 0.0f)
            {
                SelectedEntry = EIdx;
                break;
            }
        }

        const FInstancePaintEntry& Entry = Comp->PaintEntries[SelectedEntry];

        // Minimum distance check using HISM
        UHierarchicalInstancedStaticMeshComponent* HISM = Comp->GetOrCreateHISM(SelectedEntry);
        if (HISM && Entry.MinDistance > 0.0f)
        {
            TArray<int32> Nearby = HISM->GetInstancesOverlappingSphere(
                HitResult.ImpactPoint, Entry.MinDistance, /*bSphereInWorldSpace=*/true);
            if (Nearby.Num() > 0)
            {
                continue;
            }
        }

        // Compute transform and add instance
        const FTransform InstanceTransform = ComputeRandomTransform(Entry, HitResult.ImpactPoint,
                                                                     HitResult.ImpactNormal);
        Comp->AddInstance(SelectedEntry, InstanceTransform);
    }
}

void FInstancePainterEdMode::DoErase()
{
    UInstancePainterComponent* Comp = TargetComponent.Get();
    if (!Comp || !bBrushVisible)
    {
        return;
    }

    const float EraseRadius = BrushSettings.BrushRadius * BrushSettings.EraseDensity * 10.0f;
    Comp->RemoveInstancesInSphere(BrushLocation, FMath::Max(1.0f, EraseRadius));
}

void FInstancePainterEdMode::DoSinglePlace(FEditorViewportClient* ViewportClient)
{
    UInstancePainterComponent* Comp = TargetComponent.Get();
    if (!Comp || !bBrushVisible)
    {
        return;
    }

    if (Comp->PaintEntries.Num() == 0)
    {
        return;
    }

    // Place a single instance at the brush location using the first entry
    const int32 EntryIndex = 0;
    const FInstancePaintEntry& Entry = Comp->PaintEntries[EntryIndex];
    const FTransform InstanceTransform = ComputeRandomTransform(Entry, BrushLocation, BrushNormal);
    Comp->AddInstance(EntryIndex, InstanceTransform);
}

// ---------------------------------------------------------------------------
// ComputeRandomTransform
// ---------------------------------------------------------------------------

FTransform FInstancePainterEdMode::ComputeRandomTransform(const FInstancePaintEntry& Entry,
                                                            const FVector& HitLocation,
                                                            const FVector& HitNormal) const
{
    // Scale
    FVector Scale;
    if (Entry.bUniformScale)
    {
        const float S = FMath::FRandRange(Entry.ScaleMin, Entry.ScaleMax);
        Scale = FVector(S);
    }
    else
    {
        Scale = FVector(
            FMath::FRandRange(Entry.ScaleMin3D.X, Entry.ScaleMax3D.X),
            FMath::FRandRange(Entry.ScaleMin3D.Y, Entry.ScaleMax3D.Y),
            FMath::FRandRange(Entry.ScaleMin3D.Z, Entry.ScaleMax3D.Z)
        );
    }

    // Base rotation (Yaw/Pitch/Roll ranges in degrees)
    const float Yaw   = FMath::FRandRange(Entry.YawRange.X,   Entry.YawRange.Y);
    const float Pitch = FMath::FRandRange(Entry.PitchRange.X, Entry.PitchRange.Y);
    const float Roll  = FMath::FRandRange(Entry.RollRange.X,  Entry.RollRange.Y);
    FQuat BaseRotation = FQuat(FRotator(Pitch, Yaw, Roll));

    // Normal alignment
    FQuat FinalRotation = BaseRotation;
    if (Entry.bAlignToNormal && !HitNormal.IsNearlyZero())
    {
        const FVector UpVector = FVector::UpVector;
        const FQuat AlignQuat = FQuat::FindBetweenNormals(UpVector, HitNormal);
        const FQuat BlendedAlign = FQuat::Slerp(FQuat::Identity, AlignQuat, Entry.NormalAlignmentStrength);
        FinalRotation = BlendedAlign * BaseRotation;
    }

    // Height offset along normal
    const float HeightOffset = FMath::FRandRange(Entry.HeightOffsetRange.X, Entry.HeightOffsetRange.Y);
    const FVector FinalLocation = HitLocation + HitNormal * HeightOffset;

    return FTransform(FinalRotation, FinalLocation, Scale);
}

// ---------------------------------------------------------------------------
// Falloff
// ---------------------------------------------------------------------------

float FInstancePainterEdMode::ComputeFalloffWeight(float Distance) const
{
    const float Radius = BrushSettings.BrushRadius;
    const float FalloffStart = Radius * (1.0f - BrushSettings.BrushFalloff);

    if (Distance <= FalloffStart)
    {
        return 1.0f;
    }
    if (Distance >= Radius)
    {
        return 0.0f;
    }
    return 1.0f - (Distance - FalloffStart) / (Radius - FalloffStart);
}

// ---------------------------------------------------------------------------
// Surface validity check
// ---------------------------------------------------------------------------

bool FInstancePainterEdMode::IsSurfaceValid(AActor* HitActor, const FVector& Normal) const
{
    if (!HitActor)
    {
        return false;
    }

    // Slope check
    const float SlopeAngleDeg = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Normal, FVector::UpVector)));
    if (SlopeAngleDeg > BrushSettings.MaxSlopeAngle)
    {
        return false;
    }

    // Actor type filter
    const bool bIsLandscape = HitActor->IsA<ALandscapeProxy>();
    const bool bIsStaticMesh = HitActor->IsA<AStaticMeshActor>();
    // BSP: actors with a UBrushComponent
    const bool bIsBSP = HitActor->FindComponentByClass<UBrushComponent>() != nullptr;

    if (bIsLandscape && !BrushSettings.bFilterLandscape) return false;
    if (bIsStaticMesh && !BrushSettings.bFilterStaticMesh) return false;
    if (bIsBSP && !BrushSettings.bFilterBSP) return false;

    return true;
}

// ---------------------------------------------------------------------------
// Mask sampling
// ---------------------------------------------------------------------------

float FInstancePainterEdMode::SampleMaskTexture(const FVector2D& BrushUV) const
{
    UTexture2D* MaskTex = BrushSettings.BrushMask.LoadSynchronous();
    if (!MaskTex)
    {
        // No mask – always pass
        return 1.0f;
    }

    // Rotate UV around center (0.5, 0.5)
    const float AngleRad = FMath::DegreesToRadians(ActiveMaskRotation);
    const float CosA = FMath::Cos(AngleRad);
    const float SinA = FMath::Sin(AngleRad);
    const FVector2D Centered = BrushUV - FVector2D(0.5f, 0.5f);
    const FVector2D Rotated(
        Centered.X * CosA - Centered.Y * SinA,
        Centered.X * SinA + Centered.Y * CosA
    );
    FVector2D RotatedUV = Rotated + FVector2D(0.5f, 0.5f);

    // Clamp to [0,1]
    RotatedUV.X = FMath::Clamp(RotatedUV.X, 0.0f, 1.0f);
    RotatedUV.Y = FMath::Clamp(RotatedUV.Y, 0.0f, 1.0f);

    // Read mip 0 data
    FTexture2DMipMap* Mip = &MaskTex->GetPlatformData()->Mips[0];
    const int32 Width = Mip->SizeX;
    const int32 Height = Mip->SizeY;

    if (Width <= 0 || Height <= 0)
    {
        return 1.0f;
    }

    void* RawData = Mip->BulkData.Lock(LOCK_READ_ONLY);
    if (!RawData)
    {
        return 1.0f;
    }

    const int32 PixelX = FMath::Clamp((int32)(RotatedUV.X * Width), 0, Width - 1);
    const int32 PixelY = FMath::Clamp((int32)(RotatedUV.Y * Height), 0, Height - 1);
    const FColor* Pixels = static_cast<const FColor*>(RawData);
    const float RedValue = Pixels[PixelY * Width + PixelX].R / 255.0f;

    Mip->BulkData.Unlock();

    return RedValue;
}

#undef LOCTEXT_NAMESPACE
