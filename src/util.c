#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gl_funcs.h"
#include "util.h"
#include "stb_image.h"

#define FILE_SIZE 4000

unsigned int CompileShader (unsigned int shaderType, const char* filePath)
{
    const char* shaderSource = LoadFile (filePath);

    unsigned int shader = glCreateShader (shaderType);
    glShaderSource (shader, 1, &shaderSource, NULL);
    glCompileShader (shader);

    GetShaderCompileError (shader, shaderType);

    return shader;
}

unsigned int CreateShaderProgram  (const char* vertexShaderPath,const char* fragmentShaderPath)
{
    unsigned int vertexShader = CompileShader (GL_VERTEX_SHADER,  vertexShaderPath);
    unsigned int fragmentShader = CompileShader (GL_FRAGMENT_SHADER,  fragmentShaderPath);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram ();

    glAttachShader (shaderProgram, vertexShader);
    glAttachShader (shaderProgram, fragmentShader);
    glLinkProgram (shaderProgram);
    glDeleteShader (vertexShader);
    glDeleteShader (fragmentShader);

    return shaderProgram;
}

char *LoadFile (const char* filePath)
{
    FILE* file;
    file = fopen (filePath, "r");
    char* buffer = (char*) malloc (FILE_SIZE+1); // +1 for \0
    memset(buffer, 0, FILE_SIZE+1);
    const size_t returnCode = fread ((void*)buffer, sizeof(char), FILE_SIZE, file);

    buffer[FILE_SIZE] = '\0';

    if (!feof (file))
    {
        fprintf(stderr, "Error reading file %s: unexpected end of file.\n", filePath);
        exit(1);
        return NULL;
    }
    else if (ferror (file))
    {
        fprintf(stderr, "Error reading file %s\n", filePath);
        exit(1);
        return NULL;
    }

    return buffer;
}

void* LoadGLFunction (const char* name)
{
    void *function = (void *)wglGetProcAddress (name);
    if (!function)
    {
        fprintf(stderr, "Failed to load function: %s\n", name); 
        exit(1);
    }
    return function;
}

// Since windows.h has the same function but with a different signature, we replace Image with Img
bool LoadImg(const char* imagePath, Image* image)
{
    image->data = stbi_load (imagePath, &(image->width), &(image->height), &(image->colorChannels), 0);
    if (!image->data)
        return false;
    return true; 
}

int GetShaderCompileError (unsigned int shader, unsigned int shaderType)
{
    int success;
    char infoLog [2056];

    glGetShaderiv (shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog (shader, 2056, NULL, infoLog);
        switch (shaderType)
        {
            case GL_VERTEX_SHADER:
                {
                    printf ("OpenGL (GL_VERTEX_SHADER): %s\n",infoLog);
                } break;
            case GL_FRAGMENT_SHADER:
                {
                    printf ("OpenGL (GL_FRAGMENT_SHADER): %s\n",infoLog);
                } break;
        }
    }

    return success;
}

void WhatsTheProblemWindows ()
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 
    printf ("Error Code: %d",dw);
}

