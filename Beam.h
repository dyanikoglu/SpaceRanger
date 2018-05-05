#pragma once
#include "Angel/Angel.h"
#include "Materials.h"
#include "SpaceShip.h"
#include <vector>

class Beam {
public:
	Beam(GLuint arrayOffset, vec4 initPosition, GLfloat heading, GLuint lifespan, SpaceShip* owner);
	~Beam();
	std::vector<vec4> points;
	std::vector<vec3> normals;
	void quad(int a, int b, int c, int d);
	vec4 vertices[8] = {
		vec4(-0.5, -0.5,  0.5, 1.0),
		vec4(-0.5,  0.5,  0.5, 1.0),
		vec4(0.5,  0.5,  0.5, 1.0),
		vec4(0.5, -0.5,  0.5, 1.0),
		vec4(-0.5, -0.5, -0.5, 1.0),
		vec4(-0.5,  0.5, -0.5, 1.0),
		vec4(0.5,  0.5, -0.5, 1.0),
		vec4(0.5, -0.5, -0.5, 1.0)
	};

	GLfloat velocity;
	GLfloat heading;
	GLuint arrayOffset;
	GLuint lifespan; // Lifespan of beam
	Material mat;
	vec4 position;
	mat4 model;
	SpaceShip* owner;
	void calculateLighting();
	void move();

	vec4 ambient_product;
	vec4 diffuse_product;
	vec4 specular_product;

	void draw(mat4 view, GLuint program, GLuint modelView_ptr);
};