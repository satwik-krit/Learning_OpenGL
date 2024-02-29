#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>
#include <Windows.h>
#include <stdio.h>
#include "gl_funcs.h"

#include "util.h"

#define local_persist static
#define global_variable static
#define internal static

void
InitOpenGL (HWND Window, HDC DeviceContext)
{
  PIXELFORMATDESCRIPTOR DesiredPixelFormat = {0};
  PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
  DesiredPixelFormat.nSize = sizeof (DesiredPixelFormat);
  DesiredPixelFormat.nVersion = 1;
  DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_DRAW_TO_WINDOW;
  DesiredPixelFormat.cColorBits = 32; // Why 32 bits for 3 colors? Does it include the alpha channel as well as opposed to the docs?
  DesiredPixelFormat.cAlphaBits = 8;
  /* DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE; */

  int SuggestedPixelFormatIndex = ChoosePixelFormat (DeviceContext, &DesiredPixelFormat);
  DescribePixelFormat (DeviceContext, SuggestedPixelFormatIndex, sizeof (SuggestedPixelFormat), &SuggestedPixelFormat);
  SetPixelFormat (DeviceContext, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

  HGLRC OpenGLRC = wglCreateContext (DeviceContext);

  if (!wglMakeCurrent (DeviceContext, OpenGLRC))
  {
	printf ("Failed to create context!");
    exit (0);
  }
}

LRESULT
MainWindowCallback (HWND Window,
                    UINT Message,
                    WPARAM wParam,
                    LPARAM lParam)
{
  LRESULT Result = 0;
  switch(Message)
  {
    case WM_CREATE:
    {
    } break;
    case WM_SIZE:
    {
	OutputDebugString("WM_SIZE\n");
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
}


inline void
OpenGLPleaseDraw (HDC DeviceContext)
{
  glViewport (0, 0, 800, 800);
  glClearColor (0.0f,1.0f,1.0f,1.0f);
  glClear (GL_COLOR_BUFFER_BIT);
  SwapBuffers (DeviceContext);
}


int WINAPI
WinMain (HINSTANCE Instance, // Windows-provided instance of the program
	     HINSTANCE prevInstance, // NULL on newer Windows versions
         LPSTR argCount, // Number of command-line arguments
         int windowType) // How to open the window - minimised, maximised or ...
{
  WNDCLASS WindowClass = {0};
  WindowClass.style = CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc = MainWindowCallback;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = "MyFirstWindow";

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

	  float vertices [] = {
	    -0.5f, -0.5f, 0.0f,
	    0.5f, -0.5f, 0.0f,
	    0.0f, 0.5f, 0.0f
	  };

	  unsigned int VBO;
	  glGenBuffers (1, &VBO);
	  glBindBuffer (GL_ARRAY_BUFFER, VBO); 
	  glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW); 

	  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

	  unsigned int vertexShader;
	  vertexShader = glCreateShader (GL_VERTEX_SHADER);
	  // TODO: Segfault at runtime
	  glShaderSource (vertexShader, 1, &vertexShaderSource,NULL );
	  glCompileShader (vertexShader);

	  const char* fragmentShaderSource = "#version 330 core\n"
										 "out vec4 FragColor;\n"
											"void main()\n"
										"{\n"
										"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
										"}\0";

		unsigned int fragmentShader;
		fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader (fragmentShader);

		unsigned int shaderProgram;
		shaderProgram = glCreateProgram ();
		glAttachShader (shaderProgram, vertexShader);
		glAttachShader (shaderProgram, fragmentShader);
		glLinkProgram (shaderProgram);

		glDeleteShader (vertexShader);
		glDeleteShader (fragmentShader);

		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *)0);
		glEnableVertexAttribArray (0);

		unsigned int VAO;
		glGenVertexArrays(1,&VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *)0);
		glEnableVertexAttribArray (0);
		
		glUseProgram (shaderProgram);

		GLenum error = glGetError();
        

	  while (1)
	  {
	     MSG Message;
	     BOOL MessageResult = GetMessage (&Message, 0, 0, 0);

	     if (MessageResult > 0)
	     {
	      TranslateMessage (&Message);
	      DispatchMessage (&Message);

	      glClearColor (0.0f,1.0f,1.0f,1.0f);
	      glClear (GL_COLOR_BUFFER_BIT);

		  glBindVertexArray (VAO);
		  glDrawArrays (GL_TRIANGLES, 0, 3);

	      SwapBuffers (DeviceContext);
	     }

	     else
	     {
	      break;
	     }

	  }
      }

      else
	{
  // TODO: Logging
	}}

  else
  {
    // TODO: Logging
  }

  return 0;
}
