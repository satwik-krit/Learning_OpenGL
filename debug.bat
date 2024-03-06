@echo off
set output=out
set include=include
set source=src
pushd %~dp0
echo "Building tags"
ctags -R
echo "Cleaning up..."
mkdir %output%
del %output%\main.exe
echo "Compiling..."
gcc %source%/*.c -Iinclude -luser32 -lgdi32 -lopengl32 -g -o out/main
rem gcc %source%/*.c -I%include% -luser32 -lgdi32 -lopengl32 -g -E > translated.c
echo "Running..."
%output%\main
popd
