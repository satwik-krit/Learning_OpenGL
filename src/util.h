#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

typedef struct 
{
    unsigned char* data;
    int width;
    int height;
    int colorChannels;
} Image;

char* LoadFile (const char* filePath);

// The _ because windows.h already defines a LoadImage
bool LoadImg(const char* imagePath, Image* image);

unsigned int CompileShader (unsigned int shaderType, const char* filePath);

unsigned int CreateShaderProgram (const char* vertexShaderPath,const char* fragmentShaderPath);

void* LoadGLFunction (const char* name);

int GetShaderCompileError (unsigned int shader, unsigned int shaderType);

void WhatsTheProblemWindows ();

#endif
