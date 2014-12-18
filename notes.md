# Notes

http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction


Vertex shader + Fragment shader --> GLSL program


GL_STATIC_DRAW indicates that we will not write to this buffer often, and that the GPU should keep a copy of it in its own memory. It is always possible to write new values to the VBO. If the data changes once per frame or more often, you could use GL_DYNAMIC_DRAW or GL_STREAM_DRAW.





