// Copyright (c) 2024 Boos501. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class FInstancePainterEdMode;
enum class EInstancePainterTool : uint8;

/**
 * SInstancePainterPanel
 * Slate panel displayed in the editor mode toolkit.
 * 编辑器模式工具面板（Slate）
 */
class SInstancePainterPanel : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SInstancePainterPanel) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, FInstancePainterEdMode* InEdMode);

private:
    FInstancePainterEdMode* EdMode = nullptr;

    // Tool selection / 工具选择
    FReply OnPaintClicked();
    FReply OnEraseClicked();
    FReply OnSinglePlaceClicked();

    // Brush sliders / 笔刷滑条
    TOptional<float> GetBrushRadius() const;
    void OnBrushRadiusChanged(float Value);
    void OnBrushRadiusCommitted(float Value, ETextCommit::Type);

    TOptional<float> GetBrushFalloff() const;
    void OnBrushFalloffChanged(float Value);

    TOptional<float> GetPaintDensity() const;
    void OnPaintDensityChanged(float Value);

    // Mask / Mask 设置
    TOptional<float> GetMaskRotation() const;
    void OnMaskRotationChanged(float Value);

    ECheckBoxState GetRandomizeMaskRotation() const;
    void OnRandomizeMaskRotationChanged(ECheckBoxState NewState);

    // Surface filters / 表面过滤
    ECheckBoxState GetFilterLandscape() const;
    void OnFilterLandscapeChanged(ECheckBoxState NewState);

    ECheckBoxState GetFilterStaticMesh() const;
    void OnFilterStaticMeshChanged(ECheckBoxState NewState);

    ECheckBoxState GetFilterBSP() const;
    void OnFilterBSPChanged(ECheckBoxState NewState);

    TOptional<float> GetMaxSlopeAngle() const;
    void OnMaxSlopeAngleChanged(float Value);

    // Actions / 操作
    FReply OnClearAllClicked();

    // Highlight active tool button / 高亮当前工具按钮
    bool IsPaintActive() const;
    bool IsEraseActive() const;
    bool IsSinglePlaceActive() const;
};
