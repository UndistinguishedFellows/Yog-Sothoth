# Yog-Sothoth v1.0.38
GameEngine
Repository: https://github.com/UndistinguishedFellows/Yog-Sothoth
Releases: https://github.com/UndistinguishedFellows/Yog-Sothoth/releases

# Basic Usage

.exe must be in Yog-Sothoth_Release folder. 

## Importing files

This software ONLY loads .fbx files and extension must be in lower case
	-Those files will be stored in data/assets with their prefabs and resources in library
	-Files can only be imported by draging them and droping into the window.


## Inspector

Inspector displays:
	- Non serializable objects has an informative text that will say they won be saved inside the prefab.
	-CheckBox to display the AABB of the game object if it has one
	-Transform
	-Mesh
		-CheckBox to display Normals
		-CheckBox to display the mesh in wireframe mode
	-Material
		-If model has no texture, no image will display
		-If model has texture, image with texture and its size will be displayed
		-If has texture, texture name will be dispalyed
		-Color, this is an editable propertie
		-Checkers texture as a test texture 
	-Camera
		-Frustum properties
		-Culling
		-Active camera can be changed

## Outliner

- With right click you can open a menu with game object options. Must be uncollapsed.
	- Adding new go
	- Loading prefabs
	- Deleting go
	- Rename 

## Menu

-File
	-Load scene prefab
	-Save scene prefab
	-Quit
-Windows, lets you show or hide the outliner window
-View, lets you show or hide configuration window
	-Configuration
		-Application
			-Shows a basic graph with framerate, miliseconds and memmory usage
			-FPS limiter
		-Window
		-Render
			-Lets you to set up some render properties
			-You can change between editor mode and fbx viewer mode
		-Hardware
-Help
	-Lets you to report an issue 
	-Show a window with some tests
	-Show demo gui window
-About, can show credits, open repository and navigate to releases page

## Console

For now its only to display info, but, in the future will allow some commands to interact with the engine
-Red -> ERROR
-Yellow -> Warning
-Green -> Info
-White -> Basic console interactions
-Grey -> command

## Camera Controls

To interact with camera
	- RIGHT MOUSE CLICK MUST BE ALWAYS PRESSED TO
		- WASD -> to move
		- EQ -> to go up/down
		- mouse movement -> rotate camera
	- mouse wheel -> Zoom
	- alt + left mouse click + mouse movement -> orbit around selected object
	- f -> focus the selected object
	- Arrow keys -> move the camera

## Play Pause
Play saves the scene and stop will undo all changes. Exept non serializable objects.

## Shader Editor
Can load and save shaders inside "data/shaders/"
To reload shaders to apply changes go to component shader and press reload shader button.

## Shaders
All shaders must be placed inside "data/shaders/"
And must be added to "shaderConfig.json"
There is a shader component to select wich shader must use

## Water Shader
Water shader uses 3 textures as height maps 2 caustics and one perlin noise. One to emulate top waves and other one to emulate the shadow of upper ones. The third texture is
perlin noise to put some chaos in waves.
The color chosen tries to emulate a peaceful mass of water (whiter at the top, darker in the bottom). In editor you can edit the 2 main waves to make some more violent ones with some basic parameters.



## Made by
CapitanLiteral
dibu13

