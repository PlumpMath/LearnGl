attribute vec3 coord3d;
attribute vec3 v_color;
varying vec3 f_color;

uniform mat4 m_transform;
uniform float m_time;

// attribute - per vertex
// varying - send to fragment shader
// uniform - for the whole VBO

void main(void) {
  vec3 modpos = vec3(coord3d.x + sin(m_time) * 0.3, coord3d.y, coord3d.z);
  gl_Position = m_transform * vec4(modpos, 1.0);
  f_color = v_color;
}
