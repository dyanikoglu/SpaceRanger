#include "Camera.h"

//************************************
// Method:    Camera
// FullName:  Camera::Camera
// Access:    public 
// Initializes the camera. focal takes place for "at" point on Angel.h
//************************************
Camera::Camera() {
	this->focal = vec4(0, 0, 0, 1);
	this->position = vec4(0, 0, 0, 1);
	this->up = vec4(0, 0, 1, 0);
	this->view = LookAt(position, focal, up);
	this->currMode = VIEW_MODE::FP;
}

void Camera::attach(SpaceShip* player) {
	this->actor = player;
}

//************************************
// Method:    update
// FullName:  Camera::update
// Access:    public 
// Returns:   void
// Updates components of camera. Also adds some camera shake with respect to linear velocity of attached ship.
//************************************
void Camera::update() {
	GLfloat camShake = rand() % 5;
	camShake = (actor->linearVelocity * camShake) / 1700.f;
	if (rand() % 2 == 1) {
		camShake = -camShake;
	}

	switch(this->currMode) {
		case VIEW_MODE::TP:
			this->position = Translate(actor->position) * RotateZ(actor->heading) * Translate(-actor->position) * actor->TPCameraAttachPoint;
			this->focal = Translate(RotateZ(actor->heading) * vec4(camShake, 4 + camShake, camShake, 0)) * actor->position;
			break;
		case VIEW_MODE::FP:
			this->position = Translate(actor->position) * RotateZ(actor->heading) * Translate(-actor->position) * actor->FPCameraAttachPoint;
			this->focal = Translate(RotateZ(actor->heading) * vec4(camShake, 7 + camShake, camShake, 0)) * actor->position;
			break;
		case VIEW_MODE::TOP:
			this->position = vec4(0.1, 0.1, 2500, 1);
			this->focal = vec4(0, 0, 0, 1);
			break;
	}

	this->view = LookAt(position, focal, up);
}