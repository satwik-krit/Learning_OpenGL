#ifndef GL_FUNCS_H
#define GL_FUNCS_H 
#include <gl/gl.h>

#include "glext.h"
#include "wglext.h"

#define DECLARE_GLFUNCTION(func_name,token_name) extern PFN##token_name##PROC func_name;

void LoadGLFunctionPointers (void);

DECLARE_GLFUNCTION (glUseProgram, GLUSEPROGRAM)
DECLARE_GLFUNCTION (glCreateProgram, GLCREATEPROGRAM)
DECLARE_GLFUNCTION (glDeleteShader, GLDELETESHADER)
DECLARE_GLFUNCTION (glLinkProgram, GLLINKPROGRAM)
DECLARE_GLFUNCTION (glShaderSource, GLSHADERSOURCE)
DECLARE_GLFUNCTION (glAttachShader, GLATTACHSHADER)
DECLARE_GLFUNCTION (glGenBuffers, GLGENBUFFERS)
DECLARE_GLFUNCTION (glBindBuffer, GLBINDBUFFER)
DECLARE_GLFUNCTION (glBufferData, GLBUFFERDATA)
DECLARE_GLFUNCTION (glCreateShader, GLCREATESHADER)
DECLARE_GLFUNCTION (glCompileShader, GLCOMPILESHADER)
DECLARE_GLFUNCTION (glVertexAttribPointer, GLVERTEXATTRIBPOINTER)
DECLARE_GLFUNCTION (glEnableVertexAttribArray, GLENABLEVERTEXATTRIBARRAY)
DECLARE_GLFUNCTION (glGenVertexArrays, GLGENVERTEXARRAYS)
DECLARE_GLFUNCTION (glBindVertexArray, GLBINDVERTEXARRAY)
DECLARE_GLFUNCTION (glGetShaderiv, GLGETSHADERIV)
DECLARE_GLFUNCTION (glGetShaderInfoLog, GLGETSHADERINFOLOG)
DECLARE_GLFUNCTION (glGetUniformLocation, GLGETUNIFORMLOCATION) 
DECLARE_GLFUNCTION (glUniform4f, GLUNIFORM4F)
DECLARE_GLFUNCTION (glUniform1i, GLUNIFORM1I)
DECLARE_GLFUNCTION (glGenerateMipmap, GLGENERATEMIPMAP)
DECLARE_GLFUNCTION (glActiveTexture, GLACTIVETEXTURE)
// DECLARE_GLFUNCTION (wglCreateContextAttribsARB, WGLCREATECONTEXTATTRIBSARB)
// DECLARE_GLFUNCTION (wglChoosePixelFormatARB, WGLCHOOSEPIXELFORMATARB)

#endif
