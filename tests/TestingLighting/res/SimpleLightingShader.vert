attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;
uniform vec3 u_light_position;

uniform mat4 u_mv_matrix;
uniform mat4 u_projection_matrix;
uniform mat4 u_normal_matrix;

varying vec3 v_position;
varying vec3 v_normal;
varying vec4 v_color;
varying vec3 v_light_position;

void main()
{
  vec4 transformedPosition = u_mv_matrix * vec4(a_position, 1);

  v_position = transformedPosition.xyz;
  v_normal = (u_normal_matrix * vec4(a_normal, 1)).xyz;
  v_color = a_color;
  v_light_position = u_light_position;

  gl_Position = u_projection_matrix * transformedPosition;
}
