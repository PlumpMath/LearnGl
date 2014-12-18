#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint program;
GLint attribute_coord2d;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
      return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "ERIK IS LEARNING", NULL, NULL);
    if (!window)
    {
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);

    /* Must initialize glew before starting to use open gl functions (or it will segfault) */
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
      return EXIT_FAILURE;
    }

    GLint compile_ok = GL_FALSE;

    /* Vertex shader */
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    
    const char *vs_source = 
      "#version 120                           \n"
      "attribute vec2 coord2d;                  "
      "void main(void) {                        "
      "  gl_Position = vec4(coord2d, 0.0, 1.0); "
      "}                                        ";

    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
    if (0 == compile_ok) {
      fprintf(stderr, "Error in vertex shader\n");
      return 0;
    }

    /* Fragment shader */
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char *fs_source =
      "#version 120           \n"
      "void main(void) {        "
      "  gl_FragColor[0] = gl_FragCoord.x/640.0; "
      "  gl_FragColor[1] = 0.5; "
      "  gl_FragColor[2] = 1.0; "
      "}                        ";
    
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
    if (!compile_ok) {
      fprintf(stderr, "Error in fragment shader\n");
      return 0;
    }

    /* GLSL program */
    GLint link_ok = GL_FALSE;
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
      fprintf(stderr, "glLinkProgram:");
      return 0;
    }

    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation(program, attribute_name);
    if (attribute_coord2d == -1) {
      fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
      return 0;
    } 

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      glClearColor(1.0, 1.0, 1.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);
 
      glUseProgram(program);
      glEnableVertexAttribArray(attribute_coord2d);
      
      GLfloat triangle_vertices[] = {
	0.0,  0.95,
	-0.2, -0.8,
	0.8, -0.8,
	0.8, -0.8,
	0.9, -0.9,
	0.7, -0.9,
      };
      
      /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
      glVertexAttribPointer(attribute_coord2d, // attribute
			    2,                 // number of elements per vertex, here (x,y)
			    GL_FLOAT,          // the type of each element
			    GL_FALSE,          // take our values as-is
			    0,                 // no extra data between each position
			    triangle_vertices  // pointer to the C array
			    );
 
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glDisableVertexAttribArray(attribute_coord2d);
 
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}

