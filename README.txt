Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Skybox Tutorial!
Prerequesites: Intro Graphics, Materials

This tutorial uses MSAA (anti-aliasing). 
No knowledge of MSAA is needed to learn about skyboxes. 
If you are interested in MSAA, feel free to see our 
tutorial for it. Only takes two lines to enable

This tutorial implements a new type of texture: Cubemap
Previously, we've had 2D textures that have (x, y) UV coordinates.
Each UV coordinate corresponds to an (x, y) position on the texture.

Cube maps are different. One cubemap is a collection of six 2D Textures,
each texture is a different side of the cube. Cubemap UV coordinates
are 3D (x, y, z). For cubemaps, UV does not coorespond to an (x, y, z) position,
but instead cooresponds to an (x, y, z) direction.

Allow me to explain in more detail:
To get a pixel from a 2D texture in GLSL, we use the command:
	color = texture(diffuseTex, vec2(0.1, 0.2)
This would give us a pixel that is 10% from the left side of the image,
and 20% from the top of the image
To get a pixel from a 3D cubemap in GLSL, we use commands like this:
Get the pixel in the center of the left-side of the cube:
	color = texture(cube, texture(-1, 0, 0))
Get the pixel in the center of the top of the cube:
	color = texture(cube, texture(0, 1, 0))
Get the pixel in the center of the front-side of the cube:
	color = texture(cube, texture(0, 0, 1))
	
==show code for this===
When we draw our skybox, we need the skybox to have its own material
(shaders, textures), that are sepearte from the rest of the objects we want to draw.
In our vertex shader we simply have 
	vec4 p = cameraView * vec4(in_position, 1);
We don't need a model matrix because the skybox won't be moving in relation to the camera
We then finalize position as
	gl_Position = p.xyww;
This is so the position is {x, y, w, w }, rather than {x, y, z, w }
That makes the depth of each pixel, the maximum possible depth, so that
no objects can be drawn behind the skybox, only in front.
We also need the position of each vertex to be passed to the fragment shader in world-space,
so we pass it to the fragment shader
	out vec3 position;
	position = in_position;
The fragment shader gets a pixel from the skybox texture with a 3D UV-coordinate (A UV with an x, y, and z).
The 3D coordinates for each pixel in the fragment shader comes from the direction from the camera
to each pixel
	gl_FragColor = texture(cubeMap, -position);

How to Improve:
Try the Reflections-Flat tutorial, to reflect a skybox on a model
