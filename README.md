------------------
-- JoJo Engine --
------------------

Authors: Josep Atencia and Jordi Nunes

Repository: https://github.com/joeyGumer/JoJoEngine

License: https://github.com/joeyGumer/JoJoEngine/blob/master/LICENSE.txt

HOW TO USE IT
----------------------------

Geometry Loading
----------------------------
Process to load any geometry:
- Drop the fbx file into the engine window.
- Automatically the model will be loaded with its texture (camera will focus on the geometry loaded).
- You can modify its texture dragging another png file into the window.
- In the Window->Console, you can check all the loading process

At this moment the 3D drawing process works without shaders, through vertices and indices.

Editor movement
------------------------------
- Move the camera with AWSD.
- Zoom in/out with the mouse wheel.
- Left Alt + Left mouse orbit the camera.
- Press F to focus camera around geometry.

Menu Options
------------------------------
File:
- Exit: Close the engine.

Window:
- Configuration: Editor window that allows you to modify a lot of parameters about the engine:
	- Application: app and organization names.
	- Window: size of window, vsync, fullscreen or not, etc.
	- Render: modify parameters about OpenGL render process.
	- Camera: shows camera position and you can modify speed 				parameters.
	- Input: shows mouse position and how many joysticks are 				connected.
	- Hardware: shows info about CPU, RAM, caps, GPU and VRAM
- Console: Displays the console.
- Properties: Displays a window with information about the FBX model loaded (transformations, triangles and texture size).
	
Help:
- Demo: Displays ImGui demo window.
- Download latest version: Link to the github release site of the project.
- Report a bug: Link to the github issues site of the project.
	
About Us:
- Information about the project, authors and libraries used.
