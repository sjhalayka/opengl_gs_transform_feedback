#include "vertex_geometry_shader.h"


bool vertex_geometry_shader::init(const char* vertex_shader_filename, const char* geometry_shader_filename, string varying_name)
{
	program = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* source = read_text_file(vertex_shader_filename);

	if (source == NULL)
	{
		glDeleteProgram(program);
		glDeleteShader(vertex_shader);
		return false;
	}

	glShaderSource(vertex_shader, 1, &source, NULL);
	delete[] source;

	glCompileShader(vertex_shader);
	GLint compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(vertex_shader, len, &len, log);
		cerr << "Vertex shader compilation failed: " << log << endl;
		delete[] log;

		glDeleteProgram(program);
		glDeleteShader(vertex_shader);
		return false;
	}

	glAttachShader(program, vertex_shader);



	GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);

	source = read_text_file(geometry_shader_filename);

	if (source == NULL)
	{
		glDeleteProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(geometry_shader);
		return false;
	}

	glShaderSource(geometry_shader, 1, &source, NULL);
	delete[] source;

	glCompileShader(geometry_shader);
	glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(geometry_shader, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(geometry_shader, len, &len, log);
		cerr << "Geometry shader compilation failed: " << log << endl;
		delete[] log;

		glDeleteProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(geometry_shader);

		return false;
	}

	glAttachShader(program, geometry_shader);


	if (varying_name != "")
	{
		const GLchar* feedbackVaryings[] = { varying_name.c_str() };
		glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
	}



	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		cerr << "Shader linking failed: " << log << endl;
		delete[] log;

		glDeleteProgram(program);
		glDeleteShader(vertex_shader);
		glDeleteShader(geometry_shader);

		return false;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(geometry_shader);

	return true;
}



#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

const GLchar* vertex_geometry_shader::read_text_file(const char* filename)
{
	FILE* infile = fopen(filename, "rb");

	if (!infile)
	{
		cerr << "Unable to open file '" << filename << "'" << endl;
		return NULL;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	fread(source, sizeof(char), len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);
}

void vertex_geometry_shader::use_program(void)
{
	glUseProgram(program);
}