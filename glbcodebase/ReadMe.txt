//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon.  All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/01/14
// Breif: Include some important information about the graphics labtory project
// Update: 2017/01/14 File created. All project config information
// Update: 2017/03/19 Add config for dx11
//-----------------------------------------------------------------------------------

/***************************************************************************************
---------------------------------------------------------------------------------------------------------------------------
                                    OpenGL Version
---------------------------------------------------------------------------------------------------------------------------

Project Config

Common Setting:
Output Directory: $(SolutionDir)$(ProjectName)\
Target File Name: 
$(ProjectName)_DGL (For debug version)
$(ProjectName)_RGL (For release version)

C/C++ Setting:
Common->Addtional Include Directory:
$(SolutionDir)\glb
$(SolutionDir)\..\glew\include
$(DXSDK_DIR)\Include
输出文件->对象文件名:$(IntDir)/%(RelativeDir)/

Preprocessor->Preprocessor:
GLB_PLATFORM_OPENGL (For opengl version graphics labtory)

Linker Setting:
Common->Addtional Library:
$(SolutionDir)\..\glew\lib
$(DXSDK_DIR)\Lib\x86
Input->Addtional:
glew32.lib
opengl32.lib
dinput8.lib
dxguid.lib
***************************************************************************************/

/***************************************************************************************
---------------------------------------------------------------------------------------------------------------------------
	                                    DX11 Version
---------------------------------------------------------------------------------------------------------------------------

Project Config

Common Setting:
Output Directory: $(SolutionDir)$(ProjectName)\
Target File Name: 
$(ProjectName)_DDX (For debug version)
$(ProjectName)_RDX (For release version)

C/C++ Setting:
Common->Addtional Include Directory:
$(SolutionDir)\glb
$(DXSDK_DIR)\Include

Preprocessor->Preprocessor:
GLB_PLATFORM_DX11 (For directx11 version graphics labtory)

Linker Setting:
Common->Addtional Library:
$(DXSDK_DIR)\Lib\x86
Input->Addtional:
d3d11.lib
d3dx11d.lib (For debug version)
d3dx11.lib (For release version)
d3dcompiler.lib
dxerr.lib
dxgi.lib
***************************************************************************************/