#include "World.h"
#include <vector>

GLuint program;
GLuint  ModelView, Projection;
World* world;

void init()
{
	srand(3);

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	world = new World(program);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 0.f, 1.f); // white background
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world->draw();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l':
		world->player->targetAngularVel+= 0.1;
		break;
	case 'k':
		world->player->targetAngularVel+= -0.1; 
		break;
	case 'z':
		world->player->targetAngularVel = 0;
		world->player->targetLinearVel = 0;
		break;
	case 'a':
		world->player->targetLinearVel += 0.20;
		break;
	case 's':
		world->player->targetLinearVel += -0.20;
		break;
	case 't':
		world->camera->currMode = Camera::VIEW_MODE::TP;
		break;
	case 'f':
		world->camera->currMode = Camera::VIEW_MODE::FP;
		break;
	case 'w':
		if (world->camera->currMode != Camera::TOP) {
			world->camera->prevMode = world->camera->currMode;
			world->camera->currMode = Camera::TOP;
		}
		else if(world->camera->currMode == Camera::TOP) {
			world->camera->currMode = world->camera->prevMode;
		}
		break;
	case 'b':
		world->playerIndex = (world->playerIndex + 1) % 2;
		world->player = world->playerShips[world->playerIndex];
		world->surface->changeActor(world->player);
		world->camera->attach(world->player);
		break;
	case 'd':
		world->playerShoot();
		break;
	case 'q':
		exit(0);
		break;
	}
}

void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	mat4 projection = Perspective(60 * (float)width / height, (float)width / height, 0.01f, 1000.0f);
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		if (world->worldState == world->State::RUN) {
			world->worldState = world->State::PAUSE;
		}
		else if (world->worldState == world->State::PAUSE) {
			world->worldState = world->State::RUN;
		}
	}

	if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		if (world->worldState == world->State::RUN) {
			world->worldState = world->State::PAUSE;
		}
		else if (world->worldState == world->State::PAUSE) {
			world->worldState = world->State::ONE_FRAME;
		}
	}
}

void timer(GLint) {
	if (world->worldState != World::PAUSE) {
		if (world->worldState == World::ONE_FRAME) {
			using namespace std;
			cout << "@@@@@@@@@ FRAME " << world->frameCounter << " @@@@@@@@" << endl;
			cout << "Current Player Ship Position: " << world->player->position << endl;
			cout << "Current Player Ship Heading: " << world->player->heading << endl;
			cout << "Current Player Ship Linear Velocity: " << world->player->linearVelocity << endl;
			cout << "Current Player Ship Angular Velocity: " << world->player->angularVelocity << endl << endl;
			
			cout << "Other Player Ship Position: " << world->playerShips[(world->playerIndex + 1) % 2]->position << endl;
			cout << "Other Player Ship Heading: " << world->playerShips[(world->playerIndex + 1) % 2]->heading << endl;
			cout << "Other Player Ship Linear Velocity: " << world->playerShips[(world->playerIndex + 1) % 2]->linearVelocity << endl;
			cout << "Other Player Ship Angular Velocity: " << world->playerShips[(world->playerIndex + 1) % 2]->angularVelocity << endl << endl;

			cout << "Total Planet Count On Scene: " << world->planets.size() << endl << endl;

			cout << "Current NPC count on scene: " << world->NPCShips.size() << endl << endl;

			cout << "Current Blaster beam count on scene: " << world->beams.size() << endl << endl;;

			cout << "Current Camera View Matrix: " << endl << world->camera->view << endl << endl;;

			world->worldState = World::PAUSE;
		}
		world->updateWorld();
	}
	glutPostRedisplay();
	glutTimerFunc(1000.f / 60.f, timer, 0); // 60 FPS
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(750, 750);
	glutCreateWindow("Space Ranger");
	glewExperimental = GL_TRUE;
	glewInit();

	init();
	glutTimerFunc(1000.f / 60.f, timer, 0); // 60 FPS
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}