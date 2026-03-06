@echo off
REM ============================================================
REM  清理编译缓存（编译出错时使用）
REM ============================================================

echo.
echo ========================================
echo  清理编译缓存
echo ========================================
echo.

echo 正在清理...

if exist "%~dp0Binaries" (
    echo   删除 Binaries/
    rmdir /s /q "%~dp0Binaries"
)

if exist "%~dp0Intermediate" (
    echo   删除 Intermediate/
    rmdir /s /q "%~dp0Intermediate"
)

if exist "%~dp0Saved" (
    echo   删除 Saved/
    rmdir /s /q "%~dp0Saved"
)

if exist "%~dp0DerivedDataCache" (
    echo   删除 DerivedDataCache/
    rmdir /s /q "%~dp0DerivedDataCache"
)

if exist "%~dp0.vs" (
    echo   删除 .vs/
    rmdir /s /q "%~dp0.vs"
)

if exist "%~dp0Plugins\InstancePainter\Binaries" (
    echo   删除 Plugins/InstancePainter/Binaries/
    rmdir /s /q "%~dp0Plugins\InstancePainter\Binaries"
)

if exist "%~dp0Plugins\InstancePainter\Intermediate" (
    echo   删除 Plugins/InstancePainter/Intermediate/
    rmdir /s /q "%~dp0Plugins\InstancePainter\Intermediate"
)

if exist "%~dp0BuiltPlugin" (
    echo   删除 BuiltPlugin/
    rmdir /s /q "%~dp0BuiltPlugin"
)

echo.
echo 清理完成! 现在可以重新编译。
echo.
pause
