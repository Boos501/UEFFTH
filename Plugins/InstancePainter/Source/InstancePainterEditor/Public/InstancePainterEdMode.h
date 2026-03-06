// Copyright (c) 2024 Boos501. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Tools/UEdMode.h"
#include "InstancePainterTypes.h"

class UInstancePainterComponent;
class SInstancePainterPanel;

/** Editor mode ID / 编辑器模式 ID */
#define EM_InstancePainter (FEditorModeID(TEXT("EM_InstancePainter")))

/** Active tool types / 当前工具类型 */
enum class EInstancePainterTool : uint8
{
    Paint,
    Erase,
    SinglePlace,
};

/**
 * FInstancePainterEdMode
 * Custom editor mode for brush-painting mesh instances.
 * 自定义编辑器模式，用于笔刷绘制网格体实例。
 */
class FInstancePainterEdMode : public FEdMode
{
public:
    FInstancePainterEdMode();
    virtual ~FInstancePainterEdMode() override;

    // FEdMode interface
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
    virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                          FKey Key, EInputEvent Event) override;
    virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                           int32 X, int32 Y) override;
    virtual bool StartTracking(FEditorViewportClient* ViewportClient, FViewport* Viewport) override;
    virtual bool EndTracking(FEditorViewportClient* ViewportClient, FViewport* Viewport) override;
    virtual bool CapturedMouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport,
                                   int32 InMouseX, int32 InMouseY) override;
    virtual bool UsesToolkits() const override { return true; }
    virtual void ActorSelectionChangeNotify() override {}

    /** Brush settings used for painting / 笔刷配置 */
    FInstanceBrushSettings BrushSettings;

    /** Currently active tool / 当前激活的工具 */
    EInstancePainterTool ActiveTool = EInstancePainterTool::Paint;

    /** Target component to paint on / 目标组件 */
    TWeakObjectPtr<UInstancePainterComponent> TargetComponent;

private:
    /** Current brush location in world space / 笔刷当前世界坐标位置 */
    FVector BrushLocation;
    FVector BrushNormal;
    bool bBrushVisible = false;
    bool bIsPainting = false;

    /** Perform a paint/erase stroke at the current brush location / 在当前位置执行绘制/擦除 */
    void DoPaint(FEditorViewportClient* ViewportClient);
    void DoErase();
    void DoSinglePlace(FEditorViewportClient* ViewportClient);

    /**
     * Trace a ray from screen position to find the world-space hit.
     * 从屏幕坐标追踪射线，找到世界坐标命中点
     */
    bool TraceFromScreen(FEditorViewportClient* ViewportClient, FVector& OutHitLocation,
                         FVector& OutHitNormal, AActor*& OutHitActor) const;

    /**
     * Update the brush location from the current mouse position.
     * 根据当前鼠标位置更新笔刷坐标
     */
    void UpdateBrushLocation(FEditorViewportClient* ViewportClient);

    /**
     * Sample the brush mask texture at the given brush-local position.
     * Returns a value in [0, 1] from the red channel.
     * 采样 Mask 纹理，返回红通道值 [0,1]
     */
    float SampleMaskTexture(const FVector2D& BrushUV) const;

    /**
     * Compute a random FTransform for a new instance.
     * 计算新实例的随机 FTransform
     */
    FTransform ComputeRandomTransform(const FInstancePaintEntry& Entry,
                                      const FVector& HitLocation,
                                      const FVector& HitNormal) const;

    /** Falloff weight at a given radial distance / 给定径向距离处的衰减权重 */
    float ComputeFalloffWeight(float Distance) const;

    /** Check if a surface is valid based on filter settings and actor type / 检查面是否通过过滤 */
    bool IsSurfaceValid(AActor* HitActor, const FVector& Normal) const;

    /** Last mouse position in viewport space / 鼠标在视口中的最后位置 */
    int32 LastMouseX = 0;
    int32 LastMouseY = 0;

    /** Randomly generated mask rotation for current stroke / 当前笔划的随机 Mask 旋转 */
    float ActiveMaskRotation = 0.0f;
};
