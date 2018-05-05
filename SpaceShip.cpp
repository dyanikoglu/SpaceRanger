#include "SpaceShip.h"

SpaceShip::SpaceShip(vec4 initPosition, GLfloat initHeading, GLfloat initLinearVel, GLfloat initAngularVel, GLuint lifespan) {
	this->lifespan = lifespan;

	bufferOffset = 0;
	heading = initHeading;
	linearVelocity = initLinearVel;
	targetLinearVel = initLinearVel;
	angularVelocity = initAngularVel;
	targetAngularVel = initAngularVel;

	isCrashed = 0;
	isHitByBeam = 0;

	crashR1Rot = 0;
	crashR2Rot = 0;
	crashR1X = 0;
	crashR2X = 0;
	crashR2Y = 0;
	crashR1Y = 0;
	crashR2Z = 0;
	crashR1Z = 0;

	model = Translate(initPosition) * mat4(); // Put the ship at initial position
	position = initPosition;

	// Initialize camera attach points
	TPCameraAttachPoint = Translate(0, -10, 5) * initPosition;
	FPCameraAttachPoint = Translate(0, 0, 2.5) * initPosition;
	

	GLfloat r_outer = 2;
	GLfloat r_inner = 2 - 0.001f;
	for (GLfloat theta = 0; theta < 360.f; theta+=0.5) {
		GLfloat angle = theta * TO_RADIANS;
		GLfloat angle2 = (theta+0.5) * TO_RADIANS;

		vec4 a = vec4(r_outer*sin(angle), r_outer*cos(angle), -0.5, 1.f);
		vec4 b = vec4(r_outer*sin(angle2), r_outer*cos(angle2), -0.5, 1.f);
		vec4 c = vec4(r_outer*sin(angle), r_outer*cos(angle), 0.5, 1.f);

		vec4 a2 = vec4(r_inner*sin(angle), r_inner*cos(angle), -0.5, 1.f);
		vec4 b2 = vec4(r_inner*sin(angle2), r_inner*cos(angle2), -0.5, 1.f);
		vec4 c2 = vec4(r_inner*sin(angle), r_inner*cos(angle), 0.5, 1.f);

		vec4 d = vec4(r_outer*sin(angle), r_outer*cos(angle),0.5, 1.f);
		vec4 e = vec4(r_outer*sin(angle2), r_outer*cos(angle2),0.5, 1.f);
		vec4 f = vec4(r_outer*sin(angle2), r_outer*cos(angle2), -0.5, 1.f);

		vec4 d2 = vec4(r_inner*sin(angle), r_inner*cos(angle),0.5, 1.f);
		vec4 e2 = vec4(r_inner*sin(angle2), r_inner*cos(angle2),0.5, 1.f);
		vec4 f2 = vec4(r_inner*sin(angle2), r_inner*cos(angle2), -0.5, 1.f);


		vec4 u = c - b;
		vec4 v = c - a;
		vec3 normal = normalize(cross(u, v));
		for (int i = 0; i < 6; i++) {
			this->normals.emplace_back(normal);
		}

		points.emplace_back(c);
		points.emplace_back(b);
		points.emplace_back(a);
		points.emplace_back(d);
		points.emplace_back(e);
		points.emplace_back(f);


		vec4 u2 = c2 - b2;
		vec4 v2 = c2 - a2;
		vec3 normal2 = normalize(cross(v2, u2));
		for (int i = 0; i < 6; i++) {
			this->normals.emplace_back(normal2);
		}

		points.emplace_back(c2);
		points.emplace_back(b2);
		points.emplace_back(a2);
		points.emplace_back(d2);
		points.emplace_back(e2);
		points.emplace_back(f2);
	}

	mat = Material(Material::type(rand() % 9));

	calculateLighting();
}

SpaceShip::~SpaceShip()
{
	this->points.clear();
	this->normals.clear();
}

void SpaceShip::calculateLighting() {
	this->ambient_product = mat.light_ambient * mat.material_ambient;
	this->diffuse_product = mat.light_diffuse * mat.material_diffuse;
	this->specular_product = mat.light_specular * mat.material_specular;
}

void SpaceShip::draw(mat4 view, GLuint program, GLuint modelView_ptr) {
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, this->ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, this->diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, this->specular_product);
	glUniform1f(glGetUniformLocation(program, "Shininess"), this->mat.material_shininess);
	
	if (isCrashed) {
		// Some complicated crash animation stuff.
		
		glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model * Translate(crashR1X, crashR1Y, crashR1Z) * RotateY(crashR2Rot) * RotateX(crashR1Rot));
		glDrawArrays(GL_TRIANGLES, 0, this->points.size());

		// Draw second ring
		glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model * Translate(crashR2X, crashR2Y, crashR2Z) * RotateX(90+ crashR2Rot) * RotateY(crashR2Rot) * RotateY(90));
		glDrawArrays(GL_TRIANGLES, 0, this->points.size());
	}
	else { // No crash, continue drawing normally
		// Draw first ring
		glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model);
		glDrawArrays(GL_TRIANGLES, 0, this->points.size());

		// Draw second ring
		glUniformMatrix4fv(modelView_ptr, 1, GL_TRUE, view * model * RotateX(90) * RotateY(90));
		glDrawArrays(GL_TRIANGLES, 0, this->points.size());
	}
}

//************************************
// Method:    move
// FullName:  SpaceShip::move
// Access:    public 
// Returns:   void
// On each frame, interpolate current velocities to target velocities. Then update components of ship.
//************************************
void SpaceShip::move() {
	// Some lerp stuff for smooth movement
	if (abs(targetLinearVel - linearVelocity) > 0.001) {
		linearVelocity = lerp(linearVelocity, targetLinearVel, 0.05);
	}
	else {
		linearVelocity = targetLinearVel;
	}

	if (abs(targetAngularVel - angularVelocity) > 0.001) {
		angularVelocity = lerp(angularVelocity, targetAngularVel, 0.075);
	}
	else {
		angularVelocity = targetAngularVel;
	}
	////

	if (isCrashed) { // If spaceship is crashed, do some animation stuff
		// Some complicated crash animation stuff.
		mat4 temp = Translate(RotateZ(heading - 180) * vec4(0, linearVelocity, 0, 0));
		if (isHitByBeam == 0) {
			// Push back the ship
			position = temp * position;
		}
		else {
			// If ship hit by beam, do not push back it
			linearVelocity = 0;
		}
		targetLinearVel = 0;
		TPCameraAttachPoint = temp * TPCameraAttachPoint;
		FPCameraAttachPoint = temp * FPCameraAttachPoint;
		model = temp * model;

		// Swing first ring of ship
		crashR1Rot = lerp(crashR1Rot, 209, 0.01);
		crashR1X -= 0.01;
		crashR1Y -= 0.01;
		crashR1Z -= 0.01;

		// Swing second ring of ship
		crashR2X += 0.01;
		crashR2Y += 0.01;
		crashR2Z -= 0.01;
		crashR2Rot = lerp(crashR2Rot, 223, 0.01);
	}

	else { // No crash, calculate normally
		// Change heading by current angularVelocity
		heading += -angularVelocity;
		//heading = (int)heading % 360 + (heading - (int)heading); 
		if (heading < -180) {
			heading =  360 + heading;
		} else if (heading > 180) {
			heading = -360 + heading;
		}

		// Rotate ship vertices by angularVelocity
		model = Translate(position) * RotateZ(-angularVelocity) * Translate(-position) * model;

		// Change ship rotation by heading, and move ship on that direction
		mat4 temp = Translate(RotateZ(heading) * vec4(0, linearVelocity, 0, 0));
		position = temp * position;

		TPCameraAttachPoint = temp * TPCameraAttachPoint;
		FPCameraAttachPoint = temp * FPCameraAttachPoint;
		model = temp * model;
	}
}