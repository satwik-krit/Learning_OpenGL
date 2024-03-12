#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "gl_funcs.h"

#include "util.h"

unsigned int
CompileShader (unsigned int shaderType, const char* filePath)
{
    const char* Shadersource = LoadFile (filePath);
    unsigned int shader = glCreateShader (shaderType);
    glShaderSource (shader, 1, &Shadersource, NULL);
    glCompileShader (shader);
    GetShaderCompileError (shader);
    return shader;
}

unsigned int
CreateShaderProgram  (const char* vertexShaderPath,const char* fragmentShaderPath)
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

const char* 
LoadFile (const char* filePath)
{
   HANDLE File = CreateFileA (
            filePath,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
           ); 

   if (File == INVALID_HANDLE_VALUE)
   {
       printf ("Failed to open file at: %s\n",filePath);
       WhatsTheProblemWindows ();
   }

   const char* buffer =  (const char*)malloc (sizeof (char)*250);
   DWORD BytesRead;
   ReadFile (File, buffer, 250, &BytesRead, NULL);
   return buffer;
}

void*
LoadGLFunction (const char* name)
{
  void *function = (void *)wglGetProcAddress (name);
  // TODO: Handle case where function is NULL or 0
  return function;
}

int
GetShaderCompileError (unsigned int shader)
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

void 
WhatsTheProblemWindows ()
{
  LPVOID lpMsgBuf;
  LPVOID lpDisplayBuf;
  DWORD dw = GetLastError(); 
  printf ("Error Code: %d",dw);
}

