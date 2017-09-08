#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_offset;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;

void main()
{
	texcoord = in_uv;
	norm =  mat3(uModel) * in_normal;
	
	vec4 viewSpace = uView * uModel * vec4(in_vert + in_offset,1.0f);
	vec4 modelSpace = uModel * vec4(in_vert + in_offset,1.0f);

	gl_Position = uProj * viewSpace;
	
	pos = modelSpace.xyz;
}