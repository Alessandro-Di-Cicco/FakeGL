#include "ofApp.h"

#include "Camera.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SimpleShader.h"
#include "CubeGen.h"
#include "OutlineShader.h"
#include "RainbowShader.h"
#include "GLFW/glfw3.h"

int width = 1000;
int height = 800;

void updateInput();
glm::mat4 getPerspective();

Renderer renderer{};
glm::mat4 persp = getPerspective();
SimpleShader shader{ persp, true };
Mesh cube{ generateCube({}, {1, 1, 1}, {255, 255, 255}) };

Mesh pyramid{ generatePyramid({2.5, 0, 0}, {1, 1, 1}, {120, 50, 130}) };

SimpleShader unlit{ persp, false };
Mesh lightMesh{ generateCube({1.25, .5, 1}, glm::vec3{0.2f}, {255, 100, 10})};
Light light{ lightMesh, 2, {255, 100, 10} };

Mesh lightMesh2{ generateCube({1.25, -.5, -1}, glm::vec3{0.2f}, {0, 150, 255}) };
Light light2{ lightMesh2, 2, {0, 150, 255} };

OutlineShader outlineShader{ persp, true, {20, 20, 20} };
RainbowShader rainbowShader{ persp, true };

Mesh lightMesh3{ generateCube({}, {0.15, 0.15, 0.15}, {255, 255, 255}) };
Light light3{ lightMesh3, 0.5, {255, 255, 255} };

Camera cam{ {0, 0, -4} };

float dist{ 2 };

void ofApp::setup(){
	ofSetWindowShape(width, height);

	renderer.setClearColor({25, 255});

	shader.addLight(light);
	shader.addLight(light2);
	shader.addLight(light3);

	outlineShader.addLight(light);
	outlineShader.addLight(light2);
	outlineShader.addLight(light3);

	rainbowShader.addLight(light);
	rainbowShader.addLight(light2);
	rainbowShader.addLight(light3);

	outlineShader.setUniform1fv("minThickness", 0.2f);
	outlineShader.setUniform1fv("maxThickness", 0.4f);

	rainbowShader.setUniform1fv("frequency", 300);

	ofHideCursor();
}

void ofApp::update(){
	updateInput();

	float time = ofGetElapsedTimeMillis() / 1000.0f;
	float sinTime = sin(time);
	float cosTime = cos(time);

	shader.setUniform4fm("view", cam.getMatrix());

	outlineShader.setUniform4fm("view", cam.getMatrix());
	outlineShader.setUniform1fv("sinTime", sinTime);
	
	unlit.setUniform4fm("view", cam.getMatrix());

	rainbowShader.setUniform4fm("view", cam.getMatrix());
	rainbowShader.setUniform1fv("time", time * 50);

	cube.setRotation(glm::vec3{ time, 0, time } * 30.0f);
	pyramid.setRotation(glm::vec3{ 0, 0, time * 20 });

	const float invSin = sin(-time);
	const float invCos = cos(-time);

	lightMesh3.setPosition(pyramid.getPosition() + glm::vec3{ 0,	invSin, invCos });
}

void ofApp::draw(){
	renderer.clearBuffers();

	renderer.setShader(&unlit);
	lightMesh.render(renderer);
	lightMesh2.render(renderer);
	lightMesh3.render(renderer);

	// light2.render(renderer);
	renderer.setShader(&outlineShader);
	cube.render(renderer);

	renderer.setShader(&rainbowShader);
	pyramid.render(renderer);

	// Draw fps counter
	const float limit = min(ofGetWindowWidth(), ofGetWindowHeight());
	renderer.getTexture().draw((ofGetWindowWidth() - limit) / 2, (ofGetWindowHeight() - limit) / 2, limit, limit);
	ofDrawBitmapString(to_string(ofGetFrameRate()), 10, 20);
}

bool firstMove = true;
void ofApp::mouseMoved(int x, int y)
{
	if (firstMove)
	{
		firstMove = false;
		return;
	}

	int xDelta{ x - ofGetPreviousMouseX() };
	int yDelta{ y - ofGetPreviousMouseY() };

	if (xDelta != 0)
		cam.rotate(CameraAxis::Yaw, -xDelta);
	if (yDelta != 0)
		cam.rotate(CameraAxis::Pitch, yDelta);
}


void ofApp::keyPressed(ofKeyEventArgs& key)
{
	if (key.keycode == 'F' && !key.isRepeat)
		ofToggleFullscreen();
}


void updateInput()
{
	static GLFWwindow* window{ glfwGetCurrentContext() };

	static std::map<int, CameraDir> mappings{
		{GLFW_KEY_W, CameraDir::Forward},
		{GLFW_KEY_S, CameraDir::Back},
		{GLFW_KEY_D, CameraDir::Right},
		{GLFW_KEY_A, CameraDir::Left},
		{GLFW_KEY_SPACE, CameraDir::Up},
		{GLFW_KEY_LEFT_SHIFT, CameraDir::Down}
	};

	for (const auto& kvp : mappings)
	{
		if (glfwGetKey(window, kvp.first) == GLFW_PRESS)
			cam.move(kvp.second);
	}

	const float amount = 80.0f * ofGetLastFrameTime();

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cube.getRotation()  += glm::vec3{1, 0, 0} *amount;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cube.getRotation() -= glm::vec3{1, 0, 0} *amount;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cube.getRotation() += glm::vec3{0, 0, -1} *amount;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cube.getRotation() -= glm::vec3{0, 0, -1} *amount;
	if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
		cube.getRotation() += glm::vec3{0, 1, 0} *amount;
	if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS)
		cube.getRotation() -= glm::vec3{0, 1, 0} *amount;
}

glm::mat4 getPerspective()
{
	return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
}