#include <GL/glew.h>
#include <GL/glut.h>



#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

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

	GLint win_id = glutCreateWindow("GS Test");

	if (GLEW_OK != glewInit())
	{
		cout << "GLEW initialization error" << endl;
		return 0;
	}

	int GL_major_version = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &GL_major_version);

	int GL_minor_version = 0;
	glGetIntegerv(GL_MINOR_VERSION, &GL_minor_version);

	if (GL_major_version < 4)
	{
		cout << "GPU does not support OpenGL 4.3 or higher" << endl;
		return 0;
	}
	else if (GL_major_version == 4)
	{
		if (GL_minor_version < 3)
		{
			cout << "GPU does not support OpenGL 4.3 or higher" << endl;
			return 0;
		}
	}

	vertex_geometry_shader g0_mc_shader;

	if (false == g0_mc_shader.init("points.vs.glsl", "points.gs.glsl", "vert"))
	{
		cout << "Couldn't load shaders" << endl;
		return 0;
	}

	g0_mc_shader.use_program();

	// Make enough data for 1 point
	vector<float> point_vertex_data;

	point_vertex_data.push_back(0);
	point_vertex_data.push_back(0);
	point_vertex_data.push_back(0);
	point_vertex_data.push_back(0);

	const GLuint components_per_position = 4;
	const GLuint components_per_vertex = components_per_position;

	GLuint point_buffer;

	glGenBuffers(1, &point_buffer);

	const GLuint num_vertices = static_cast<GLuint>(point_vertex_data.size()) / components_per_vertex;

	glBindBuffer(GL_ARRAY_BUFFER, point_buffer);
	glBufferData(GL_ARRAY_BUFFER, point_vertex_data.size() * sizeof(GLfloat), &point_vertex_data[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(glGetAttribLocation(g0_mc_shader.get_program(), "position"));
	glVertexAttribPointer(glGetAttribLocation(g0_mc_shader.get_program(), "position"),
		components_per_position,
		GL_FLOAT,
		GL_FALSE,
		components_per_vertex * sizeof(GLfloat),
		0);

	glUseProgram(g0_mc_shader.get_program());


	size_t max_triangles_per_geometry_shader = 2;
	size_t num_vertices_per_triangle = 3;
	size_t num_floats_per_vertex = 3;

	// Allocate enough for the maximum number of triangles
	GLuint tbo;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * num_vertices * max_triangles_per_geometry_shader * num_vertices_per_triangle * num_floats_per_vertex, nullptr, GL_STATIC_READ);

	GLuint query;
	glGenQueries(1, &query);

	// Perform feedback transform
	glEnable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
	glBeginTransformFeedback(GL_TRIANGLES);
	glDrawArrays(GL_POINTS, 0, num_vertices);
	glEndTransformFeedback();
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	glDisable(GL_RASTERIZER_DISCARD);

	glFlush();

	GLuint primitives;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

	// Read back actual number of triangles (in case it's less than two triangles)
	vector<GLfloat> feedback(primitives * num_vertices_per_triangle * num_floats_per_vertex);
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(GLfloat) * feedback.size(), &feedback[0]);

	glDeleteQueries(1, &query);
	glDeleteBuffers(1, &tbo);

	for (size_t i = 0; i < primitives; i++)
	{
		size_t feedback_index = 9 * i;

		cout << feedback[feedback_index + 0] << " ";
		cout << feedback[feedback_index + 1] << " ";
		cout << feedback[feedback_index + 2] << endl;
		cout << feedback[feedback_index + 3] << " ";
		cout << feedback[feedback_index + 4] << " ";
		cout << feedback[feedback_index + 5] << endl;
		cout << feedback[feedback_index + 6] << " ";
		cout << feedback[feedback_index + 7] << " ";
		cout << feedback[feedback_index + 8] << endl;

		cout << endl;
	}

	return 1;
}