varying vec3 f_color;

void main(void) {
  gl_FragColor = vec4(1.0 - f_color.x, 0.1 * f_color.y, 1.0 - f_color.z, gl_FragCoord.x / 3000);
}
