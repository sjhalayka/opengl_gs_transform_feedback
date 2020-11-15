#ifndef VERTEX_GEOMETRY_SHADER
#define VERTEX_GEOMETRY_SHADER

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
using namespace std;


class vertex_geometry_shader
{
public:

	vertex_geometry_shader(void) { program = 0; }
	~vertex_geometry_shader(void) { if (program != 0) { glDeleteProgram(program); } }

	bool init(const char* vertex_shader_filename, const char* geometry_shader_filename, string varying_name);
	void use_program(void);
	GLuint get_program(void) { return program; };

private:
	const GLchar* read_text_file(const char* filename);
	GLuint program;

};


#endif