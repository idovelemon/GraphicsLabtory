//----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 05
// Brief: Define some help macro
//----------------------------------------------------------------------------------------
#ifndef GLB_GLBMACRO_H_
#define GLB_GLBMACRO_H_

#include <assert.h>
#include <stdio.h>

#include <Windows.h>

#include "glblog.h"
#include "glbutil.h"

#define GLB_SAFE_DELETE(_pointer_) \
    if (_pointer_ != nullptr) {\
        delete _pointer_; \
        _pointer_ = nullptr; \
    }

#define GLB_SAFE_DELETE_ARRAY(_pointer_) \
    if (_pointer_ != nullptr) { \
        delete[] _pointer_; \
        _pointer_ = nullptr; \
    }

#define GLB_SAFE_RELEASE(_pointer_) \
    if (_pointer_ != nullptr) { \
        _pointer_->Release(); \
        _pointer_ = nullptr; \
    }

#if _DEBUG
#define GLB_SAFE_ASSERT(_expression_) GLB_SAFE_ASSERT_D(_expression_, "Error")
#define GLB_SAFE_ASSERT_LOG(_expression_, _error_msg_) GLB_SAFE_ASSERT_D(_expression_, _error_msg_)
#else
#define GLB_SAFE_ASSERT(_expression_) GLB_SAFE_ASSERT_R(_expression_, "Error")
#define GLB_SAFE_ASSERT_LOG(_expression_, _error_msg_) GLB_SAFE_ASSERT_R(_expression_, _error_msg_)
#endif

#define GLB_SAFE_ASSERT_D(_expression_, _error_msg_) \
    do {\
        bool result = _expression_; \
        if (result == false) {\
            char buffer[256];\
            sprintf_s(buffer, "%s: %s", __FUNCTION__, _error_msg_);\
            OutputDebugStringA(buffer); \
            glb::util::log::LogPrint(buffer); \
            MessageBoxA(nullptr, buffer, "Assert-Error", MB_OK); \
        } \
        assert(result); \
    }while(false);

// #define GLB_SAFE_ASSERT_D(_expression_) (_expression_)

#define GLB_SAFE_ASSERT_R(_expression_, _error_msg_) \
    do {\
        bool result = _expression_; \
        if (result == false) {\
            char buffer[256];\
            sprintf_s(buffer, "%s: %s", __FUNCTION__, _error_msg_);\
            glb::util::log::LogPrint(buffer); \
        }\
        assert(result); \
    }while(false);

#define GLB_CHECK_GL_ERROR \
    {\
        GLenum err = glGetError();\
        if (err != GL_NO_ERROR) {\
            const GLubyte* err_str = glewGetErrorString(err);\
            char buffer[128];\
            sprintf_s(buffer, "ERR:%d Desc:%s", err, reinterpret_cast<LPCSTR>(const_cast<GLubyte*>(err_str)));\
            OutputDebugStringA(buffer);\
            MessageBoxA(nullptr, buffer, "GL-Error", MB_OKCANCEL);\
        }\
    }

#define GLB_OVERRIDE_COPY_AND_ASSIGN(_class_name_) \
    _class_name_(_class_name_&); \
    const _class_name_& operator=(const _class_name_&);

#define GLB_ARRAY_SIZE(_array_) (sizeof(_array_) / sizeof(_array_[0]))

#define GLB_USER_ERROR_MSG(_error_msg_) \
    do {\
        glb::util::log::LogPrint(_error_msg_); \
        glb::util::user_error_pushmsg(_error_msg_); \
    }while(false);

#endif  // GLB_GLBMACRO_H_
