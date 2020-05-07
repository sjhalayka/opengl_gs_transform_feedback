#version 430 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 15) out;

in VS_OUT
{
    vec4 position;
} gs_in[];

void main(void)
{
	vec4 vertex0 = vec4(gs_in[0].position.x, 0, 0, 0); 
	vec4 vertex1 = vec4(gs_in[0].position.x, gs_in[0].position.y, 0, 0);    
    vec4 vertex2 = vec4(gs_in[0].position.x, gs_in[0].position.y, gs_in[0].position.z, 0);

	gl_Position = vertex0;
    EmitVertex();
    gl_Position = vertex1;
    EmitVertex();
    gl_Position = vertex2;
    EmitVertex();
    EndPrimitive();
}  