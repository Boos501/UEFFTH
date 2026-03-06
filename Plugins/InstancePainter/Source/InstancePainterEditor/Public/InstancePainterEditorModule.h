// Copyright (c) 2024 Boos501. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

/**
 * InstancePainterEditor module.
 * Registers the editor mode, toolbar button, and style set.
 * 注册编辑器模式、工具栏按钮和样式集
 */
class FInstancePainterEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterEditorMode();
    void UnregisterEditorMode();

    void RegisterToolbarExtension();
    void UnregisterToolbarExtension();

    void RegisterStyleSet();
    void UnregisterStyleSet();

    void OnToolbarButtonClicked();

    TSharedPtr<FSlateStyleSet> StyleSet;
    TSharedPtr<class FUICommandList> PluginCommands;
};
