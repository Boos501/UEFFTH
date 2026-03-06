// Copyright (c) 2024 Boos501. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InstancePainterBrushSettings.generated.h"

/**
 * UInstancePainterBrushSettings
 * UObject wrapper for brush settings exposed via Details panel.
 * 笔刷设置的 UObject 包装，用于详细面板展示
 */
UCLASS()
class UInstancePainterBrushSettings : public UObject
{
    GENERATED_BODY()

public:
    /** Brush radius / 笔刷半径 */
    UPROPERTY(EditAnywhere, Category = "Brush",
        meta = (ClampMin = "10.0", ClampMax = "10000.0", DisplayName = "Brush Radius"))
    float BrushRadius = 500.0f;

    /** Brush falloff (0=hard, 1=full) / 衰减 */
    UPROPERTY(EditAnywhere, Category = "Brush",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Brush Falloff"))
    float BrushFalloff = 0.5f;

    /** Paint density / 绘制密度 */
    UPROPERTY(EditAnywhere, Category = "Brush",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Paint Density"))
    float PaintDensity = 0.1f;

    /** Erase density / 擦除密度 */
    UPROPERTY(EditAnywhere, Category = "Brush",
        meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Erase Density"))
    float EraseDensity = 0.1f;
};
