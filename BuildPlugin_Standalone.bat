@echo off
REM ============================================================
REM  独立编译 InstancePainter 插件（可分发到其他项目）
REM  编译输出在 BuiltPlugin/ 目录
REM ============================================================

REM ===================== 修改这里 =====================
SET UE_ROOT=C:\Program Files\Epic Games\UE_5.4
REM ====================================================

SET PROJECT_FILE=%~dp0UEFFTH.uproject
SET PLUGIN_FILE=%~dp0Plugins\InstancePainter\InstancePainter.uplugin
SET OUTPUT_DIR=%~dp0BuiltPlugin

echo.
echo ========================================
echo  独立编译 InstancePainter 插件
echo ========================================
echo.

if not exist "%UE_ROOT%\Engine" (
    echo [ERROR] 找不到 UE 引擎路径: %UE_ROOT%
    echo [ERROR] 请编辑此脚本，修改 UE_ROOT 为你的引擎安装路径
    pause
    exit /b 1
)

if not exist "%PLUGIN_FILE%" (
    echo [ERROR] 找不到插件文件: %PLUGIN_FILE%
    echo [ERROR] 请确保 Plugins/InstancePainter/ 目录存在
    pause
    exit /b 1
)

echo 正在编译插件 (RunUAT BuildPlugin)...
echo 输出目录: %OUTPUT_DIR%
echo.

call "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%PLUGIN_FILE%" -Package="%OUTPUT_DIR%" -TargetPlatforms=Win64 -Rocket

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] 插件编译失败!
    echo.
    echo 常见问题:
    echo   1. 检查 UE_ROOT 路径
    echo   2. 确保 Visual Studio 2022 已安装
    echo   3. 检查插件源码是否有错误
    echo.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ========================================
echo  插件编译成功!
echo ========================================
echo.
echo 输出位置: %OUTPUT_DIR%
echo.
echo 使用方法:
echo   将 %OUTPUT_DIR%\InstancePainter 复制到任意 UE 项目的 Plugins/ 目录下
echo.
pause
