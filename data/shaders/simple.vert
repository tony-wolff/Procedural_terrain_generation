#version 330 core

uniform mat4 obj_mat;
uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat3 normal_mat;


in vec3 vtx_position;
in vec3 vtx_normal;
in vec2 vtx_texcoord;

out vec3 v_normal;
out vec2 v_uv;

void main()
{
  v_uv  = vtx_texcoord;
  //vec4 y =  texture2D(heightMap, vtx_texcoord);
  v_normal = normalize(normal_mat * vtx_normal);
  vec4 p = view_mat * (obj_mat * vec4(vtx_position, 1.));
  //vec4 p = view_mat * (obj_mat *y );
  gl_Position = proj_mat * p;
}
