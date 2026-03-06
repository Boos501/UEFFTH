@echo off
REM ============================================================
REM  InstancePainter UE 工程编译脚本 (Windows)
REM
REM  使用方式:
REM    1. 修改下方 UE_ROOT 为你的 UE 引擎安装路径
REM    2. 双击运行本脚本
REM
REM  默认配置针对 UE 5.4；其他版本请同步修改 UE_ROOT 路径
REM  以及 InstancePainterHost.uproject 中的 EngineAssociation 字段
REM ============================================================

REM === 修改为你本地的 UE 引擎路径 ===
SET UE_ROOT=C:\Program Files\Epic Games\UE_5.4

REM === 自动推导路径 ===
SET SCRIPT_DIR=%~dp0
SET PROJECT_FILE=%SCRIPT_DIR%InstancePainterHost.uproject
SET UBT=%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
SET GEN_PROJ=%UE_ROOT%\Engine\Build\BatchFiles\GenerateProjectFiles.bat

REM 检查引擎路径
IF NOT EXIST "%UE_ROOT%\Engine" (
    echo.
    echo [ERROR] 未找到 UE 引擎路径: %UE_ROOT%
    echo 请修改本脚本中的 UE_ROOT 变量
    echo.
    pause
    exit /b 1
)

REM 检查 .uproject 文件
IF NOT EXIST "%PROJECT_FILE%" (
    echo.
    echo [ERROR] 未找到项目文件: %PROJECT_FILE%
    echo 请确保本脚本位于仓库根目录
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================================
echo  InstancePainter 编译工具
echo ============================================================
echo  引擎路径: %UE_ROOT%
echo  项目文件: %PROJECT_FILE%
echo ============================================================
echo.

echo [1/3] 生成项目文件...
call "%GEN_PROJ%" -project="%PROJECT_FILE%" -game -engine 2>nul
echo.

echo [2/3] 编译 Development Editor (Win64)...
echo.

REM UE 5.x 使用 dotnet 版 UBT
IF EXIST "%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    "%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" InstancePainterHostEditor Win64 Development -project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
) ELSE IF EXIST "%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool.exe" (
    REM UE 4.x 回退
    "%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool.exe" InstancePainterHostEditor Win64 Development -project="%PROJECT_FILE%" -WaitMutex -FromMsBuild
) ELSE (
    echo [ERROR] 未找到 UnrealBuildTool
    pause
    exit /b 1
)

IF %ERRORLEVEL% NEQ 0 (
    echo.
    echo ============================================================
    echo  [ERROR] 编译失败!
    echo ============================================================
    echo.
    echo  常见问题排查:
    echo    1. UE_ROOT 路径是否正确？
    echo    2. .uplugin 中的模块名是否与 Build.cs 匹配？
    echo    3. Build.cs 中的依赖模块是否都存在？
    echo    4. 头文件 #include 路径是否正确？
    echo.
    echo  尝试方法:
    echo    - 在 UE 编辑器中打开 .uproject 查看编译错误详情
    echo    - 使用 VS/Rider 打开 .sln 编译查看详细错误
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================================
echo  [3/3] 编译成功!
echo ============================================================
echo.
echo  下一步:
echo    方式1: 双击 InstancePainterHost.uproject 打开编辑器
echo    方式2: 用 VS/Rider 打开生成的 .sln 文件
echo.
echo  在编辑器中:
echo    1. 确认 Plugins 菜单中 "Instance Painter" 已启用
echo    2. 场景中添加任意 Actor
echo    3. 给它添加 InstancePainterComponent
echo    4. 在 Modes 面板中切换到 "Instance Painter" 模式
echo    5. 开始绘制!
echo.
pause
