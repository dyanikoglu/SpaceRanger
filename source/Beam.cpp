#pragma once
#include "Beam.h"

Beam::Beam(GLuint arrayOffset, vec4 initPosition, GLfloat heading, GLuint lifespan, SpaceShip* owner) {
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);

	this->owner = owner;
	this->lifespan = lifespan;
	this->velocity = owner->linearVelocity + 8;
	this->position = Translate(RotateZ(heading) * vec4(0, velocity, 0, 0)) * initPosition;
	this->heading = heading;
	this->arrayOffset = arrayOffset;

	mat = Material(Material::type::bright_green);
	
	model = Translate(position) * RotateZ(heading) * Scale(0.2,1.5,0.2);

	calculateLighting();
}

Beam::~Beam() {
	this->points.clear();
	this->normals.clear();
}

void Beam::calculateLighting() {
	this->ambient_product = mat.light_ambient * mat.material_ambient;
	this->diffuse_product = mat.light_diffuse * mat.material_diffuse;
	this->specular_product = mat.light_specular * mat.material_specular;
}

void Beam::quad(int a, int b, int c, int d)
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];

	vec3 normal = normalize(cross(u, v));

	for (int i = 0; i < 6; i++) {
		normals.emplace_back(normal);
	}

	points.emplace_back(vertices[a]);
	points.emplace_back(vertices[b]);
	points.emplace_back(vertices[c]);
	points.emplace_back(vertices[a]);
	points.emplace_back(vertices[c]);
	points.emplace_back(vertices[d]);
}


//************************************
// Method:    move
// FullName:  Beam::move
// Access:    public 
// Returns:   void
// Move beam with same moving direction of owner when it's shot
//************************************
void Beam::move() {
	mat4 temp = Translate(RotateZ(heading) * vec4(0, velocity, 0, 0));
	position = temp * position;
	model = temp * model;
}

void Beam::draw(mat4 view, GLuint program, GLuint modelView_ptr) {
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, this->ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, this->diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, this->specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), this->mat.material_shininess);

	glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model);
	glDrawArrays(GL_TRIANGLES, arrayOffset, this->points.size());
}