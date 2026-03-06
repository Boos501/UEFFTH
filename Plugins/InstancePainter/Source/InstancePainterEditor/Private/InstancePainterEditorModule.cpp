// Copyright (c) 2024 Boos501. All Rights Reserved.

#include "InstancePainterEditorModule.h"
#include "InstancePainterEdMode.h"
#include "EditorModeRegistry.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateStyle.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#define LOCTEXT_NAMESPACE "FInstancePainterEditorModule"

void FInstancePainterEditorModule::StartupModule()
{
    RegisterStyleSet();
    RegisterEditorMode();
    RegisterToolbarExtension();
}

void FInstancePainterEditorModule::ShutdownModule()
{
    UnregisterToolbarExtension();
    UnregisterEditorMode();
    UnregisterStyleSet();
}

// ---- Style Set ----

void FInstancePainterEditorModule::RegisterStyleSet()
{
    StyleSet = MakeShareable(new FSlateStyleSet("InstancePainterStyle"));

    // Point to the plugin's Resources directory for icons
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("InstancePainter"));
    if (Plugin.IsValid())
    {
        StyleSet->SetContentRoot(Plugin->GetBaseDir() / TEXT("Resources"));
    }

    FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

void FInstancePainterEditorModule::UnregisterStyleSet()
{
    if (StyleSet.IsValid())
    {
        FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
        StyleSet.Reset();
    }
}

// ---- Editor Mode ----

void FInstancePainterEditorModule::RegisterEditorMode()
{
    FEditorModeRegistry::Get().RegisterMode<FInstancePainterEdMode>(
        EM_InstancePainter,
        LOCTEXT("InstancePainterEdModeName", "Instance Painter"),
        FSlateIcon(),
        true /* visible in toolbar */
    );
}

void FInstancePainterEditorModule::UnregisterEditorMode()
{
    FEditorModeRegistry::Get().UnregisterMode(EM_InstancePainter);
}

// ---- Toolbar Extension ----

void FInstancePainterEditorModule::RegisterToolbarExtension()
{
    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FInstancePainterEditorModule::OnToolbarButtonClicked));
}

void FInstancePainterEditorModule::UnregisterToolbarExtension()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
}

void FInstancePainterEditorModule::OnToolbarButtonClicked()
{
    // Activate the editor mode when the toolbar button is pressed
    if (GLevelEditorModeTools().IsModeActive(EM_InstancePainter))
    {
        GLevelEditorModeTools().DeactivateMode(EM_InstancePainter);
    }
    else
    {
        GLevelEditorModeTools().ActivateMode(EM_InstancePainter);
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInstancePainterEditorModule, InstancePainterEditor)
