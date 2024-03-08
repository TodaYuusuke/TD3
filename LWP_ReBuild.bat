@echo off

REM Visual Studio Developer Command Promptを使用する場合
REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

cd "DirectXGame"

REM Debugでビルド
MSBuild "DirectXGame.sln" /t:clean;rebuild /p:ConfigurationType=StaticLibrary;Configuration=Debug;MultiProcessorCompilation=true
REM Releaseでビルド
MSBuild "DirectXGame.sln" /t:clean;rebuild /p:ConfigurationType=StaticLibrary;Configuration=Release;MultiProcessorCompilation=true

REM システムのリソースをコピー
xcopy "resources/system" "../resources/system\" /e /y

pause