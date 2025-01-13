/* __vimdothis__
   let b:dispatch = 'make'
   packadd a.vim
   packadd vim-fugitive
   set foldmethod=marker
   __vimendthis__ */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>
#include <Windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <stdbool.h>

#include "gl_funcs.h"
#include "util.h"
#include "stb_image.h"
#include "HandmadeMath.h"
#include "cglm/struct.h"

#define local_persist static
#define global_variable static
#define internal static

void 
InitOpenGLExtensions(void)
{/* {{{ */

}/* }}} */

void
InitOpenGL (HWND Window, HDC DeviceContext)
{/* {{{ */
    PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
    PIXELFORMATDESCRIPTOR DesiredPixelFormat = {
        .nSize = sizeof (DesiredPixelFormat),
        .nVersion = 1,
        .dwFlags = PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_DRAW_TO_WINDOW,
        .cColorBits = 32, // Why 32 bits for 3 colors? Does it include the alpha channel as well as opposed to the docs?
        .cAlphaBits = 8
    };

    /* DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE; */

    int SuggestedPixelFormatIndex = ChoosePixelFormat (DeviceContext, &DesiredPixelFormat);
    DescribePixelFormat (DeviceContext, SuggestedPixelFormatIndex, sizeof (SuggestedPixelFormat), &SuggestedPixelFormat);
    SetPixelFormat (DeviceContext, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

    // Create a 'fake' OpenGL context to load wglCreateContextAttribsARB and wglChoosePixelFormatARB functions
    // Then use them to properly request for a core version.
    HGLRC LegacyOpenGLRC = wglCreateContext (DeviceContext);
    wglMakeCurrent(DeviceContext, LegacyOpenGLRC);


    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) LoadGLFunction("wglCreateContextAttribsARB");
    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) LoadGLFunction("wglChoosePixelFormatARB");

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_FLAGS_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB, // Tell Windows we want the core profile
        0 
    };
    printf("Creating context...\n");
    HGLRC OpenGLRC = wglCreateContextAttribsARB (DeviceContext, NULL, attribs);

    if (!wglMakeCurrent (DeviceContext, OpenGLRC))
    {
        printf ("Failed to create context!");
        exit (1);
    }
    else
    {
        wglDeleteContext(LegacyOpenGLRC);
        LoadGLFunctionPointers ();
        printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
        /* MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0); */
    }
}/* }}} */

LRESULT
MainWindowCallback (HWND Window,
                    UINT Message,
                    WPARAM wParam,
                    LPARAM lParam)
{/* {{{ */
    LRESULT Result = 0;
    switch(Message)
    {
        case WM_CREATE:
            {
            } break;
        case WM_SIZE:
            {
                RECT rect;
                GetWindowRect (Window, &rect);
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                glViewport (0, 0, width, height);
            }	break;

        case WM_DESTROY:
            {
                OutputDebugString("WM_DESTROY\n");
            } break;

        case WM_MOVE:
            {
                OutputDebugString("WM_MOVE\n");
            } break;

        case WM_CLOSE:
            {
                OutputDebugString("WM_CLOSE\n");
                exit(0);
            } break;

        case WM_ACTIVATEAPP:
            {
                OutputDebugString("WM_ACTIVATEAPP\n");
            } break;

        default:
            {
                /* OutputDebugString("default\n"); */
                Result = DefWindowProc(Window, Message, wParam, lParam);
            } break;
    }

    return Result;
}/* }}} */


inline void
OpenGLPleaseDraw (HDC DeviceContext)
{/* {{{ */
    glViewport (0, 0, 400, 400);
    glClearColor (0.0f,0.0f,0.0f,0.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    SwapBuffers (DeviceContext);
}/* }}} */


int WINAPI
WinMain (HINSTANCE Instance, // Windows-provided instance of the program
        HINSTANCE prevInstance, // NULL on newer Windows versions
        LPSTR argCount, // Number of command-line arguments
        int windowType) // How to open the window - minimised, maximised or ...
{ /* {{{ */
    WNDCLASS WindowClass = {
        .style = CS_HREDRAW|CS_VREDRAW,
        .lpfnWndProc = MainWindowCallback,
        .hInstance = Instance,
        .lpszClassName = "MyFirstWindow"
    };

    if(RegisterClass (&WindowClass))
    {
        HWND WindowHandle = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW|WS_EX_WINDOWEDGE,
                WindowClass.lpszClassName,
                "Learning OpenGL",
                WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                0,
                0,
                Instance,
                0);

        if(WindowHandle)
        {
            HDC DeviceContext = GetDC (WindowHandle);

            InitOpenGL (WindowHandle, DeviceContext);
            glViewport (0, 0, 800, 800);

            // Loading image
            stbi_set_flip_vertically_on_load(1);
            Image container_image;
            _LoadImage ("res/container.jpg", &container_image);

            Image face_image;
            _LoadImage ("res/awesomeface.png", &face_image);

            if (!container_image.data || !face_image.data)
            {
                printf ("Failed to load image\n");
                const char* fail = stbi_failure_reason(); 
                printf("Reason:%s\n",fail);
                exit (1);
            }
            unsigned int shaderProgram = CreateShaderProgram ("res/vert_shader.glsl", "res/frag_shader.glsl");

            float vertices[] = {
                // positions          // colors           // texture coords
                0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
                0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
            };

            unsigned int indices [] = {
                0, 1, 3, // First triangle
                1, 2, 3  // Second triangle
            };

            float texCoords[] = {
                0.0f, 0.0f, // lower left
                1.0f, 0.0f, // lower right
                0.5f, 1.0f  // top center
            };

            float borderColor[] = {1.0, 1.0f, 0.0f, 1.0f};

            vec3s vec1 = (vec3s){0.0f, 0.0f, -1.0f}, vec2 = {0.5f, 0.5f, 0.5f};
            mat4s m1 = glms_mat4_identity();
            m1 = glms_rotate(m1, GLM_PI, vec1);
            m1 = glms_scale(m1, vec2);
            /*m1 = glms_scale(m1, vec1);*/

            unsigned int VAO, VBO, EBO;
            glGenVertexArrays (1,&VAO);
            glGenBuffers (1, &VBO);
            glGenBuffers (1, &EBO);

            glBindVertexArray (VAO);

            glBindBuffer (GL_ARRAY_BUFFER, VBO); 
            glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW); 

            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void *)0);
            glEnableVertexAttribArray (0);

            // color attribute
            glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void *)(3 * sizeof (float)));
            glEnableVertexAttribArray (1);

            // vertex attribute
            glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float),  (void *)(6 * sizeof (float)));
            glEnableVertexAttribArray (2);

            // Loading texture
            glTexParameterfv (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            unsigned int texture1, texture2;
            glGenTextures (1, &texture1);
            glGenTextures (1, &texture2);
            glBindTexture (GL_TEXTURE_2D, texture1);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, container_image.width, container_image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, container_image.data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture (GL_TEXTURE_2D, texture2);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face_image.width, face_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, face_image.data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free (container_image.data);
            stbi_image_free (face_image.data);

            glUseProgram (shaderProgram);
            glUniform1i(glGetUniformLocation(shaderProgram, "texture1"),0);
            glUniform1i(glGetUniformLocation(shaderProgram, "texture2"),1);
            /* glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); */

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, trans.cont_members);

            float i = 0;
            while (1)
            {
                MSG Message;
                bool MessageResult = GetMessage (&Message, 0, 0, 0);

                if (MessageResult > 0)
                {
                    TranslateMessage (&Message);
                    DispatchMessage (&Message);

                    //glViewport (0, 0, 400, 400);
                    glClearColor (0.0f,0.0f,0.0f,0.0f);
                    glClear (GL_COLOR_BUFFER_BIT);

                    glActiveTexture (GL_TEXTURE0);
                    glBindTexture (GL_TEXTURE_2D, texture1);

                    glActiveTexture (GL_TEXTURE1);
                    glBindTexture (GL_TEXTURE_2D, texture2);

                    printf("i: %f\n", i);
                    vec.y =  i;
                    trans = Mat4Translate(&vec);
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, trans.cont_members);

                    /*         for (int i = 0; i < 4; i++) */
                    /*         { */
                    /*             printf("\n"); */
                    /*             for (int j = 0; j < 4; j++) */
                    /*                 printf("%f ", trans.members[i][j]); */
                    /*         }; */

                    glUseProgram (shaderProgram);
                    glBindVertexArray (VAO);
                    glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    SwapBuffers (DeviceContext);
                    i = i + 0.1;
                }

                else
                    break;
            }

            ReleaseDC (WindowHandle, DeviceContext);
        }

        else
        {
            // TODO: Logging
        }
    }

    else
    {
        // TODO: Logging
    }

    return 0;
}/* }}} */
