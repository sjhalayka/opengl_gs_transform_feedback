#include <GL/glew.h>
#include <GL/glut.h>

// Automatically link in the GLUT and GLEW libraries if compiling on MSVC++
#ifdef _MSC_VER
#pragma comment(lib, "glew32")
#pragma comment(lib, "freeglut")
#endif

#include <iostream>
#include <vector>
using namespace std;

#include "vertex_geometry_shader.h"


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(10, 10);
	glutInitWindowPosition(0, 0);

	GLint win_id = glutCreateWindow("Julia 4D 3");

	if (GLEW_OK != glewInit())
	{
		cout << "GLEW initialization error" << endl;
		return false;
	}

	int GL_major_version = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &GL_major_version);

	int GL_minor_version = 0;
	glGetIntegerv(GL_MINOR_VERSION, &GL_minor_version);

	if (GL_major_version < 4)
	{
		cout << "GPU does not support OpenGL 4.3 or higher" << endl;
		return false;
	}
	else if (GL_major_version == 4)
	{
		if (GL_minor_version < 3)
		{
			cout << "GPU does not support OpenGL 4.3 or higher" << endl;
			return false;
		}
	}

	vertex_geometry_shader g0_mc_shader;

	if (false == g0_mc_shader.init("points.vs.glsl", "points.gs.glsl"))
	{
		cout << "Couldn't load shaders" << endl;

		return false;
	}

	glUseProgram(g0_mc_shader.get_program());

	vector<float> points_vertex_data;

	// Make enough data for one point
	points_vertex_data.push_back(0);
	points_vertex_data.push_back(0);
	points_vertex_data.push_back(0);
	points_vertex_data.push_back(0);

	const GLuint components_per_position = 4;
	const GLuint components_per_vertex = components_per_position;

	static GLuint triangle_buffer;

	glDeleteBuffers(1, &triangle_buffer);
	glGenBuffers(1, &triangle_buffer);

	const GLuint num_vertices = static_cast<GLuint>(points_vertex_data.size()) / components_per_vertex;

	glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
	glBufferData(GL_ARRAY_BUFFER, points_vertex_data.size() * sizeof(GLfloat), &points_vertex_data[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(glGetAttribLocation(g0_mc_shader.get_program(), "position"));
	glVertexAttribPointer(glGetAttribLocation(g0_mc_shader.get_program(), "position"),
		components_per_position,
		GL_FLOAT,
		GL_FALSE,
		components_per_vertex * sizeof(GLfloat),
		0);

	GLuint query;

	glGenQueries(1, &query);

	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	glBeginTransformFeedback(GL_TRIANGLES);
		glDrawArrays(GL_POINTS, 0, num_vertices);
	glEndTransformFeedback();

	glEndQuery(GL_PRIMITIVES_GENERATED);

	GLuint primitives;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

	cout << primitives << endl;

	// how to set up transform feedback?


	return 0;
}