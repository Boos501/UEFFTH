@echo off
REM ============================================================
REM  生成 Visual Studio / Rider 项目文件
REM  用法: 修改下方 UE_ROOT 为你的引擎安装路径，然后双击运行
REM ============================================================

REM ===================== 修改这里 =====================
SET UE_ROOT=C:\Program Files\Epic Games\UE_5.4
REM ====================================================

SET PROJECT_FILE=%~dp0UEFFTH.uproject

echo.
echo ========================================
echo  生成项目文件
echo ========================================
echo.

if not exist "%UE_ROOT%\Engine" (
    echo [ERROR] 找不到 UE 引擎路径: %UE_ROOT%
    echo [ERROR] 请编辑此脚本，修改 UE_ROOT 为你的引擎安装路径
    pause
    exit /b 1
)

if not exist "%PROJECT_FILE%" (
    echo [ERROR] 找不到项目文件: %PROJECT_FILE%
    pause
    exit /b 1
)

echo 正在生成项目文件...
call "%UE_ROOT%\Engine\Build\BatchFiles\GenerateProjectFiles.bat" -project="%PROJECT_FILE%" -game -engine

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] 生成项目文件失败!
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo ========================================
echo  项目文件生成成功!
echo ========================================
echo.
echo 现在可以打开 UEFFTH.sln 进行开发
echo.
pause
