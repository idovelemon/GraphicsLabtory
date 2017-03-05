//----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 05
// Brief: Define some help macro
//----------------------------------------------------------------------------------------
#ifndef GLB_GLBMACRO_H_
#define GLB_GLBMACRO_H_

#include <assert.h>

#include <Windows.h>

#define GLB_SAFE_DELETE(_pointer_) \
    if (_pointer_ != NULL) {\
        delete _pointer_; \
        _pointer_ = NULL; \
    }

#define GLB_SAFE_DELETE_ARRAY(_pointer_) \
    if (_pointer_ != NULL) { \
        delete[] _pointer_; \
        _pointer_ = NULL; \
    }

#if _DEBUG
#define GLB_SAFE_ASSERT(_expression_) GLB_SAFE_ASSERT_D(_expression_)
#else
#define GLB_SAFE_ASSERT(_expression_) GLB_SAFE_ASSERT_R(_expression_)
#endif

#define GLB_SAFE_ASSERT_D(_expression_) \
    do {\
        bool result = _expression_; \
        if (result == false) {\
            OutputDebugStringA(__FUNCTION__); \
            MessageBoxA(NULL, __FUNCTION__, "Assert-Error", MB_OK); \
        } \
        assert(result); \
    }while(false);

#define GLB_SAFE_ASSERT_R(_expression_) \
    do {\
        bool result = _expression_; \
        assert(result); \
    }while(false);

#define GLB_CHECK_GL_ERROR \
    {\
        GLenum err = glGetError();\
        if (err != GL_NO_ERROR) {\
            const GLubyte* err_str = glewGetErrorString(err);\
            char buffer[128];\
            sprintf(buffer, "ERR:%d Desc:%s", err, reinterpret_cast<LPCSTR>(const_cast<GLubyte*>(err_str)));\
            OutputDebugStringA(buffer);\
            MessageBoxA(NULL, buffer, "GL-Error", MB_OK);\
        }\
    }
#endif  // GLB_GLBMACRO_H_
