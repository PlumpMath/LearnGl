#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LoadShaders.h"

GLuint program;
GLint attribute_coord2d, attribute_v_color;
GLuint vbo_triangle, vbo_triangle_colors;
GLint uniform_fade;

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

  GLuint vs, fs;
  if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;
    
  /* GLSL program */
  GLint link_ok = GL_FALSE;
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    print_log(program);
    return 0;
  }

  GLfloat triangle_attributes[] = {
     0.0,  0.8,   1.0, 1.0, 0.0,
    -0.8, -0.8,   0.0, 1.0, 1.0,
     0.8, -0.8,   1.0, 0.0, 0.0,
  };
  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

  const char* attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  const char* attribute_name2 = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name2);
  if (attribute_v_color == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name2);
    return 0;
  }

   const char* uniform_name;
   uniform_name = "fade";
   uniform_fade = glGetUniformLocation(program, uniform_name);
   if (uniform_fade == -1) {
     fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
     return 0;
   }

   int t = 1;
   
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
    {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      glClearColor(1.0, 1.0, 1.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);
 
      glUseProgram(program);

      t += 1;
      float fade = 0.5f + cosf(t * 0.1f) * 0.5f;
      glUniform1f(uniform_fade, fade);

      glEnableVertexAttribArray(attribute_coord2d);
      glEnableVertexAttribArray(attribute_v_color);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
      
      glVertexAttribPointer(
			    attribute_coord2d, // attribute
			    2,                 // number of elements per vertex, here (x,y)
			    GL_FLOAT,          // the type of each element
			    GL_FALSE,          // take our values as-is
			    5 * sizeof(GLfloat),
			    0                  // offset of first element
			    );
 
      glVertexAttribPointer(
			    attribute_v_color, // attribute
			    3,                 // number of elements per vertex, here (x,y)
			    GL_FLOAT,          // the type of each element
			    GL_FALSE,          // take our values as-is
			    5 * sizeof(GLfloat),                 // no extra data between each position
			    (GLvoid*) (2 * sizeof(GLfloat))
			    );

      glDrawArrays(GL_TRIANGLES, 0, 3);
 
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
    
  glfwTerminate();
  return 0;
}

