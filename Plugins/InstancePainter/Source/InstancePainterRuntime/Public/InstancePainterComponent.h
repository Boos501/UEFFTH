// Copyright (c) 2024 Boos501. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "InstancePainterTypes.h"
#include "InstancePainterComponent.generated.h"

/**
 * UInstancePainterComponent
 * Actor component that manages multiple HISM components for painted mesh instances.
 * 管理多个 HISM 组件，用于存储被笔刷绘制的 StaticMesh 实例。
 */
UCLASS(ClassGroup = (InstancePainter), meta = (BlueprintSpawnableComponent),
       DisplayName = "Instance Painter Component")
class INSTANCEPAINTERRUNTIME_API UInstancePainterComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInstancePainterComponent();

    /** List of mesh paint entries / 绘制条目列表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instance Painter",
        meta = (DisplayName = "Paint Entries"))
    TArray<FInstancePaintEntry> PaintEntries;

    /**
     * Get or create the HISM component for the given entry index.
     * 获取或创建对应条目的 HISM 组件
     */
    UFUNCTION(BlueprintCallable, Category = "Instance Painter")
    UHierarchicalInstancedStaticMeshComponent* GetOrCreateHISM(int32 EntryIndex);

    /**
     * Add a single instance at the given world transform.
     * 在指定世界坐标变换处添加一个实例
     */
    UFUNCTION(BlueprintCallable, Category = "Instance Painter")
    void AddInstance(int32 EntryIndex, const FTransform& WorldTransform);

    /**
     * Remove all instances within a sphere.
     * 移除球形范围内的所有实例
     */
    UFUNCTION(BlueprintCallable, Category = "Instance Painter")
    void RemoveInstancesInSphere(const FVector& Center, float Radius);

    /**
     * Clear all painted instances.
     * 清除所有实例
     */
    UFUNCTION(BlueprintCallable, Category = "Instance Painter")
    void ClearAllInstances();

protected:
    virtual void BeginPlay() override;

private:
    /** Map from entry index to HISM component / 条目索引 -> HISM 组件映射 */
    UPROPERTY()
    TMap<int32, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;
};
