/* __vimdothis__
packadd a.vim
packadd vim-fugitive
set foldmethod=marker
__vimendthis__ */

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

typedef struct 
{
    char* data;
    int width;
    int height;
    int colorChannels;
} Image;

const char* LoadFile (const char* filePath);

// The _ because windows.h already defines a LoadImage
bool _LoadImage(const char* imagePath, Image* image);

unsigned int CompileShader (unsigned int shaderType, const char* filePath);

unsigned int CreateShaderProgram (const char* vertexShaderPath,const char* fragmentShaderPath);

void* LoadGLFunction (const char* name);

int GetShaderCompileError (unsigned int shader, unsigned int shaderType);

void WhatsTheProblemWindows ();

bool _LoadImage (const char* imagePath, Image* image);

#endif
