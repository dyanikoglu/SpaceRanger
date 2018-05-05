#include "Surface.h"

//************************************
// Method:    Surface
// FullName:  Surface::Surface
// Access:    public 
// Parameter: GLuint arrayOffset, Offset of points in buffer
// Parameter: SpaceShip * player, Pointer to current player
// Parameter: GLint squareCountX, Total square count on x axis
// Parameter: GLint squareCountY,  Total square count on y axis
// Parameter: GLint squareSize,  Size for each square
//************************************
Surface::Surface(GLuint arrayOffset, SpaceShip* player, GLint squareCountX, GLint squareCountY, GLint squareSize) {
	this->squareCountX = squareCountX;
	this->squareCountY = squareCountY;
	this->squareSize = squareSize;

	this->actor = player;
	this->arrayOffset = arrayOffset;

	vec4 v1 = vec4(0.5, 0.5, 0, 1);
	vec4 v2 = vec4(-0.5, 0.5, 0, 1);
	vec4 v3 = vec4(-0.5, -0.5, 0, 1);
	vec4 v4 = vec4(0.5, -0.5, 0, 1);

	vec4 u = v2 - v1;
	vec4 v = v3 - v2;

	vec3 normal = normalize(cross(u, v));

	for (GLint i = 0; i < 5; i++) {
		this->normals.emplace_back(normal);
	}

	this->points.emplace_back(v1);
	this->points.emplace_back(v2);
	this->points.emplace_back(v3);
	this->points.emplace_back(v4);
	this->points.emplace_back(v1);

	params.light_ambient = vec4(1, 1, 1, 1.0);
	params.light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	params.light_specular = vec4(1.0, 1.0, 1.0, 1.0);

	params.material_ambient = vec4(0.633, 0.727811, 0.633, 1.0);
	params.material_diffuse = vec4(0.7568, 0.61424, 0.07568, 1.0);
	params.material_specular = vec4(0.215, 0.1745, 0.0215, 1.0);
	params.material_shininess = 1000;

	buildSurfaceMatrix();

	calculateLighting();

	this->arraySize = points.size();
}


//************************************
// Method:    changeActor
// FullName:  Surface::changeActor
// Access:    public 
// Returns:   void
// Parameter: SpaceShip * player
// Swap actor pointer and recalculate surface matrix
//************************************
void Surface::changeActor(SpaceShip* player) {
	this->actor = player;
	buildSurfaceMatrix();
}


//************************************
// Method:    checkBounds
// FullName:  Surface::checkBounds
// Access:    public 
// Returns:   void
// Extends infinitely the surface matrix according to player's position 
//************************************
void Surface::checkBounds() {
	if (actor->position.x < xBound1 || actor->position.x > xBound2) {
		buildSurfaceMatrix();
	}
	else if (actor->position.y < yBound1 || actor->position.y > yBound2) {
		buildSurfaceMatrix();
	}
}

//************************************
// Method:    buildSurfaceMatrix
// FullName:  Surface::buildSurfaceMatrix
// Access:    private 
// Returns:   void
// Calculates vertices of current surface matrix and it's bounds
//************************************
void Surface::buildSurfaceMatrix() {
	models.clear();

	for (GLint i = -squareCountX/2; i < squareCountX/2; i++) {
		for (GLint j = -squareCountY/2; j < squareCountY/2; j++) {
			this->models.emplace_back(Translate((int)(actor->position.x / squareSize) * squareSize + i * squareSize, (int)(actor->position.y / squareSize) * squareSize + j * squareSize, -50) * Scale(squareSize));
		}
	}

	xBound1 = (actor->position.x / squareSize) * squareSize - (squareSize * squareCountX / 2.f) + 10 * squareSize;
	xBound2 = (actor->position.x / squareSize) * squareSize + (squareSize * squareCountX / 2.f) - 10 * squareSize;
	yBound1 = (actor->position.y / squareSize) * squareSize - (squareSize * squareCountY / 2.f) + 10 * squareSize;
	yBound2 = (actor->position.y / squareSize) * squareSize + (squareSize * squareCountY / 2.f) - 10 * squareSize;
}

void Surface::calculateLighting() {
	this->ambient_product = params.light_ambient * params.material_ambient;
	this->diffuse_product = params.light_diffuse * params.material_diffuse;
	this->specular_product = params.light_specular * params.material_specular;
}

void Surface::draw(mat4 view, GLuint program, GLuint modelView_ptr) {
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, this->ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, this->diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, this->specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), this->params.material_shininess);

	for (int i = 0; i < models.size(); i++) {
		glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * models[i]);
		glDrawArrays(GL_LINE_STRIP, arrayOffset, arraySize);
	}
}