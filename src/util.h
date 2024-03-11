#ifndef UTIL_H
#define UTIL_H

struct Shader
{
    int ID;
    const char* content;
};

const char* LoadFile (const char* FilePath);

void LoadShader ();

void* LoadGLFunction (const char* name);

int GetShaderCompileError (unsigned int shader);

void WhatsTheProblemWindows ();

#endif
