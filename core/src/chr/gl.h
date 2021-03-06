#pragma once

#include "glm.h"
#include "chr/Platform.h"

#if defined(CHR_PLATFORM_GLFW)
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
#elif defined(CHR_PLATFORM_WIN)
  #include "glload/gl_2_0.h"
  #include "glload/gl_load.h"
#elif defined(CHR_PLATFORM_EMSCRIPTEN)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <html5.h>
#elif defined(CHR_PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <OpenGLES/ES2/glext.h>
#elif defined(CHR_PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
#endif

#if defined (CHR_PLATFORM_DESKTOP)
  #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
  #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
