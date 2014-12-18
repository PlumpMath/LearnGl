#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LoadShaders.h"
#include <Linmath.h>

GLuint program;
GLuint vbo_triangle;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_fade;
GLint uniform_m_transform;

struct attributes {
  GLfloat coord3d[3];
  GLfloat v_color[3];
};

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

  struct attributes triangle_attributes[] = {
    {{ 0.0,  0.8, 0.0}, {1.0, 1.0, 0.0}},
    {{-0.8, -0.8, 0.0}, {0.0, 0.0, 1.0}},
    {{ 0.8, -0.8, 0.0}, {1.0, 0.0, 0.0}}
  };
  
  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

  const char* attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (attribute_v_color == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }

  const char* uniform_name = "m_transform";
  uniform_m_transform = glGetUniformLocation(program, uniform_name);
  if (uniform_m_transform == -1) {
    fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
    return 0;
  }

  float rotate = 0.0f;
  
   
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {

    rotate += 0.01f;
    
    mat4x4 transform, tmpa, tmpb;
    mat4x4_perspective(tmpb, 70, 1, 0.5f, 2000);
    mat4x4_look_at(tmpa, (vec3){sin(rotate)*64 + 32,100,cos(rotate)*64 + 32}, (vec3){32,32,32}, (vec3){0,64,0});
    mat4x4_mul(transform, tmpb, tmpa);

    glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, (const float*)transform);
      
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
 
    glUseProgram(program);
    
    glEnableVertexAttribArray(attribute_coord3d);
    glEnableVertexAttribArray(attribute_v_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glVertexAttribPointer(
			  attribute_coord3d,   // attribute
			  3,                   // number of elements per vertex, here (x,y,z)
			  GL_FLOAT,            // the type of each element
			  GL_FALSE,            // take our values as-is
			  sizeof(struct attributes),  // next coord3d appears every 5 floats
			  0                    // offset of first element
			  );
    glVertexAttribPointer(
			  attribute_v_color,      // attribute
			  3,                      // number of elements per vertex, here (r,g,b)
			  GL_FLOAT,               // the type of each element
			  GL_FALSE,               // take our values as-is
			  sizeof(struct attributes),  // stride
			  //(void*) (2 * sizeof(GLfloat))     // offset of first element
			  (void*) offsetof(struct attributes, v_color)  // offset
			  );
 
    /* Push each element in buffer_vertices to the vertex shader */
    glDrawArrays(GL_TRIANGLES, 0, 3);
 
    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_v_color);
 
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
    
  glfwTerminate();
  return 0;
}

