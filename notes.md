# Notes

http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction


Vertex shader + Fragment shader --> GLSL program


GL_STATIC_DRAW indicates that we will not write to this buffer often, and that the GPU should keep a copy of it in its own memory. It is always possible to write new values to the VBO. If the data changes once per frame or more often, you could use GL_DYNAMIC_DRAW or GL_STREAM_DRAW.



## Attempt at explaining the render loop when using VBO:s

1. Clear
2. Set GLSL program
3. Enable correct vertex attribute array 
4. Bind buffer (upload data from my array to opengl array)
5. Set vertext attribute pointers (set attributes in shader programs)
6. Draw!
7. Swap buffers
8. Poll events



A 'varying' variable in a shader has to exist in both the vertex- and the fragment shader.


Attributes can change for the various vertices
Uniforms are the same for all vertices in the vbo



Matrices affect the object from right to left.
A matrix is 16 floats.

Rotate stuff like this:
       float rotmat[] = {cosf( angle ), -sinf( angle ), 0.0, 0.0,
		      	 sinf( angle ),  cosf( angle ), 0.0, 0.0,
		         0.0,           0.0,            1.0, 0.0,
		         0.0,           0.0,            0.0, 1.0};

If the matrix is the same for all vertices it's better to calculate it in
the C code and set it for the whole shader.

Use the matrix in the shader like this:

    gl_Position = m_transform * vec4(coord3d, 1.0);



