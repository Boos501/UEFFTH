@echo off
REM ============================================================
REM  InstancePainter 一键编译脚本 (Windows x64)
REM  用法: 修改下方 UE_ROOT 为你的引擎安装路径，然后双击运行
REM ============================================================

REM ===================== 修改这里 =====================
SET UE_ROOT=C:\Program Files\Epic Games\UE_5.7
REM ====================================================

SET PROJECT_FILE=%~dp0UEFFTH.uproject
SET UBT="%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
SET UAT="%UE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat"

echo.
echo ========================================
echo  InstancePainter Build Script
echo ========================================
echo  Engine: %UE_ROOT%
echo  Project: %PROJECT_FILE%
echo ========================================
echo.

REM 检查引擎路径是否存在
if not exist "%UE_ROOT%\Engine" (
    echo [ERROR] 找不到 UE 引擎路径: %UE_ROOT%
    echo [ERROR] 请编辑此脚本，修改 UE_ROOT 为你的引擎安装路径
    echo.
    echo 常见路径:
    echo   Epic Launcher:  C:\Program Files\Epic Games\UE_5.7
    echo   自定义安装:     D:\UnrealEngine\UE_5.7
    echo   源码编译:       D:\UnrealEngine
    echo.
    pause
    exit /b 1
)

REM 检查项目文件
if not exist "%PROJECT_FILE%" (
    echo [ERROR] 找不到项目文件: %PROJECT_FILE%
    echo [ERROR] 请确保此脚本位于仓库根目录
    pause
    exit /b 1
)

echo [1/2] 编译 UEFFTHEditor (Development, Win64)...
echo.

call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" UEFFTHEditor Win64 Development -Project="%PROJECT_FILE%" -WaitMutex -FromMsBuild

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] 编译失败! 错误码: %ERRORLEVEL%
    echo.
    echo 常见问题:
    echo   1. 检查 UE_ROOT 路径是否正确
    echo   2. 确保已安装 Visual Studio 2022 + "使用 C++ 的游戏开发" 工作负载
    echo   3. 确保已安装 .NET 6.0 SDK
    echo   4. 检查 Source 代码中是否有语法错误
    echo.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ========================================
echo  编译成功!
echo ========================================
echo.
echo 下一步:
echo   1. 双击 UEFFTH.uproject 打开编辑器
echo   2. 或运行 GenerateProjectFiles.bat 生成 VS 工程文件
echo.
pause
