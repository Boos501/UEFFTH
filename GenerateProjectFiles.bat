@echo off
SET UE_ROOT=C:\Program Files\Epic Games\UE_5.4
SET PROJECT_FILE=%~dp0InstancePainterHost.uproject

IF NOT EXIST "%UE_ROOT%\Engine" (
    echo [ERROR] 引擎路径不存在: %UE_ROOT%
    pause
    exit /b 1
)

echo 生成项目文件...
call "%UE_ROOT%\Engine\Build\BatchFiles\GenerateProjectFiles.bat" -project="%PROJECT_FILE%" -game -engine

IF %ERRORLEVEL% NEQ 0 (
    echo [ERROR] 生成失败
    pause
    exit /b 1
)

echo.
echo 项目文件已生成！
echo 请用 Visual Studio 或 Rider 打开 InstancePainterHost.sln
pause
