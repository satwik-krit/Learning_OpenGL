/* __vimdothis__
let b:dispatch = 'mingw32-make'
packadd a
__vimendthis__ */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include "gl_funcs.h"
#include "util.h"
#include "stb_image.h"

#define FILE_SIZE 4000

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
   FILE* file;
   file = fopen (filePath, "r");
   const char* buffer = (const char*) malloc (FILE_SIZE);
   const size_t returnCode = fread ((void*)buffer, sizeof(char), FILE_SIZE, file);
   
    /* printf("%s: %s", filePath, buffer); */ 

   if (!feof (file))
   {
       fprintf(stderr, "Error reading file %s: unexpected end of file.\n", filePath);
       return NULL;
   }
   else if (ferror (file))
   {
       fprintf(stderr, "Error reading file %s\n", filePath);
       return NULL;
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

// We have to add a _ prefix since windows.h has the same function but with a
// different signature.
bool
_LoadImage(const char* imagePath, Image* image)
{
    image->data = stbi_load (imagePath, &(image->width), &(image->height), &(image->colorChannels), 0);
    if (!image->data)
       return false;
    return true; 
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
    printf ("OpenGL : %s\n",infoLog);
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

