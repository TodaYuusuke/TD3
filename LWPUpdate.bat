@echo off
REM Visual Studio Developer Command Promptを使用する場合
REM call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

cd "DirectXGame"
REM 変更をすべて取り消す
git reset --hard
REM Gitで最新のコードを取得
git pull origin LWP_20231213

REM MSBuildのパス
REM set MSBUILD_PATH="C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe"
REM VSINSTALLDIR=MSBuild\Microsoft\VC\v170\

REM Visual Studioでビルド
REM %MSBUILD_PATH% "DirectXGame.sln"

REM Debugでビルド
MSBuild "DirectXGame.sln" /t:clean;rebuild /p:ConfigurationType=StaticLibrary;Configuration=Debug;MultiProcessorCompilation=true
REM Releaseでビルド
MSBuild "DirectXGame.sln" /t:clean;rebuild /p:ConfigurationType=StaticLibrary;Configuration=Release;MultiProcessorCompilation=true

REM システムのリソースをコピー
xcopy "resources/system" "../resources/system\" /e /y

pause