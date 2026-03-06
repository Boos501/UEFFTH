// Copyright (c) 2024 Boos501. All Rights Reserved.

#include "SInstancePainterPanel.h"
#include "InstancePainterEdMode.h"
#include "InstancePainterComponent.h"

#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/SBoxPanel.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "SInstancePainterPanel"

void SInstancePainterPanel::Construct(const FArguments& InArgs, FInstancePainterEdMode* InEdMode)
{
    EdMode = InEdMode;

    ChildSlot
    [
        SNew(SScrollBox)
        + SScrollBox::Slot().Padding(4.0f)
        [
            SNew(SVerticalBox)

            // ---- Tool Selection ----
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ToolLabel", "Tool"))
                .Font(FEditorStyle::GetFontStyle("BoldFont"))
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(1.0f)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("PaintBtn", "Paint"))
                    .IsEnabled_Lambda([this]() { return EdMode != nullptr; })
                    .ButtonColorAndOpacity_Lambda([this]() -> FSlateColor {
                        return IsPaintActive()
                            ? FLinearColor(0.2f, 0.8f, 0.2f)
                            : FLinearColor(0.3f, 0.3f, 0.3f);
                    })
                    .OnClicked(this, &SInstancePainterPanel::OnPaintClicked)
                ]
                + SHorizontalBox::Slot().FillWidth(1.0f)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("EraseBtn", "Erase"))
                    .IsEnabled_Lambda([this]() { return EdMode != nullptr; })
                    .ButtonColorAndOpacity_Lambda([this]() -> FSlateColor {
                        return IsEraseActive()
                            ? FLinearColor(0.8f, 0.2f, 0.2f)
                            : FLinearColor(0.3f, 0.3f, 0.3f);
                    })
                    .OnClicked(this, &SInstancePainterPanel::OnEraseClicked)
                ]
                + SHorizontalBox::Slot().FillWidth(1.0f)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("SinglePlaceBtn", "Single Place"))
                    .IsEnabled_Lambda([this]() { return EdMode != nullptr; })
                    .ButtonColorAndOpacity_Lambda([this]() -> FSlateColor {
                        return IsSinglePlaceActive()
                            ? FLinearColor(0.2f, 0.4f, 0.8f)
                            : FLinearColor(0.3f, 0.3f, 0.3f);
                    })
                    .OnClicked(this, &SInstancePainterPanel::OnSinglePlaceClicked)
                ]
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [ SNew(SSeparator) ]

            // ---- Brush Settings ----
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("BrushSettingsLabel", "Brush Settings"))
                .Font(FEditorStyle::GetFontStyle("BoldFont"))
            ]

            // Radius
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("RadiusLabel", "Radius"))
                ]
                + SHorizontalBox::Slot().FillWidth(0.6f)
                [
                    SNew(SNumericEntryBox<float>)
                    .Value(this, &SInstancePainterPanel::GetBrushRadius)
                    .MinValue(10.0f)
                    .MaxValue(10000.0f)
                    .MinSliderValue(10.0f)
                    .MaxSliderValue(10000.0f)
                    .AllowSpin(true)
                    .OnValueChanged(this, &SInstancePainterPanel::OnBrushRadiusChanged)
                    .OnValueCommitted(this, &SInstancePainterPanel::OnBrushRadiusCommitted)
                ]
            ]

            // Falloff
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("FalloffLabel", "Falloff"))
                ]
                + SHorizontalBox::Slot().FillWidth(0.6f)
                [
                    SNew(SNumericEntryBox<float>)
                    .Value(this, &SInstancePainterPanel::GetBrushFalloff)
                    .MinValue(0.0f)
                    .MaxValue(1.0f)
                    .MinSliderValue(0.0f)
                    .MaxSliderValue(1.0f)
                    .AllowSpin(true)
                    .OnValueChanged(this, &SInstancePainterPanel::OnBrushFalloffChanged)
                ]
            ]

            // Paint Density
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("PaintDensityLabel", "Paint Density"))
                ]
                + SHorizontalBox::Slot().FillWidth(0.6f)
                [
                    SNew(SNumericEntryBox<float>)
                    .Value(this, &SInstancePainterPanel::GetPaintDensity)
                    .MinValue(0.0f)
                    .MaxValue(1.0f)
                    .MinSliderValue(0.0f)
                    .MaxSliderValue(1.0f)
                    .AllowSpin(true)
                    .OnValueChanged(this, &SInstancePainterPanel::OnPaintDensityChanged)
                ]
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [ SNew(SSeparator) ]

            // ---- Mask Settings ----
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("MaskSettingsLabel", "Mask Settings"))
                .Font(FEditorStyle::GetFontStyle("BoldFont"))
            ]

            // Mask Rotation
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("MaskRotLabel", "Mask Rotation"))
                ]
                + SHorizontalBox::Slot().FillWidth(0.6f)
                [
                    SNew(SNumericEntryBox<float>)
                    .Value(this, &SInstancePainterPanel::GetMaskRotation)
                    .MinValue(0.0f)
                    .MaxValue(360.0f)
                    .MinSliderValue(0.0f)
                    .MaxSliderValue(360.0f)
                    .AllowSpin(true)
                    .OnValueChanged(this, &SInstancePainterPanel::OnMaskRotationChanged)
                ]
            ]

            // Randomize Mask Rotation
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [
                    SNew(STextBlock).Text(LOCTEXT("RandomMaskRotLabel", "Randomize Mask Rotation"))
                ]
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(SCheckBox)
                    .IsChecked(this, &SInstancePainterPanel::GetRandomizeMaskRotation)
                    .OnCheckStateChanged(this, &SInstancePainterPanel::OnRandomizeMaskRotationChanged)
                ]
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [ SNew(SSeparator) ]

            // ---- Surface Filters ----
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("FilterLabel", "Surface Filters"))
                .Font(FEditorStyle::GetFontStyle("BoldFont"))
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [ SNew(STextBlock).Text(LOCTEXT("FilterLandscapeLabel", "Landscape")) ]
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(SCheckBox)
                    .IsChecked(this, &SInstancePainterPanel::GetFilterLandscape)
                    .OnCheckStateChanged(this, &SInstancePainterPanel::OnFilterLandscapeChanged)
                ]
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [ SNew(STextBlock).Text(LOCTEXT("FilterStaticMeshLabel", "Static Mesh")) ]
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(SCheckBox)
                    .IsChecked(this, &SInstancePainterPanel::GetFilterStaticMesh)
                    .OnCheckStateChanged(this, &SInstancePainterPanel::OnFilterStaticMeshChanged)
                ]
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [ SNew(STextBlock).Text(LOCTEXT("FilterBSPLabel", "BSP")) ]
                + SHorizontalBox::Slot().AutoWidth()
                [
                    SNew(SCheckBox)
                    .IsChecked(this, &SInstancePainterPanel::GetFilterBSP)
                    .OnCheckStateChanged(this, &SInstancePainterPanel::OnFilterBSPChanged)
                ]
            ]

            // Max Slope Angle
            + SVerticalBox::Slot().AutoHeight().Padding(0, 2)
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot().FillWidth(0.4f).VAlign(VAlign_Center)
                [ SNew(STextBlock).Text(LOCTEXT("MaxSlopeLabel", "Max Slope Angle")) ]
                + SHorizontalBox::Slot().FillWidth(0.6f)
                [
                    SNew(SNumericEntryBox<float>)
                    .Value(this, &SInstancePainterPanel::GetMaxSlopeAngle)
                    .MinValue(0.0f)
                    .MaxValue(90.0f)
                    .MinSliderValue(0.0f)
                    .MaxSliderValue(90.0f)
                    .AllowSpin(true)
                    .OnValueChanged(this, &SInstancePainterPanel::OnMaxSlopeAngleChanged)
                ]
            ]

            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [ SNew(SSeparator) ]

            // ---- Actions ----
            + SVerticalBox::Slot().AutoHeight().Padding(0, 4)
            [
                SNew(SButton)
                .Text(LOCTEXT("ClearAllBtn", "Clear All Instances"))
                .HAlign(HAlign_Center)
                .OnClicked(this, &SInstancePainterPanel::OnClearAllClicked)
            ]
        ]
    ];
}

// ---------------------------------------------------------------------------
// Tool selection
// ---------------------------------------------------------------------------

FReply SInstancePainterPanel::OnPaintClicked()
{
    if (EdMode) EdMode->ActiveTool = EInstancePainterTool::Paint;
    return FReply::Handled();
}

FReply SInstancePainterPanel::OnEraseClicked()
{
    if (EdMode) EdMode->ActiveTool = EInstancePainterTool::Erase;
    return FReply::Handled();
}

FReply SInstancePainterPanel::OnSinglePlaceClicked()
{
    if (EdMode) EdMode->ActiveTool = EInstancePainterTool::SinglePlace;
    return FReply::Handled();
}

bool SInstancePainterPanel::IsPaintActive() const
{
    return EdMode && EdMode->ActiveTool == EInstancePainterTool::Paint;
}
bool SInstancePainterPanel::IsEraseActive() const
{
    return EdMode && EdMode->ActiveTool == EInstancePainterTool::Erase;
}
bool SInstancePainterPanel::IsSinglePlaceActive() const
{
    return EdMode && EdMode->ActiveTool == EInstancePainterTool::SinglePlace;
}

// ---------------------------------------------------------------------------
// Brush sliders
// ---------------------------------------------------------------------------

TOptional<float> SInstancePainterPanel::GetBrushRadius() const
{
    return EdMode ? TOptional<float>(EdMode->BrushSettings.BrushRadius) : TOptional<float>();
}
void SInstancePainterPanel::OnBrushRadiusChanged(float Value)
{
    if (EdMode) EdMode->BrushSettings.BrushRadius = FMath::Clamp(Value, 10.0f, 10000.0f);
}
void SInstancePainterPanel::OnBrushRadiusCommitted(float Value, ETextCommit::Type)
{
    OnBrushRadiusChanged(Value);
}

TOptional<float> SInstancePainterPanel::GetBrushFalloff() const
{
    return EdMode ? TOptional<float>(EdMode->BrushSettings.BrushFalloff) : TOptional<float>();
}
void SInstancePainterPanel::OnBrushFalloffChanged(float Value)
{
    if (EdMode) EdMode->BrushSettings.BrushFalloff = FMath::Clamp(Value, 0.0f, 1.0f);
}

TOptional<float> SInstancePainterPanel::GetPaintDensity() const
{
    return EdMode ? TOptional<float>(EdMode->BrushSettings.PaintDensity) : TOptional<float>();
}
void SInstancePainterPanel::OnPaintDensityChanged(float Value)
{
    if (EdMode) EdMode->BrushSettings.PaintDensity = FMath::Clamp(Value, 0.0f, 1.0f);
}

// ---------------------------------------------------------------------------
// Mask
// ---------------------------------------------------------------------------

TOptional<float> SInstancePainterPanel::GetMaskRotation() const
{
    return EdMode ? TOptional<float>(EdMode->BrushSettings.MaskRotation) : TOptional<float>();
}
void SInstancePainterPanel::OnMaskRotationChanged(float Value)
{
    if (EdMode) EdMode->BrushSettings.MaskRotation = Value;
}

ECheckBoxState SInstancePainterPanel::GetRandomizeMaskRotation() const
{
    if (!EdMode) return ECheckBoxState::Unchecked;
    return EdMode->BrushSettings.bRandomizeMaskRotation ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void SInstancePainterPanel::OnRandomizeMaskRotationChanged(ECheckBoxState NewState)
{
    if (EdMode) EdMode->BrushSettings.bRandomizeMaskRotation = (NewState == ECheckBoxState::Checked);
}

// ---------------------------------------------------------------------------
// Surface filters
// ---------------------------------------------------------------------------

ECheckBoxState SInstancePainterPanel::GetFilterLandscape() const
{
    if (!EdMode) return ECheckBoxState::Unchecked;
    return EdMode->BrushSettings.bFilterLandscape ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void SInstancePainterPanel::OnFilterLandscapeChanged(ECheckBoxState NewState)
{
    if (EdMode) EdMode->BrushSettings.bFilterLandscape = (NewState == ECheckBoxState::Checked);
}

ECheckBoxState SInstancePainterPanel::GetFilterStaticMesh() const
{
    if (!EdMode) return ECheckBoxState::Unchecked;
    return EdMode->BrushSettings.bFilterStaticMesh ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void SInstancePainterPanel::OnFilterStaticMeshChanged(ECheckBoxState NewState)
{
    if (EdMode) EdMode->BrushSettings.bFilterStaticMesh = (NewState == ECheckBoxState::Checked);
}

ECheckBoxState SInstancePainterPanel::GetFilterBSP() const
{
    if (!EdMode) return ECheckBoxState::Unchecked;
    return EdMode->BrushSettings.bFilterBSP ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void SInstancePainterPanel::OnFilterBSPChanged(ECheckBoxState NewState)
{
    if (EdMode) EdMode->BrushSettings.bFilterBSP = (NewState == ECheckBoxState::Checked);
}

TOptional<float> SInstancePainterPanel::GetMaxSlopeAngle() const
{
    return EdMode ? TOptional<float>(EdMode->BrushSettings.MaxSlopeAngle) : TOptional<float>();
}
void SInstancePainterPanel::OnMaxSlopeAngleChanged(float Value)
{
    if (EdMode) EdMode->BrushSettings.MaxSlopeAngle = FMath::Clamp(Value, 0.0f, 90.0f);
}

// ---------------------------------------------------------------------------
// Actions
// ---------------------------------------------------------------------------

FReply SInstancePainterPanel::OnClearAllClicked()
{
    if (EdMode && EdMode->TargetComponent.IsValid())
    {
        EdMode->TargetComponent->ClearAllInstances();
    }
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
