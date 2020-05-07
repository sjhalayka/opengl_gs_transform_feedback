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
	vec4 vertex0 = vec4(1, 2, 3, 0); 
	vec4 vertex1 = vec4(4, 5, 6, 0);    
    vec4 vertex2 = vec4(7, 8, 9, 0);

	gl_Position = vertex0;
    EmitVertex();
    gl_Position = vertex1;
    EmitVertex();
    gl_Position = vertex2;
    EmitVertex();
    EndPrimitive();
}  