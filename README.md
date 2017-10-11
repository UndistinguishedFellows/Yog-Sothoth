# Yog-Sothoth
GameEngine

# Basic Usage

## Importing files

This software ONLY loads .fbx files
	-Those files must be stored in data/assets
	-The loaded files are showed in the inspector window, inside "/"
	-When geometry is loaded the camera will adapt to its size (NOT DONE)

## Inspector

Inspector displays:
	-Transform, wich is not being drawed, only displayed
	-Mesh
	-Material
		-If model has no texture, no image will display
		-If model has texture, image with texture and its size will be displayed
		-Color, this is an editable propertie
		-Checkers texture as a text texture (NOT DONE)

## Outliner

Lets you to navigate between loaded meshes

## Menu

-File, has for now the option to quit alone
-Windows, lets you show or hide the outliner window
-View, lets you show or hide configuration window
	-Application
		-Shows a basic graph with framerate, miliseconds and memmory usage
		-FPS limiter (NOT DONE)
	-Window
	-Render
	-Hardware
-Help, lets you to report an issue and show a test window
-About, can show credits, open repository and navigate to releases page

## Console

Has very basic commands, for now its only to display info, but, in the future will allow some commands to interact with the engine
-Red -> ERROR
-Yellow -> Warning
-Green -> Info
-White -> Basic console interactions
-Grey -> command

## Camera Controls

To interact with camera
	- RIGHT CLICK MUST BE ALWAYS PRESSED
	- wasd -> to move
	- rf -> to go up/down
	- space -> focus the selected object (NOT DONE)
	- mouse movement -> rotate camera
	- alt + mouse movement -> orbit around selected object (NOT DONE)
	- mouse wheel -> Zoom (NOT WORKING)





