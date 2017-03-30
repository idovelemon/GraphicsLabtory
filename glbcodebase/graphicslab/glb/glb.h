//-----------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 05 / 31
// Brief: Define some method to help building labtory
//-----------------------------------------------------------------------------------------
#ifndef GLB_GLB_H_
#define GLB_GLB_H_

#ifndef GLB_PLATFORM
#define GLB_PLATFORM (0)
#endif

#ifdef GLB_PLATFORM_OPENGL
#undef GLB_PLATFORM
#define GLB_PLATFORM (1)
#endif

#ifdef GLB_PLATFORM_DX11
#undef GLB_PLATFORM
#define GLB_PLATFORM (2)
#endif

#if !GLB_PLATFORM
#error Must define one GLB_PLATFORM_XXX
#endif

#include "comm.h"
#include "glbapplication.h"
#include "glbcamera.h"
#include "glbeditorcomm.h"
#include "glblight.h"
#include "glblog.h"
#include "glbmacro.h"
#include "glbmaterial.h"
#include "glbmatrix.h"
#include "glbmesh.h"
#include "glbmodelfile.h"
#include "glbobject.h"
#include "glbprofile.h"
#include "glbrender.h"
#include "glbrendertarget.h"
#include "glbscene.h"
#include "glbshader.h"
#include "glbtexture.h"
#include "glbtexturereader.h"
#include "glbvector.h"

#endif  // GLB_GLB_H_
