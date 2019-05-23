//"#version 100\n"  // OpenGL ES 2.0
//"#version 120\n"  // OpenGL 2.1

attribute vec2 coord2d;
attribute vec3 v_color;

varying vec3 f_color;
uniform mat4 MVP;

void main(void) {
  gl_Position = MVP * vec4(coord2d, 0.0, 1.0);
  f_color = v_color;
}
