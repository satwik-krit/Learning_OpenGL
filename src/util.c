/* __vimdothis__
let b:dispatch = 'mingw32-make'
packadd a
__vimendthis__ */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "gl_funcs.h"

#include "util.h"

unsigned int
CompileShader (unsigned int shaderType, const char* filePath)
{
    const char* shaderSource = LoadFile (filePath);

    unsigned int shader = glCreateShader (shaderType);
    glShaderSource (shader, 1, &shaderSource, NULL);
    glCompileShader (shader);

    GetShaderCompileError (shader, shaderType);

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

const char * 
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
       return NULL;
   }

   unsigned int fileSize = GetFileSize (File, NULL);

   const char* buffer =  (const char*)malloc (sizeof (char)*fileSize);
   
   DWORD BytesRead;
   int status;

   status = ReadFile (File, (void *)buffer, fileSize, &BytesRead, NULL);
   /* printf("Buffer: %s", (char *)buffer); */

   if (!status)
   {
       WhatsTheProblemWindows ();
   }
   return buffer;
}

void*
LoadGLFunction (const char* name)
{
  void *function = (void *)wglGetProcAddress (name);
  if (!function)
  {
     fprintf(stderr, "Failed to load function: %s\n", name); 
     exit(1);
  }
  return function;
}

int
GetShaderCompileError (unsigned int shader, unsigned int shaderType)
{
  int success;
  char infoLog [2056];

  glGetShaderiv (shader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog (shader, 2056, NULL, infoLog);
  }
  /* OutputDebugString (infoLog); */
  printf ("OpenGL : %s\n",infoLog);

/*  switch (shaderType)
  {
      case GL_VERTEX_SHADER:
      {
        printf ("OpenGL (Vertex): %s\n",infoLog);
      } break;

      case GL_FRAGMENT_SHADER:
      {
        printf ("OpenGL (Fragment): %s\n",infoLog);
      } break;
      default:
      {
        printf ("OpenGL (): %s\n",infoLog);
      } break; 
  } */


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

