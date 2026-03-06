// Copyright (c) 2024 Boos501. All Rights Reserved.

#include "InstancePainterComponent.h"
#include "Engine/StaticMesh.h"

UE_INLINE_GENERATED_CPP_BY_NAME(InstancePainterComponent)

#define LOCTEXT_NAMESPACE "InstancePainter"

UInstancePainterComponent::UInstancePainterComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInstancePainterComponent::BeginPlay()
{
    Super::BeginPlay();
}

UHierarchicalInstancedStaticMeshComponent* UInstancePainterComponent::GetOrCreateHISM(int32 EntryIndex)
{
    if (UHierarchicalInstancedStaticMeshComponent** Found = HISMComponents.Find(EntryIndex))
    {
        return *Found;
    }

    if (!PaintEntries.IsValidIndex(EntryIndex))
    {
        return nullptr;
    }

    const FInstancePaintEntry& Entry = PaintEntries[EntryIndex];
    UStaticMesh* LoadedMesh = Entry.Mesh.LoadSynchronous();
    if (!LoadedMesh)
    {
        return nullptr;
    }

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return nullptr;
    }

    UHierarchicalInstancedStaticMeshComponent* HISM =
        NewObject<UHierarchicalInstancedStaticMeshComponent>(Owner,
            *FString::Printf(TEXT("HISM_%d"), EntryIndex));

    HISM->SetStaticMesh(LoadedMesh);

    // Apply override materials
    for (int32 MatIdx = 0; MatIdx < Entry.OverrideMaterials.Num(); ++MatIdx)
    {
        if (UMaterialInterface* Mat = Entry.OverrideMaterials[MatIdx].LoadSynchronous())
        {
            HISM->SetMaterial(MatIdx, Mat);
        }
    }

    HISM->RegisterComponent();
    HISM->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

    HISMComponents.Add(EntryIndex, HISM);
    return HISM;
}

void UInstancePainterComponent::AddInstance(int32 EntryIndex, const FTransform& WorldTransform)
{
    UHierarchicalInstancedStaticMeshComponent* HISM = GetOrCreateHISM(EntryIndex);
    if (HISM)
    {
        HISM->AddInstance(WorldTransform, /*bWorldSpace=*/true);
    }
}

void UInstancePainterComponent::RemoveInstancesInSphere(const FVector& Center, float Radius)
{
    for (auto& Pair : HISMComponents)
    {
        UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value;
        if (!HISM)
        {
            continue;
        }

        TArray<int32> Overlapping = HISM->GetInstancesOverlappingSphere(Center, Radius, /*bSphereInWorldSpace=*/true);
        // Remove in reverse order to keep indices valid
        Overlapping.Sort([](int32 A, int32 B) { return A > B; });
        for (int32 Idx : Overlapping)
        {
            HISM->RemoveInstance(Idx);
        }
    }
}

void UInstancePainterComponent::ClearAllInstances()
{
    for (auto& Pair : HISMComponents)
    {
        if (Pair.Value)
        {
            Pair.Value->ClearInstances();
        }
    }
}

#undef LOCTEXT_NAMESPACE
