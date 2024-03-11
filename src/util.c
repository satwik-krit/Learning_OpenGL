#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "gl_funcs.h"

#include "util.h"

void
LoadShader ()
{
}

const char* 
LoadFile (const char* FilePath)
{
   HANDLE File = CreateFileA (
            FilePath,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
           ); 

   if (File == INVALID_HANDLE_VALUE)
   {
       printf ("Failed to open file at: %s\n",FilePath);
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

