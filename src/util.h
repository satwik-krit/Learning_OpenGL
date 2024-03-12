#ifndef UTIL_H
#define UTIL_H

const char* LoadFile (const char* filePath);

unsigned int CompileShader (unsigned int shaderType, const char* filePath);

unsigned int CreateShaderProgram (const char* vertexShaderPath,const char* fragmentShaderPath);

void* LoadGLFunction (const char* name);

int GetShaderCompileError (unsigned int shader);

void WhatsTheProblemWindows ();

#endif
