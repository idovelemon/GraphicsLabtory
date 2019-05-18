echo f |xcopy glb_modeleditor_RGL.exe ..\glb_bin\GLB_ModelEditor\glb_modeleditor.exe /Y
echo f |xcopy assimp-vc110-mt.dll ..\glb_bin\GLB_ModelEditor\assimp-vc110-mt.dll /Y
echo f |xcopy glew32.dll ..\glb_bin\GLB_ModelEditor\glew32.dll /Y
echo f |xcopy GLB_SRP.py ..\glb_bin\GLB_ModelEditor\GLB_SRP.py /Y
echo d |xcopy res ..\glb_bin\GLB_ModelEditor\res /Y
echo d |xcopy ..\glb\resource ..\glb_bin\glb\resource /Y
echo d |xcopy ..\glb\shader ..\glb_bin\glb\shader /Y