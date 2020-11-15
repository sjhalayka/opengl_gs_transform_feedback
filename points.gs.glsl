#version 430 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 6) out;

out vec3 vert;

in VS_OUT
{
    vec4 position;
} gs_in[];

void main(void)
{
    vec3 vertex0 = vec3(1, 2, 3); 
    vec3 vertex1 = vec3(4, 5, 6);    
    vec3 vertex2 = vec3(7, 8, 9);

    vert = vertex0;
    EmitVertex();
    
    vert = vertex1;
    EmitVertex();
    
    vert = vertex2;
    EmitVertex();

    EndPrimitive();


    vertex0 = vec3(10, 11, 12); 
    vertex1 = vec3(13, 14, 15);    
    vertex2 = vec3(16, 17, 18);

    vert = vertex0;
    EmitVertex();
    
    vert = vertex1;
    EmitVertex();
    
    vert = vertex2;
    EmitVertex();

    EndPrimitive();
}