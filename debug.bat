@echo off
set ABSPATH=C:\Users\sande\Documents\Projects\OpenGL\Learning_OpenGL\
set output=out
set include=include
set source=src
pushd %ABSPATH%
echo "Cleaning up..."
mkdir %output%
del %output%\main.exe
echo "Compiling..."
gcc %source%/*.c -Iinclude -luser32 -lgdi32 -lopengl32 -g -o out/main
rem gcc %source%/*.c -I%include% -luser32 -lgdi32 -lopengl32 -g -E > translated.c
echo "Running..."
%output%\main
popd
