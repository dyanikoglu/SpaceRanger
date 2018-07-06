#include "World.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


World* world;

std::string readFile(const char *filePath) {
  std::string content;
  std::ifstream fileStream(filePath, std::ios::in);

  if (!fileStream.is_open()) {
    std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
    return "";
  }

  std::string line = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }

  fileStream.close();
  return content;
}


GLuint LoadShader(const char *vertex_path, const char *fragment_path) {
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Read shaders
  std::string vertShaderStr = readFile(vertex_path);
  std::string fragShaderStr = readFile(fragment_path);
  const char *vertShaderSrc = vertShaderStr.c_str();
  const char *fragShaderSrc = fragShaderStr.c_str();

  GLint result = GL_FALSE;
  int logLength;

  // Compile vertex shader
  std::cout << "Compiling vertex shader." << std::endl;
  glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
  glCompileShader(vertShader);
  // Check vertex shader
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
  std::cout << &vertShaderError[0] << std::endl;

  // Compile fragment shader
  std::cout << "Compiling fragment shader." << std::endl;
  glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
  glCompileShader(fragShader);

  // Check fragment shader
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
  std::cout << &fragShaderError[0] << std::endl;

  std::cout << "Linking program" << std::endl;
  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<char> programError((logLength > 1) ? logLength : 1);
  glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
  std::cout << &programError[0] << std::endl;

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  return program;
}

void init()
{
	srand(GetTickCount());

	GLuint program = LoadShader("./source/vshader.glsl", "./source/fshader.glsl");
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
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}