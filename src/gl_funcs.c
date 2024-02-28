#include "gl_funcs.h"
#include "util.h"
 
#define LOAD_GLFUNCTION(func_name,token_name) PFN##token_name##PROC func_name = (PFN##token_name##PROC) LoadGLFunction (#func_name);

inline void 
LoadGLFunctionPointers (void)
{
    LOAD_GLFUNCTION (glUseProgram, GLUSEPROGRAM)
    LOAD_GLFUNCTION (glCreateProgram, GLCREATEPROGRAM)
    LOAD_GLFUNCTION (glDeleteShader, GLDELETESHADER)
    LOAD_GLFUNCTION (glLinkProgram, GLLINKPROGRAM)
    LOAD_GLFUNCTION (glShaderSource, GLSHADERSOURCE)
    LOAD_GLFUNCTION (glAttachShader, GLATTACHSHADER)
    LOAD_GLFUNCTION (glGenBuffers, GLGENBUFFERS)
    LOAD_GLFUNCTION (glBindBuffer, GLBINDBUFFER)
    LOAD_GLFUNCTION (glBufferData, GLBUFFERDATA)
    LOAD_GLFUNCTION (glCreateShader, GLCREATESHADER)
    LOAD_GLFUNCTION (glCompileShader, GLCOMPILESHADER)
    LOAD_GLFUNCTION (glVertexAttribPointer, GLVERTEXATTRIBPOINTER)
    LOAD_GLFUNCTION (glEnableVertexAttribArray, GLENABLEVERTEXATTRIBARRAY)
    LOAD_GLFUNCTION (glGenVertexArrays, GLGENVERTEXARRAYS)
    LOAD_GLFUNCTION (glBindVertexArray, GLBINDVERTEXARRAY)
    LOAD_GLFUNCTION (glGetShaderiv, GLGETSHADERIV)
    LOAD_GLFUNCTION (glGetShaderInfoLog, GLGETSHADERINFOLOG)
}
