#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include "LoadShaders.h"

GLint attribute_coord3d;
GLint attribute_v_color;
GLint uniform_m_transform;

struct attributes {
  GLfloat coord3d[3];
  GLfloat v_color[3];
};

void draw(GLuint vbo, float x, float y, float rot) {
  glEnableVertexAttribArray(attribute_coord3d);
  glEnableVertexAttribArray(attribute_v_color);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT,	GL_FALSE, sizeof(struct attributes), 0);

  void *offset = (void*)offsetof(struct attributes, v_color);
  glVertexAttribPointer(attribute_v_color,3,GL_FLOAT,GL_FALSE,sizeof(struct attributes), offset);

  mat4x4 translation;
  mat4x4_identity(translation);
  mat4x4_translate_in_place(translation, x, y, 0.0f);

  mat4x4 rotation;
  mat4x4_identity(rotation);
  mat4x4_rotate(rotation, translation, 0.0f, 0.0f, 1.0f, rot);

  glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, (float*)rotation);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  
  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_v_color);
}

int main(void)
{
  GLFWwindow* window;

  if (!glfwInit()) {
    return -1;
  }

  window = glfwCreateWindow(640, 640, "ERIK IS LEARNING", NULL, NULL);
  if (!window) {
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
  GLuint program;
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

  struct attributes shape1_data[] = {
    {{-0.25, 0.25, 0.0}, {1.0, 0.0, 0.0}},
    {{0.25, 0.25, 0.0}, {0.0, 1.0, 1.0}},
    {{-0.25, -0.25, 0.0}, {0.0, 1.0, 1.0}},
    {{0.25, -0.25, 0.0}, {1.0, 1.0, 0.0}}
  };

  struct attributes shape2_data[] = {
    {{-0.35, 0.25, 0.0}, {1.0, 0.0, 0.0}},
    {{0.35, 0.25, 0.0}, {0.0, 1.0, 1.0}},
    {{-0.15, -0.25, 0.0}, {0.0, 0.5, 1.0}},
    {{0.15, -0.25, 0.0}, {0.5, 1.0, 0.0}}
  };

  GLuint vbo_shape1, vbo_shape2;
  
  glGenBuffers(1, &vbo_shape1);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_shape1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(shape1_data), shape1_data, GL_STATIC_DRAW);

  glGenBuffers(1, &vbo_shape2);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_shape2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(shape2_data), shape2_data, GL_STATIC_DRAW);

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
    float angle = rotate;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
 
    glUseProgram(program);
    
    draw(vbo_shape1, -0.4f, 0.0f, -angle / 2);
    draw(vbo_shape2, 0.2f, 0.2f, angle);

    for (float i = -1.0f; i < 1.0f; i += 0.05) {
      draw(vbo_shape2, i, -0.8f, i + angle / 10.0f);
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_shape1);
  glDeleteBuffers(2, &vbo_shape2);
    
  glfwTerminate();
  return 0;
}

