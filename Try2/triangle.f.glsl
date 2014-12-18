void main(void) {        
  gl_FragColor[0] = gl_FragCoord.x/640.0;
  gl_FragColor[1] = 0.75;
  gl_FragColor[2] = 1.0;
}