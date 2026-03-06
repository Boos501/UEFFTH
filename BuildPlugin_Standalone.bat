@echo off
REM ============================================================
REM  使用 RunUAT 独立编译 InstancePainter 插件
REM  编译输出可直接复制到任何 UE 项目的 Plugins/ 下使用
REM ============================================================

SET UE_ROOT=C:\Program Files\Epic Games\UE_5.4
SET SCRIPT_DIR=%~dp0
SET PLUGIN_FILE=%SCRIPT_DIR%Plugins\InstancePainter\InstancePainter.uplugin
SET OUTPUT_DIR=%SCRIPT_DIR%BuiltPlugin

IF NOT EXIST "%UE_ROOT%\Engine" (
    echo [ERROR] 引擎路径不存在: %UE_ROOT%
    echo 请修改 UE_ROOT 变量
    pause
    exit /b 1
)

IF NOT EXIST "%PLUGIN_FILE%" (
    echo [ERROR] 插件文件不存在: %PLUGIN_FILE%
    pause
    exit /b 1
)

echo.
echo ============================================================
echo  独立编译 InstancePainter 插件
echo ============================================================
echo  输出目录: %OUTPUT_DIR%
echo ============================================================
echo.

REM -Rocket 适用于 Epic Games Launcher 分发的二进制引擎版本（推荐）
REM 若使用从源码自行编译的引擎，可删除 -Rocket 参数
call "%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin ^
    -Plugin="%PLUGIN_FILE%" ^
    -Package="%OUTPUT_DIR%" ^
    -TargetPlatforms=Win64 ^
    -Rocket

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] 编译失败！查看上方日志获取详细错误
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================================
echo  编译成功!
echo ============================================================
echo  插件已输出到: %OUTPUT_DIR%
echo.
echo  使用方式:
echo    将 %OUTPUT_DIR%\InstancePainter 文件夹
echo    复制到目标 UE 项目的 Plugins/ 目录下
echo    重启编辑器即可
echo.
pause
