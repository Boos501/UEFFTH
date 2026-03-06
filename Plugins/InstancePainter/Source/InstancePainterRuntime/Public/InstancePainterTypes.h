// Copyright (c) 2024 Boos501. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"
#include "InstancePainterTypes.generated.h"

#define LOCTEXT_NAMESPACE "InstancePainter"

/**
 * FInstancePaintEntry
 * Configuration for a single mesh type in the painter.
 * 单个 Mesh 类型的绘制配置
 */
USTRUCT(BlueprintType)
struct INSTANCEPAINTERRUNTIME_API FInstancePaintEntry
{
    GENERATED_BODY()

    /** The Static Mesh to paint / 要绘制的 StaticMesh */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (DisplayName = "Static Mesh"))
    TSoftObjectPtr<UStaticMesh> Mesh;

    /** Override materials for the mesh / 覆盖材质 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (DisplayName = "Override Materials"))
    TArray<TSoftObjectPtr<UMaterialInterface>> OverrideMaterials;

    /** Weight used when randomly selecting among multiple entries / 多类型混合时的权重 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (ClampMin = "0.0", DisplayName = "Weight"))
    float Weight = 1.0f;

    // ---- Scale ----

    /** Use uniform scale across all axes / 是否使用统一缩放 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale", meta = (DisplayName = "Uniform Scale"))
    bool bUniformScale = true;

    /** Minimum uniform scale / Uniform 最小缩放 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale",
        meta = (ClampMin = "0.001", DisplayName = "Scale Min", EditCondition = "bUniformScale"))
    float ScaleMin = 0.8f;

    /** Maximum uniform scale / Uniform 最大缩放 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale",
        meta = (ClampMin = "0.001", DisplayName = "Scale Max", EditCondition = "bUniformScale"))
    float ScaleMax = 1.2f;

    /** Minimum non-uniform scale (per-axis) / Non-Uniform 最小缩放（各轴）*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale",
        meta = (DisplayName = "Scale Min 3D", EditCondition = "!bUniformScale"))
    FVector ScaleMin3D = FVector(0.8f, 0.8f, 0.8f);

    /** Maximum non-uniform scale (per-axis) / Non-Uniform 最大缩放（各轴）*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scale",
        meta = (DisplayName = "Scale Max 3D", EditCondition = "!bUniformScale"))
    FVector ScaleMax3D = FVector(1.2f, 1.2f, 1.2f);

    // ---- Rotation ----

    /** Yaw (Z) rotation range in degrees / Yaw 旋转范围（度） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation",
        meta = (DisplayName = "Yaw Range (degrees)"))
    FVector2D YawRange = FVector2D(0.0f, 360.0f);

    /** Pitch (Y) rotation range in degrees / Pitch 旋转范围（度） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation",
        meta = (DisplayName = "Pitch Range (degrees)"))
    FVector2D PitchRange = FVector2D(0.0f, 0.0f);

    /** Roll (X) rotation range in degrees / Roll 旋转范围（度） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation",
        meta = (DisplayName = "Roll Range (degrees)"))
    FVector2D RollRange = FVector2D(0.0f, 0.0f);

    // ---- Normal Alignment ----

    /** Align instance up-axis to the surface normal / 是否对齐法线 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement",
        meta = (DisplayName = "Align to Normal"))
    bool bAlignToNormal = true;

    /**
     * How strongly the instance aligns to the surface normal.
     * 0 = always vertical, 1 = fully aligned to normal
     * 法线对齐强度 (0=竖直, 1=完全贴合法线)
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Normal Alignment Strength",
                EditCondition = "bAlignToNormal"))
    float NormalAlignmentStrength = 1.0f;

    // ---- Placement ----

    /** Height offset range along the surface normal / 沿法线的高度偏移范围 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement",
        meta = (DisplayName = "Height Offset Range"))
    FVector2D HeightOffsetRange = FVector2D(0.0f, 0.0f);

    /** Minimum distance between instances / 实例间最小距离 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement",
        meta = (ClampMin = "0.0", DisplayName = "Min Distance"))
    float MinDistance = 50.0f;
};

/**
 * FInstanceBrushSettings
 * Brush configuration used during painting / 笔刷配置
 */
USTRUCT(BlueprintType)
struct INSTANCEPAINTERRUNTIME_API FInstanceBrushSettings
{
    GENERATED_BODY()

    /** Brush radius in world units / 笔刷半径（世界单位） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush",
        meta = (ClampMin = "10.0", ClampMax = "10000.0", DisplayName = "Brush Radius"))
    float BrushRadius = 500.0f;

    /** Brush falloff (0 = hard edge, 1 = full falloff) / 衰减 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Brush Falloff"))
    float BrushFalloff = 0.5f;

    /** Density when painting / 绘制密度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Paint Density"))
    float PaintDensity = 0.1f;

    /** Density when erasing / 擦除密度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brush",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Erase Density"))
    float EraseDensity = 0.1f;

    /** Optional alpha mask texture (red channel used) / Alpha Mask 贴图（使用红通道） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask",
        meta = (DisplayName = "Brush Mask"))
    TSoftObjectPtr<UTexture2D> BrushMask;

    /** Fixed rotation applied to the mask / Mask 旋转角度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask",
        meta = (DisplayName = "Mask Rotation"))
    float MaskRotation = 0.0f;

    /** Randomize mask rotation each stroke / 是否随机旋转 Mask */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask",
        meta = (DisplayName = "Randomize Mask Rotation"))
    bool bRandomizeMaskRotation = false;

    // ---- Surface Filters ----

    /** Allow painting on Landscape / 允许绘制到地形 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter",
        meta = (DisplayName = "Filter Landscape"))
    bool bFilterLandscape = true;

    /** Allow painting on Static Meshes / 允许绘制到静态网格 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter",
        meta = (DisplayName = "Filter Static Mesh"))
    bool bFilterStaticMesh = true;

    /** Allow painting on BSP brushes / 允许绘制到 BSP */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter",
        meta = (DisplayName = "Filter BSP"))
    bool bFilterBSP = true;

    /** Maximum allowable slope angle in degrees / 最大坡度（度） */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter",
        meta = (ClampMin = "0.0", ClampMax = "90.0", DisplayName = "Max Slope Angle"))
    float MaxSlopeAngle = 45.0f;
};

#undef LOCTEXT_NAMESPACE
