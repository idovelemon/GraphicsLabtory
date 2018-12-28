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

#include "glbapplication.h"
#include "glbdebugmenu.h"
#include "glbinput.h"

#include "math/glbmath.h"

#include "scene/glbcamera.h"
#include "scene/glblight.h"
#include "scene/glbobject.h"
#include "scene/glbscene.h"

#include "render/glbfont.h"
#include "render/glbmaterial.h"
#include "render/glbmesh.h"
#include "render/glbrender.h"
#include "render/glbrenderdevice.h"
#include "render/glbrendertarget.h"
#include "render/glbshader.h"
#include "render/glbtexture.h"

#include "util/glblog.h"
#include "util/glbmacro.h"
#include "util/glbmeshreader.h"
#include "util/glbprofile.h"
#include "util/glbutil.h"

#endif  // GLB_GLB_H_
