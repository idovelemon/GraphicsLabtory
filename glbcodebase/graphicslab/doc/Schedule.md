# Development Schedule of GraphicsLabtory

## GLB Library
GLB(GraphicsLabtory) library is a light-weight 3D rendering library.

<br>

|Task|Begin Time|Fnish Time|Status|
|-|-|-|-|
|Resource System problem:When destroy an object, how to deal with the mesh and texture resource associated with it? |2018-06-09 10:10||<font color="Blue">New</font>|
|Model Editor|2018-06-10 22:40|2018-07-01 17:25|<font color="Green">Finish</font>|
|Rename build-in attribute and uniform with glb_attr_xxx and glb_uni_xxx|2018-07-01 17:25|2019-01-01|<font color="Green">Finish</font>|
|Using fixed layout (location = n) for uber shader|2018-07-01 17:25|2019-01-01|<font color="Green">Finish</font>|
|Font rendering|2018-07-01 17:25|2019-01-01|<font color="Green">Finish</font>|
|Build-in debug menu|2018-07-01 17:25|2019-01-01|<font color="Green">Finish</font>|
|How to merge same material with same shader|2019-03-15 21:47||<font color="Blue">New</font>|
|One material instance can only be used in one object, need a way to avoid multiple objects using same material instance|2019-04-10 07:42||<font color="Blue">New</font>|

|Bug|Begin Time|Fnish Time|Status|
|-|-|-|-|
|Missing shader type when create shader|2018-06-11 15:20|2018-06-07-01|<font color="Green">Finish</font>|
|Calculate boundbox with rotation|2018-07-01 17:25||<font color="Blue">New</font>|
|Remove debug menu item|2019-03-14 22:18||<font color="Blue">New</font>|
|Fix mesh do not save resource file name's problem|2019-04-09 8:33||<font color="Blue">New</font>|
|Add alpha dithering shader code|2019-04-10 22:53||<font color="Blue">New</font>|

## GLB LightStudio
Light studio is used for baking Lightmap with Radiosity Normal Mapping(RNM) and precompute LightProbe for image based lighting

<br>

|Task|Begin Time|Fnish Time|Status|
|-|-|-|-|
|Config GLB LightStudio Project|2018-06-07 19:30|2018-06-07 19:33|<font color="Green">Finish</font>|
|Design LightStudio Basic Interface|2018-06-07 19:33|2018-06-07 19:51|<font color="Green">Finish</font>|
|Basic GLB view display|2018-06-07 19:51|2018-06-07 20:35|<font color="Green">Finish</font>|
|Using xml as GLB LightStudio project file|2018-06-07 20:35|2018-06-07 22:45|<font color="Green">Finish</font>|
|Add scene mesh|2018-06-07 20:35|2018-06-08 8:40|<font color="Green">Finish</font>|
|Add scene mesh config interface|2018-06-08 8:40|2018-06-08 12:05|<font color="Green">Finish</font>|
|Add baking interface|2018-06-08 12:05|2018-06-08 12:20|<font color="Green">Finish</font>|
|Add baking function|2018-06-08 12:20|2018-06-08 15:10|<font color="Green">Finish</font>|
|Add light source mesh|2018-06-08 15:10|2018-06-08 15:48|<font color="Green">Finish</font>|
|Add light source mesh config interface|2018-06-08 15:50|2018-06-08 22:40|<font color="Green">Finish</font>|
|Click outline to display matching config interface|2018-06-08 22:40|2018-06-08 23:00|<font color="Green">Finish</font>|
|Add lighting for baking function|2018-06-08 23:00|2018-06-09 00:30|<font color="Green">Finish</font>|
|Organize xml project file|2018-06-09 00:30|2018-06-09 13:10|<font color="Green">Finish</font>|
|Load xml project file|2018-06-09 00:30|2018-06-09 13:10|<font color="Green">Finish</font>|
|Save baking light map to files|2018-06-09 00:30|2018-06-09 14:30|<font color="Green">Finish</font>|
|Hide lights|2018-06-09 10:10||<font color="Blue">New</font>|
|Accelerate key|2018-06-09 15:50||<font color="Blue">New</font>|
|Light map preview|2018-06-09 15:50||<font color="Blue">New</font>|
|Light probe baking|2018-06-09 15:50||<font color="Blue">New</font>|
|Release a beta version|2018-06-09 16:00|2018-06-09 16:15|<font color="Green">Finish</font>|
|Sperate lightmap uv from albedo/roughness/metallic/normal uv|2018-06-09 20:37|2018-06-14 21:35|<font color="Green">Finish</font>|
|Add support for Directional light|2018-06-19 20:00|2018-06-20 8:10|<font color="Green">Finish</font>|

<br>

|Bug|Begin Time|Fnish Time|Status|
|-|-|-|-|
|Add invalid scene mesh problem|2018-06-08 15:45|2018-06-09 14:40|<font color="Green">Finish</font>|
|Add light mesh first problem|2018-06-08 15:45|2018-06-09 14:45|<font color="Green">Finish</font>|
|Fix xml project information record|2018-06-08 15:50|2018-06-09 14:45|<font color="Green">Finish</font>|
|Initialize camera position problem|2018-06-08 22:15|2018-06-09 14:55|<font color="Green">Finish</font>|
|Using Color select dialog and intensity for light color|2018-06-08 22:35||<font color="Blue">New</font>|
|Press tab to config data problem|2018-06-08 22:40|2018-06-09 14:50|<font color="Green">Finish</font>|
|Move mouse to accelerate rendering speed problem|2018-06-09 0:15|2018-06-09 15:50|<font color="Green">Finish</font>|
|Click empty space in outline when create new project, crash|2018-06-09 10:10|2018-06-09 14:35|<font color="Green">Finish</font>|
|Disable all config interface and menu when baking|2018-06-09 13:05|2018-6-20 08:20|<font color="Green">Finish</font>|
|Save project when close program|2018-06-09 14:10|2018-06-09 15:00|<font color="Green">Finish</font>|
|Step1:New -> Step2:Open undefine behaivor|2018-06-09 14:50|2018-06-09 0:15|<font color="Green">Finish</font>|
|Overhead test|2018-06-09 15:55||<font color="Blue">New</font>|
|Memory leak test|2018-06-09 15:55||<font color="Blue">New</font>|
|Disable Add->Sun if there is no scene added|2018-6-19 23:15|2018-6-20 08:20|<font color="Green">Finish</font>|

## GLB ModelEditor
GLB ModelEditor is used for convert DCC tool's export file format to GLB library build-in file formats
<li> Convert mesh data
<li> Edit material
<li> Model preview

|Task|Begin Time|Fnish Time|Status|
|-|-|-|-|
|Setup GLB ModelEditor project|2018-06-10 23:20|2018-06-11 06:35|<font color="Green">Finish</font>|
|Basic GUI|2018-06-11 06:35|2018-06-11 07:20|<font color="Green">Finish</font>|
|Basic interactive|2018-06-11 07:20|2018-06-11 09:45|<font color="Green">Finish</font>|
|Download assimp library|2018-06-10 23:20|2018-06-11 10:35|<font color="Green">Finish</font>|
|Add import model function|2018-06-11 10:35|2018-06-11 10:40|<font color="Green">Finish</font>|
|Display view after import model|2018-06-11 10:35|2018-06-11 11:00|<font color="Green">Finish</font>|
|Display model after import model|2018-06-11 11:00|2018-06-11 15:25|<font color="Green">Finish</font>|
|Add texture dynamic|2018-06-11 15:25|2018-06-11 16:40|<font color="Green">Finish</font>|
|Export model as glb build-in mesh file|2018-06-11 16:40|2018-06-11 22:30|<font color="Green">Finish</font>|
|Add preview menu|2018-06-11 21:15|2018-06-11 22:30|<font color="Green">Finish</font>|
|Change modelEditor's icon|2018-06-11 22:00|2018-06-12 9:55|<font color="Green">Finish</font>|
|Support for second uv|2018-06-11 22:35|2018-06-11 23:05|<font color="Green">Finish</font>|
|Support for alpha texture|2018-06-11 22:35|2018-06-12 10:35|<font color="Green">Finish</font>|
|Support for emission texture|2018-06-11 22:35|2018-06-12 11:05|<font color="Green">Finish</font>|
|Support for diffuse and specular environment texture|2018-06-11 22:35||<font color="Red">Discard</font>|
|Support for light map texture|2018-06-11 22:35|2018-06-15 07:50|<font color="Green">Finish</font>|
|Copy texture when export model|2018-06-11 23:10|2018-06-12 11:15|<font color="Green">Finish</font>|
|Clear texture in editor mode|2018-06-12 12:35||<font color="Red">Discard</font>|
|Edit emission intensity at editor mode|2018-06-21 20:00||<font color="Red">Discard</font>|
|Support for reopen material file and edit it again|2019-03-10 16:22|2019-04-21 21:37|<font color="Green">Finish</font>|
|Support for edit general post-processing parameter|2019-03-10 16:22||<font color="Blue">New</font>|
|Support for slide edit control for material parameters|2019-03-14 22:21|2019-04-21 23:11|<font color="Green">Finish</font>|
|Support add exist shader when adding pass material|2019-04-10 21:33|2019-04-10 22:23|<font color="Green">Finish</font>|
|Support add default shadow pass material, just because most of the object has the same shadow pass material|2019-04-18 08:26||<font color="Blue">New</font>|
|Add a plane, so that we can see shadow effect of object|2019-04-23 08:36||<font color="Blue">New</font>|
|Optimize camera effect|2019-04-23 08:36||<font color="Blue">New</font>|

|Bug|Begin Time|Fnish Time|Status|
|-|-|-|-|
|Menu turn gray after click it|2018-06-11 10:40|2018-06-11 16:50|<font color="Green">Finish</font>|
|Remove cursor in editor control|2018-06-11 11:10|2018-06-12 11:55|<font color="Green">Finish</font>|
|Disabled preview after import a model|2018-06-11 22:30|2018-06-12 11:55|<font color="Green">Finish</font>|
|Do not enable texture file browser button after preview a model|2018-06-12 11:05|2018-06-12 11:55|<font color="Green">Finish</font>|
|Display default environment cube map|2018-06-12 11:15|2018-06-12 12:20|<font color="Green">Finish</font>|
|Import a model again : GLB init failed|2018-06-20 8:00||<font color="Red">Discard</font>|
|Check if has texcoord when set albedo/roughness/metallic/normal/alpha/emission texture|2018-06-23 18:15||<font color="Red">Discard</font>|
|Check only has emission material|2018-08-19 17:35||<font color="Red">Discard</font>|
|Fix space key closing window problem|2019-04-10 22:03|2019-04-23 08:35|<font color="Green">Finish</font>|
|Hasn't copy shader file when save material|2019-04-10 23:00|2019-04-23 08：20|<font color="Green">Finish</font>|