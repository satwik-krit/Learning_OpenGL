#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <glad/glad.h>

#include "util.h"

// We are not using this as of now. GLAD handles the function loading
void*
LoadGLFunction (const char* name)
{
  void *function = (void *)wglGetProcAddress (name);
  // TODO: Handle case where function is NULL or 0
  return function;
}

int
ShaderCompileError (unsigned int shader)
{
  int success;
  char infoLog [1024];
  glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog (shader, 1024, NULL, infoLog);
    printf ("%s\n",infoLog);
  }

  return success;
}
