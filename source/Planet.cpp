#include "Planet.h"

Planet::Planet(GLuint sphereArrayOffset, GLuint sphereArraySize, GLuint ringArrayOffset, GLuint ringArraySize, vec4 center, type planetType) {
	this->center = center;
	this->planetType = planetType;
	this->sphereArrayOffset = sphereArrayOffset;
	this->sphereArraySize = sphereArraySize;
	this->ringArrayOffset = ringArrayOffset;
	this->ringArraySize = ringArraySize;
	this->hasRing = rand() % 100;
	this->ringXRot = rand() % 360;
	this->ringYRot = rand() % 360;
	this->planetType = planetType;

	switch (planetType) {
		// Yellow bright material
		case type::STAR:
			this->radius = 750;
			this->planetMat = Material(Material::gold);
			break;
		case type::FAR_STAR:
			// White bright material
			this->radius = 5;
			this->planetMat = Material(Material::bright_white);
			break;
		default:
		case type::PLANET:
			this->radius = rand() % 45 + 75;
			this->planetMat = Material(Material::type(rand() % 8));
			this->ringMat = Material(Material::type(rand() % 8));
			break;
		case type::ASTEROID:
			break;
		case type::SATELLITE:
			break;
	}

	// Put the planet to right position with right size
	this->model = Translate(center) * Scale(radius);

	calculateLighting();
}

void Planet::calculateLighting() {
	this->p_ambient_product = planetMat.light_ambient * planetMat.material_ambient;
	this->p_diffuse_product = planetMat.light_diffuse * planetMat.material_diffuse;
	this->p_specular_product = planetMat.light_specular * planetMat.material_specular;

	this->r_ambient_product = ringMat.light_ambient * ringMat.material_ambient;
	this->r_diffuse_product = ringMat.light_diffuse * ringMat.material_diffuse;
	this->r_specular_product = ringMat.light_specular * ringMat.material_specular;
}

void Planet::draw(mat4 view, GLuint program, GLuint modelView_ptr) {
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, this->p_ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, this->p_diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, this->p_specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), this->planetMat.material_shininess);
	
	glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model);
	glDrawArrays(GL_TRIANGLES, sphereArrayOffset, sphereArraySize);

	if (hasRing > 74 && this->planetType == PLANET) {
		glDisable(GL_CULL_FACE);
		glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, this->r_ambient_product);
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, this->r_diffuse_product);
		glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, this->r_specular_product);
		glUniform1f(glGetUniformLocation(program, "Shininess"), this->ringMat.material_shininess);
		glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model * RotateY(ringYRot) * RotateX(ringXRot) * Scale(0.9, 0.9, 0.5) );
		glDrawArrays(GL_TRIANGLES, ringArrayOffset, ringArraySize);
		glEnable(GL_CULL_FACE);
	}
}