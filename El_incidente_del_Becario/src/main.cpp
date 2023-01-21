#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// OpenAL include
#include <AL/alut.h>

//Shadowbox include
#include "Headers/ShadowBox.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para dibujar una textura
Shader shaderTextura;

//Shadow box object
ShadowBox * shadowBox;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;
Box boxIntro;

// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;
// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLampPost2;
// Hierba
Model modelGrass;
// Fountain
Model modelFountain;
// Models of scene
Model buildingTall, buildingWide, buildingSmall;
Model Factory;
Model goop, bubble;
Model fanAnimated;
Model tree;
Model wallNorthSouth, wallEastWest;
Model pipes, grassSpots;

// Model animate instance
// becaria
Model becariaModelAnimate;
// Terrain model instance
Terrain terrain(-1, -1, 200, 8, "../Textures/heightMapBecario2.png");

GLuint textureCespedID, textureTitlePlayID, textureTitleCreditsID, textureTitleExitID, textureCreditsID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;
GLuint texturaActivaID;

bool iniciaPartida = false, presionaOpcion = false;
int titleOpt=0;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

/*std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };*/
		// Agregamos las nuevas texturas del skybox.
// Skybox del juego
std::string fileNames[6] = { "../Textures/SwedishRoyalCastle/negz.jpg", // negz corresponde a la parte frontal del skybox
		"../Textures/SwedishRoyalCastle/posz.jpg",						// posz corresponde a la parte trasera del skybox
		"../Textures/SwedishRoyalCastle/posy.jpg",						// posy corresponde a la parte de arriba del skybox
		"../Textures/SwedishRoyalCastle/negy.jpg",						// negy corresponde a la parte de abajo del skybox
		"../Textures/SwedishRoyalCastle/posx.jpg",						// posx corresponde a la parte derecha del skybox
		"../Textures/SwedishRoyalCastle/negx.jpg" };					// negx corresponde a la parte izquierda del skybox

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixbecaria = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);

glm::mat4 modelMatrixBtall = glm::mat4(1.0f);
glm::mat4 modelMatrixBwide = glm::mat4(1.0f);
glm::mat4 modelMatrixBsmall = glm::mat4(1.0f);
glm::mat4 modelMatrixFactory = glm::mat4(1.0f);
glm::mat4 modelMatrixGoop = glm::mat4(1.0f);
glm::mat4 modelMatrixBubble = glm::mat4(1.0f);
glm::mat4 modelMatrixTree = glm::mat4(1.0f);
glm::mat4 modelMatrixWallN = glm::mat4(1.0f);
glm::mat4 modelMatrixWallW = glm::mat4(1.0f);
glm::mat4 modelMatrixPipes = glm::mat4(1.0f);
glm::mat4 modelMatrixGrassSpot = glm::mat4(1.0f);

int animationIndex = 3;
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 2;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Var animate helicopter
float rotHelHelY = 0.0;

// Var animate lambo dor
int stateDoor = 0;
float dorRotCount = 0.0;

// buildingTall
/*std::vector<glm::vec3> bTallPosition = { glm::vec3(-40, 0, 35.54), glm::vec3(
		-34.96, 0, 35.54), glm::vec3(-28.90, 0, 35.54),
		glm::vec3(-40.03, 0, 41.79), glm::vec3(-34.96, 0, 41.79),
		glm::vec3(-28.90, 0, 41.79), glm::vec3(-26.56, 0, -7.22),
		glm::vec3(-3.32, 0, -27.73), glm::vec3(8.39, 0, 15.42),
		glm::vec3(35.93, 0, -3.125), glm::vec3(35.93, 0, 4.29),
		glm::vec3(42.57, 0, 41.21), glm::vec3(49.80, 0, 41.21),
		glm::vec3(42.57, 0, 48.04), glm::vec3(49.8, 0, 48.04) };
std::vector<float> bTallOrientation = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };*/
// Buildings positions and orientation
std::vector<glm::vec3> positionBuildingTalls = { glm::vec3(-40.0390625f, 0.0f, 35.546875f),	// Edificio alto 1
												 glm::vec3(-34.9609375f, 0.0f, 35.546875f),	// Edificio alto 2
												 glm::vec3(-28.90625f, 0.0f, 35.546875f),	// Edificio alto 3
												 glm::vec3(-40.0390625f, 0.0f, 41.796875f),	// Edificio alto 4
												 glm::vec3(-34.9609375f, 0.0f, 41.796875f),	// Edificio alto 5
												 glm::vec3(-28.90625f, 0.0f, 41.796875f),	// Edificio alto 6
												 glm::vec3(-26.5625f, 0.0f, -7.2265625f),	// Edificio alto 7
												 glm::vec3(-3.3203125f, 0.0f, -27.734375f),	// Edificio alto 8
												 glm::vec3(8.3984375f, 0.0f, 15.4296875f),	// Edificio alto 9
												 glm::vec3(35.9375f, 0.0f, -3.125f),		// Edificio alto 10
												 glm::vec3(35.9375f, 0.0f, 4.296875f),		// Edificio alto 11
												 glm::vec3(42.578125f, 0.0f, 41.2109375f),	// Edificio alto 12
												 glm::vec3(49.8046875f, 0.0f, 41.2109375f),	// Edificio alto 13
												 glm::vec3(42.578125f, 0.0f, 48.046875f),	// Edificio alto 14
												 glm::vec3(49.8046875f, 0.0f, 48.046875f) };	// Edificio alto 15

std::vector<glm::vec3> positionBuildingSmalls = { glm::vec3(-40.0390625f, 0.0f, 26.171875f),	// Edificio bajo 1
												  glm::vec3(-15.4296875f, 0.0f, 7.2265625f),	// Edificio bajo 2
												  glm::vec3(-22.8515625f, 0.0f, -20.8984375f),	// Edificio bajo 3
												  glm::vec3(12.890625f, 0.0f, -47.0703125f),	// Edificio bajo 4
												  glm::vec3(18.1640625f, 0.0f, -2.34375f),		// Edificio bajo 5
												  glm::vec3(54.8828125f, 0.0f, 24.609375f) };	// Edificio bajo 6

std::vector<glm::vec3> positionBuildingWides = { glm::vec3(-56.4453125f, 0.0f, 21.484375f),	// Edificio ancho 1
												 glm::vec3(-42.578125f, 0.0f, -7.421875f),	// Edificio ancho 2
												 glm::vec3(-15.625f, 0.0f, -45.8984375f),	// Edificio ancho 3
												 glm::vec3(6.25f, 0.0f, 23.046875f),		// Edificio ancho 4
												 glm::vec3(24.21875f, 0.0f, -58.203125f),	// Edificio ancho 5
												 glm::vec3(45.5078125f, 0.0f, 57.8125f) };	// Edificio ancho 6

std::vector<float> orientationBuildingWides = { 0.0f, 0.0f, 0.0f , 90.0f, 90.0f, 90.0f };	// Edificios anchos 1, 2, ... , 6. Eje Y

std::vector<glm::vec3> positionFactories = { glm::vec3(-44.3359375f, 0.0f, -37.109375f),	// Edificio fabrica 1
											 glm::vec3(41.9921875f, 0.0f, -25.9765625f),	// Edificio fabrica 2
											 glm::vec3(14.2578125f, 0.0f, 43.9453125f) };	// Edificio fabrica 3

std::vector<glm::vec3> positionFanAnimateds = { glm::vec3(-56.4453125f, 0.0f, 38.8671875f),		// Fan animado 1
												glm::vec3(-54.4921875f, 0.0f, -13.8671875f),	// Fan animado 2
												glm::vec3(38.8671875f, 0.0f, -58.7890625f),		// Fan animado 3
												glm::vec3(25.0f, 0.0f, -2.734375f),				// Fan animado 4
												glm::vec3(36.9140625f, 0.0f, 35.15625f) };		// Fan animado 5

std::vector<float> orientationFanAnimateds = { 0.0f, -59.0f, -122.0f , 180.0f, 106.0f, };	// Fans animados 1, 2, ... , 6. Eje Y

std::vector<glm::vec3> positionGoopPuddles = { glm::vec3(-54.1015625f, 0.0f, 32.6171875f),	// Charco 1
											   glm::vec3(-59.375f, 0.0f, -1.3671875f),		// Charco 2
											   glm::vec3(-29.6875f, 0.0f, 20.5078125f),		// Charco 3
											   glm::vec3(-17.96875f, 0.0f, 49.609375f),		// Charco 4
											   glm::vec3(-25.9765625f, 0.0f, -15.8203125f),	// Charco 5
											   glm::vec3(-4.4921875f, 0.0f, 10.3515625f),	// Charco 6
											   glm::vec3(-51.953125f, 0.0f, -50.78125f),	// Charco 7
											   glm::vec3(-18.75f, 0.0f, -44.140625f),		// Charco 8
											   glm::vec3(4.1015625f, 0.0f, -31.4453125f),	// Charco 9
											   glm::vec3(23.046875f, 0.0f, -49.0234375f),	// Charco 10
											   glm::vec3(19.140625f, 0.0f, -1.953125f),		// Charco 11
											   glm::vec3(28.7109375f, 0.0f, 23.6328125f),	// Charco 12
											   glm::vec3(51.5625f, 0.0f, 43.75f),			// Charco 13
											   glm::vec3(59.375f, 0.0f, 0.0f),				// Charco 14
											   glm::vec3(53.90625f, 0.0f, -48.828125f) };	// Charco 15

std::vector<float> orientationGoopPuddles = { 0.0f, 32.0f, -25.0f, -68.0f, -76.0f, -60.0f, -60.0f, -60.0f, -76.0f, -41.0f,
											  0.0f, -76.0f, -76.0f, 0.0f, 0.0f };	// Charcos 1, 2, 3, ... , 15. Eje Y

std::vector<glm::vec3> positionTrees = { glm::vec3(-61.328125f, 0.0f, -4.4921875f),		// Arbol 1
										 glm::vec3(-54.296875f, 0.0f, 54.4921875f),		// Arbol 2
										 glm::vec3(-31.640625f, 0.0f, -2.734375f),		// Arbol 3
										 glm::vec3(-13.8671875f, 0.0f, -55.2734375f),	// Arbol 4
										 glm::vec3(-21.484375f, 0.0f, -27.1484375f),	// Arbol 5
										 glm::vec3(-20.703125f, 0.0f, 13.4765625f),		// Arbol 6
										 glm::vec3(-22.8515625f, 0.0f, 35.9375f),		// Arbol 7
										 glm::vec3(-15.0390625f, 0.0f, 45.3125f),		// Arbol 8
										 glm::vec3(-2.1484375f, 0.0f, 21.484375f),		// Arbol 9
										 glm::vec3(-5.859375f, 0.0f, -5.2734375f),		// Arbol 10
										 glm::vec3(2.9296875f, 0.0f, -25.0f),		// Arbol 11
										 glm::vec3(7.421875f, 0.0f, -13.8671875f),	// Arbol 12
										 glm::vec3(17.1875f, 0.0f, 16.015625f),		// Arbol 13
										 glm::vec3(39.0625f, 0.0f, 18.1640625f),	// Arbol 14
										 glm::vec3(44.53125f, 0.0f, -0.5859375f),	// Arbol 15
										 glm::vec3(57.421875f, 0.0f, 45.703125f),	// Arbol 16
										 glm::vec3(30.2734375f, 0.0f, -50.1953125f) };	// Arbol 17

std::vector<glm::vec3> positionWallEastWests = { glm::vec3(-70.5078125f, 0.0f, 0.1953125f),		// Barda oeste
												 glm::vec3(70.8984375f, 0.0f, 0.1953125f) };	// Barda este

std::vector<glm::vec3> positionWallNorthSouths = { glm::vec3(0.1953125f, 0.0f, -69.7265625f),	// Barda norte
												   glm::vec3(0.1953125f, 0.0f, 69.7265625f) };	// Barda sur

// Variable para obtener medidas de escala y rotacion.
float varUpdate = 2.0f;
// bool buttomIsPress = false;
// Variables para el control de la neblina
float densityValue = 0.008;
float gradientValue = 1.5;
float lowerLimitValue = 0.0;
float upperLimitValue = 0.2;
float fogValue = 0.5;

std::vector<glm::vec3> lamp2Position = { glm::vec3(-36.52, 0, -23.24),
		glm::vec3(-52.73, 0, -3.90) };
std::vector<float> lamp2Orientation = {21.37 + 90, -65.0 + 90};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"aircraft", glm::vec3(10.0, 0.0, -17.5)},
		{"lambo", glm::vec3(23.0, 0.0, 0.0)},
		{"heli", glm::vec3(5.0, 10.0, -5.0)},
		{"fountain", glm::vec3(5.0, 0.0, -40.0)},
		{"fire", glm::vec3(0.0, 0.0, 7.0)}
};

double deltaTime;
double currTime, lastTime;

// Jump variables
bool isJump = false;
float GRAVITY = 1.81;
double tmv = 0;
double startTimeJump = 0;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

// OpenAL Defines
#define NUM_BUFFERS 4
#define NUM_SOURCES 4
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true, true};

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for(unsigned int i = 0; i < nParticlesFire; i++ ) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine {};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", {"Position", "Velocity", "Age"});
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");
	shaderTextura.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxIntro.init();
	boxIntro.setShader(&shaderTextura);
	boxIntro.setScale(glm::vec3(2.0f, 2.0f, 1.0f));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1.loadModel("../models/Street-Lamp-Black/objLamp.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Street_Light/Lamp.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.loadModel("../models/Street_Light/LampPost.obj");
	modelLampPost2.setShader(&shaderMulLighting);

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/fountain/fountain.obj");
	modelFountain.setShader(&shaderMulLighting);

	//becaria
	becariaModelAnimate.loadModel("../models/projectModels/mainCharacter.fbx");
	becariaModelAnimate.setShader(&shaderMulLighting);

	//Buildings
	buildingTall.loadModel("../models/projectModels/buildingTall.fbx");
	buildingTall.setShader(&shaderMulLighting);
	buildingSmall.loadModel("../models/projectModels/buildingSmall.fbx");
	buildingSmall.setShader(&shaderMulLighting);
	buildingWide.loadModel("../models/projectModels/buildingWide.fbx");
	buildingWide.setShader(&shaderMulLighting);
	Factory.loadModel("../models/projectModels/FactoryOneTex.fbx");
	Factory.setShader(&shaderMulLighting);
	fanAnimated.loadModel("../models/projectModels/fanAnimated.fbx");
	fanAnimated.setShader(&shaderMulLighting);
	goop.loadModel("../models/projectModels/GoopPuddle.fbx");
	goop.setShader(&shaderMulLighting);
	tree.loadModel("../models/projectModels/tree.fbx");
	tree.setShader(&shaderMulLighting);
	wallEastWest.loadModel("../models/projectModels/WallEastWest.fbx");
	wallEastWest.setShader(&shaderMulLighting);
	wallNorthSouth.loadModel("../models/projectModels/WallNorthSouth.fbx");
	wallNorthSouth.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/cesped.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCesped.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCesped.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTitlePlay("../Textures/Title_jugar.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTitlePlay.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTitlePlay.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTitlePlayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTitlePlayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTitlePlay.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTitleCredits("../Textures/Title_creditos.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTitleCredits.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTitleCredits.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTitleCreditsID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTitleCreditsID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTitleCredits.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTitleExit("../Textures/Title_salir.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTitleExit.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTitleExit.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTitleExitID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTitleExitID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTitleExit.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureCredits("../Textures/Creditos.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCredits.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCredits.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCreditsID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCreditsID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCredits.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/backgroundGrava.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/metal.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/verdeAcido.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/caminoAsfalto.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/BlendMapBecarioV1.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureParticlesFountain("../Textures/bluewater.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for( int i = 0; i < randData.size(); i++ ) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f,0.1f,0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0,1,0);
	n = glm::cross(glm::vec3(1,0,0), v);
	if( glm::length(n) < 0.00001f ) {
		n = glm::cross(glm::vec3(0,1,0), v);
	}
	u = glm::cross(v,n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/sonidoCarro.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 3.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.3f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_FALSE);
	alSourcef(source[3], AL_MAX_DISTANCE, 500);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();
	shaderViewDepth.destroy();
	shaderDepth.destroy();
	shaderTextura.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLampPost2.destroy();
	modelGrass.destroy();
	modelFountain.destroy();

	// Custom objects animate
	becariaModelAnimate.destroy();

	// Buildings of scene
	buildingTall.destroy();
	buildingSmall.destroy();
	buildingWide.destroy();
	Factory.destroy();
	fanAnimated.destroy();
	goop.destroy();
	tree.destroy();
	wallEastWest.destroy();
	wallNorthSouth.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureTitlePlayID);
	glDeleteTextures(1, &textureTitleCreditsID);
	glDeleteTextures(1, &textureTitleExitID);
	glDeleteTextures(1, &textureCreditsID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (!iniciaPartida) {
		bool statusEnter = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
		if (titleOpt == 0 && statusEnter) {
			iniciaPartida = true;
		}
		else if (titleOpt == 1 && statusEnter) {
			texturaActivaID = textureCreditsID;
		}
		bool upPressed = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
		bool downPressed = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
		if (!presionaOpcion && (upPressed || downPressed) ) {
			presionaOpcion = true;
			
			if (downPressed) {
				titleOpt += 1;
				if (titleOpt > 2)
					titleOpt = 0;
			}
			else if (upPressed) {
				titleOpt -= 1;
				if (titleOpt < 0)
					titleOpt = 2;
			}
			if (titleOpt==0)
				texturaActivaID = textureTitlePlayID;
			else if (titleOpt == 1)
				texturaActivaID = textureTitleCreditsID;
			else if (titleOpt == 2)
				texturaActivaID = textureTitleExitID;
		}
		else if ( (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && upPressed) || 
			(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && downPressed))
		{
			presionaOpcion = false;
			upPressed = false;
			downPressed = false;
		}
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	/*if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 2)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;*/

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));

	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		modelMatrixbecaria = glm::rotate(modelMatrixbecaria, glm::radians(9.0f), glm::vec3(0, 1, 0));
		animationIndex = 4;
	}else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		modelMatrixbecaria = glm::rotate(modelMatrixbecaria, glm::radians(-9.0f), glm::vec3(0, 1, 0));
		animationIndex = 4;
	}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		modelMatrixbecaria = glm::translate(modelMatrixbecaria, glm::vec3(0, 0, 1.02));
		animationIndex = 4;
	}else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		modelMatrixbecaria = glm::translate(modelMatrixbecaria, glm::vec3(0, 0, -1.02));
		animationIndex = 4;
	}

	/*if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		// buttomIsPress = true;
		varUpdate += 0.1f;
	}
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		// buttomIsPress = true;
		varUpdate -= 0.1f;
	}*/
	/*else
		buttomIsPress = false;*/
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		// std::cout << "scaleFactory = " << varUpdate << std::endl;
		// std::cout << "Density de niebla= " << densityValue << std::endl;
		// std::cout << "Gradiente de niebla= " << gradientValue << std::endl;
		std::cout << "valor minimo de la niebla = " << lowerLimitValue << std::endl;
		std::cout << "valor maximo de la niebla = " << upperLimitValue << std::endl;
		std::cout << "nivel de color R y B = " << fogValue << std::endl;
	}
	/*if (varUpdate >= 360.0f)
		varUpdate = 0.0f;*/
	
	// Botones para controlar los parametros de la neblina
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		densityValue += 0.001f;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		densityValue -= 0.001f;

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		gradientValue += 0.1f;
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		gradientValue -= 0.1f;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		fogValue += 0.01f;
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		fogValue -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
		lowerLimitValue += 0.1f;
		upperLimitValue += 0.1f;
	}
	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		lowerLimitValue -= 0.1f;
		upperLimitValue -= 0.1f;
	}

	bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if(!isJump && keySpaceStatus){
		isJump = true;
		startTimeJump = currTime;
		tmv = 0;
		sourcesPlay[3] = true;
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixbecaria = glm::translate(modelMatrixbecaria, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixbecaria = glm::rotate(modelMatrixbecaria, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixFountain = glm::translate(modelMatrixFountain, glm::vec3(5.0, 0.0, -40.0));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(modelMatrixFountain[3][0] , modelMatrixFountain[3][2]) + 0.2;
	modelMatrixFountain = glm::scale(modelMatrixFountain, glm::vec3(10.0f, 10.0f, 10.0f));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);
	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);
	texturaActivaID = textureTitlePlayID;

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		if(modelSelected == 1){
			axis = glm::axis(glm::quat_cast(modelMatrixDart));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixDart));
			target = modelMatrixDart[3];
		}
		else{
			axis = glm::axis(glm::quat_cast(modelMatrixbecaria));
			//axis[1] = +2;
			angleTarget = glm::angle(glm::quat_cast(modelMatrixbecaria));
			target = modelMatrixbecaria[3];
			target[1] = + 0.5f;
		}

		if(std::isnan(angleTarget))
			angleTarget = 0.0;
		if(axis.y < 0)
			angleTarget = -angleTarget;
		if(modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		// Matriz de proyección del shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;                      //float near_plane = 0.1f, far_plane = 20.0f;
		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 boxPosition = shadowBox->getCenter();
		lightView = glm::lookAt(boxPosition, boxPosition + glm::normalize(-lightPos), 
			glm::vec3(0.0, 1.0, 0.0));
		lightProjection[0][0] = 2.0 / shadowBox->getWidth();
		lightProjection[1][1] = 2.0 / shadowBox->getHeight();
		lightProjection[2][2] = -2.0 / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*************************************************************
		*	Propiedades de la neblina
		**************************************************************/
		shaderMulLighting.setFloat("density", 0.06);
		shaderMulLighting.setFloat("gradient", 0.6);
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.34f, 1.0f, 0.34f)));

		shaderTerrain.setFloat("density", 0.06);
		shaderTerrain.setFloat("gradient", 0.6);
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.34f, 1.0f, 0.34f)));

		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.34f, 1.0f, 0.34f)));
		shaderSkybox.setFloat("lowerLimit", 0.2);
		shaderSkybox.setFloat("upperLimit", 0.4);

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.32437, 0.226053, 1.79149, 1.0));
		shaderMulLighting.setInt("spotLightCount", 1);
		shaderTerrain.setInt("spotLightCount", 1);
		shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		shaderMulLighting.setFloat("spotLights[0].linear", 0.074);
		shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		shaderTerrain.setFloat("spotLights[0].linear", 0.074);
		shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		/*shaderMulLighting.setInt("pointLightCount", bTallPosition.size() + lamp2Orientation.size());
		shaderTerrain.setInt("pointLightCount", bTallPosition.size() + lamp2Orientation.size());
		for (int i = 0; i < bTallPosition.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, bTallPosition[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(bTallOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5, 0.5, 0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		for (int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(bTallPosition.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(bTallPosition.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(bTallPosition.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(bTallPosition.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(bTallPosition.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(bTallPosition.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(bTallPosition.size() + i) + "].quadratic", 0.02);
		}*/

		if (!iniciaPartida) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, screenWidth, screenHeight);
			shaderTextura.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0f)));
			shaderTextura.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0f)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturaActivaID);
			boxIntro.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render Depth map to quad for visual debugging
		shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		/*glm::vec3 front = glm::normalize(-lightPos);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		glm::vec3 up = glm::normalize(glm::cross(front, right));
		glDisable(GL_CULL_FACE);
		glm::mat4 boxViewTransform = glm::mat4(1.0f);
		boxViewTransform = glm::translate(boxViewTransform, centerBox);
		boxViewTransform[0] = glm::vec4(right, 0.0);
		boxViewTransform[1] = glm::vec4(up, 0.0);
		boxViewTransform[2] = glm::vec4(front, 0.0);
		boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		boxLightViewBox.enableWireMode();
		boxLightViewBox.render(boxViewTransform);
		glEnable(GL_CULL_FACE);*/

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/
		// Collider del dart vader lego
		//glm::mat4 modelmatrixColliderDart = glm::mat4(modelMatrixDart);
		//AbstractModel::OBB dartLegoBodyCollider;
		//// Set the orientation of collider before doing the scale
		//dartLegoBodyCollider.u = glm::quat_cast(modelMatrixDart);
		//modelmatrixColliderDart = glm::scale(modelmatrixColliderDart, glm::vec3(0.5, 0.5, 0.5));
		//modelmatrixColliderDart = glm::translate(modelmatrixColliderDart,
		//		glm::vec3(modelDartLegoBody.getObb().c.x,
		//				modelDartLegoBody.getObb().c.y,
		//				modelDartLegoBody.getObb().c.z));
		//dartLegoBodyCollider.c = glm::vec3(modelmatrixColliderDart[3]);
		//dartLegoBodyCollider.e = modelDartLegoBody.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		//addOrUpdateColliders(collidersOBB, "dart", dartLegoBodyCollider, modelMatrixDart);

		// Collider del aricraft
		//glm::mat4 modelMatrixColliderAircraft = glm::mat4(modelMatrixAircraft);
		//AbstractModel::OBB aircraftCollider;
		//// Set the orientation of collider before doing the scale
		//aircraftCollider.u = glm::quat_cast(modelMatrixAircraft);
		//modelMatrixColliderAircraft = glm::scale(modelMatrixColliderAircraft,
		//		glm::vec3(1.0, 1.0, 1.0));
		//modelMatrixColliderAircraft = glm::translate(
		//		modelMatrixColliderAircraft, modelAircraft.getObb().c);
		//aircraftCollider.c = glm::vec3(modelMatrixColliderAircraft[3]);
		//aircraftCollider.e = modelAircraft.getObb().e * glm::vec3(1.0, 1.0, 1.0);
		//addOrUpdateColliders(collidersOBB, "aircraft", aircraftCollider, modelMatrixAircraft);

		//Collider del la rock
		/*AbstractModel::SBB rockCollider;
		glm::mat4 modelMatrixColliderRock= glm::mat4(matrixModelRock);
		modelMatrixColliderRock = glm::scale(modelMatrixColliderRock, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderRock = glm::translate(modelMatrixColliderRock, modelRock.getSbb().c);
		rockCollider.c = glm::vec3(modelMatrixColliderRock[3]);
		rockCollider.ratio = modelRock.getSbb().ratio * 1.0;
		addOrUpdateColliders(collidersSBB, "rock", rockCollider, matrixModelRock);*/

		// Lamps1 colliders
		for (int i = 0; i < positionBuildingTalls.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, positionBuildingTalls[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(-90.0f),
					glm::vec3(1, 0, 0));
			/*modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(bTallOrientation[i]),
					glm::vec3(0, 1, 0));*/
			addOrUpdateColliders(collidersOBB, "bTall-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(2.0, 2.0, 4.0));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, buildingTall.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = buildingTall.getObb().e * glm::vec3(2.0, 2.0, 4.0);
			std::get<0>(collidersOBB.find("bTall-" + std::to_string(i))->second) = lampCollider;
		}

		// Lamps2 colliders
		for (int i = 0; i < lamp2Position.size(); i++){
			AbstractModel::OBB lampCollider;
			glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
			modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp2Orientation[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
			// Set the orientation of collider before doing the scale
			lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
			modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 1.0, 1.0));
			modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLampPost2.getObb().c);
			lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
			lampCollider.e = modelLampPost2.getObb().e * glm::vec3(1.0, 1.0, 1.0);
			std::get<0>(collidersOBB.find("lamp2-" + std::to_string(i))->second) = lampCollider;
		}

		// Collider de becaria
		AbstractModel::OBB becariaCollider;
		glm::mat4 modelmatrixColliderbecaria = glm::mat4(modelMatrixbecaria);
		modelmatrixColliderbecaria = glm::rotate(modelmatrixColliderbecaria,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		becariaCollider.u = glm::quat_cast(modelmatrixColliderbecaria);
		modelmatrixColliderbecaria = glm::scale(modelmatrixColliderbecaria, glm::vec3(0.06, 0.06, 0.06));
		modelmatrixColliderbecaria = glm::translate(modelmatrixColliderbecaria,
				glm::vec3(becariaModelAnimate.getObb().c.x,
						becariaModelAnimate.getObb().c.y,
						becariaModelAnimate.getObb().c.z + 5));
		becariaCollider.e = becariaModelAnimate.getObb().e * glm::vec3(0.06, 0.06, 0.06) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
		becariaCollider.c = glm::vec3(modelmatrixColliderbecaria[3]);
		addOrUpdateColliders(collidersOBB, "becaria", becariaCollider, modelMatrixbecaria);

		//Collider builgingWide
		for (int i = 0; i < positionBuildingWides.size(); i++) {
			AbstractModel::OBB bWideCollider;
			glm::mat4 modelMatrixColliderbWide = glm::mat4(1.0);
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, positionBuildingWides[i]);
			modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(-90.0f),
				glm::vec3(1, 0, 0));
			/*modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(bTallOrientation[i]),
					glm::vec3(0, 1, 0));*/
			addOrUpdateColliders(collidersOBB, "bWide-" + std::to_string(i), bWideCollider, modelMatrixColliderbWide);
			// Set the orientation of collider before doing the scale
			bWideCollider.u = glm::quat_cast(modelMatrixColliderbWide);
			modelMatrixColliderbWide = glm::scale(modelMatrixColliderbWide, glm::vec3(2.0, 2.0, 2.0));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, buildingWide.getObb().c);
			bWideCollider.c = glm::vec3(modelMatrixColliderbWide[3]);
			bWideCollider.e = buildingWide.getObb().e * glm::vec3(2.0, 2.0, 2.0);
			std::get<0>(collidersOBB.find("bWide-" + std::to_string(i))->second) = bWideCollider;
		}
		//Collider buildingSmall
		for (int i = 0; i < positionBuildingSmalls.size(); i++) {
			AbstractModel::OBB bSmallCollider;
			glm::mat4 modelMatrixColliderbWide = glm::mat4(1.0);
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, positionBuildingSmalls[i]);
			modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(-90.0f),
				glm::vec3(1, 0, 0));
			/*modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(bTallOrientation[i]),
					glm::vec3(0, 1, 0));*/
			addOrUpdateColliders(collidersOBB, "bSmall-" + std::to_string(i), bSmallCollider, modelMatrixColliderbWide);
			// Set the orientation of collider before doing the scale
			bSmallCollider.u = glm::quat_cast(modelMatrixColliderbWide);
			modelMatrixColliderbWide = glm::scale(modelMatrixColliderbWide, glm::vec3(2.0, 2.0, 2.5));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, glm::vec3(0.0, 0.0, -1.0));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, buildingSmall.getObb().c);
			bSmallCollider.c = glm::vec3(modelMatrixColliderbWide[3]);
			bSmallCollider.e = buildingWide.getObb().e * glm::vec3(2.0, 2.0, 2.5);
			std::get<0>(collidersOBB.find("bSmall-" + std::to_string(i))->second) = bSmallCollider;
		}
		//Collider paredes
		for (int i = 0; i < positionWallEastWests.size(); i++) {
			AbstractModel::OBB bSmallCollider;
			glm::mat4 modelMatrixColliderbWide = glm::mat4(1.0);
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, positionWallEastWests[i]);
			modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(-90.0f),
				glm::vec3(1, 0, 0));
			/*modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(bTallOrientation[i]),
					glm::vec3(0, 1, 0));*/
			addOrUpdateColliders(collidersOBB, "wallWE-" + std::to_string(i), bSmallCollider, modelMatrixColliderbWide);
			// Set the orientation of collider before doing the scale
			bSmallCollider.u = glm::quat_cast(modelMatrixColliderbWide);
			modelMatrixColliderbWide = glm::scale(modelMatrixColliderbWide, glm::vec3(0.25f, 0.25f, 0.25f));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, wallEastWest.getObb().c);
			bSmallCollider.c = glm::vec3(modelMatrixColliderbWide[3]);
			bSmallCollider.e = wallEastWest.getObb().e * glm::vec3(0.25f, 0.25f, 0.25f);
			std::get<0>(collidersOBB.find("wallWE-" + std::to_string(i))->second) = bSmallCollider;
		}
		for (int i = 0; i < positionWallNorthSouths.size(); i++) {
			AbstractModel::OBB bSmallCollider;
			glm::mat4 modelMatrixColliderbWide = glm::mat4(1.0);
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, positionWallNorthSouths[i]);
			modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(-90.0f),
				glm::vec3(1, 0, 0));
			/*modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(bTallOrientation[i]),
					glm::vec3(0, 1, 0));*/
			addOrUpdateColliders(collidersOBB, "wallNS-" + std::to_string(i), bSmallCollider, modelMatrixColliderbWide);
			// Set the orientation of collider before doing the scale
			bSmallCollider.u = glm::quat_cast(modelMatrixColliderbWide);
			modelMatrixColliderbWide = glm::scale(modelMatrixColliderbWide, glm::vec3(0.25f, 0.25f, 0.25f));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, wallNorthSouth.getObb().c);
			bSmallCollider.c = glm::vec3(modelMatrixColliderbWide[3]);
			bSmallCollider.e = wallNorthSouth.getObb().e * glm::vec3(0.25f, 0.25f, 0.25f);
			std::get<0>(collidersOBB.find("wallNS-" + std::to_string(i))->second) = bSmallCollider;
		}
		//Collider factory
		for (int i = 0; i < positionFactories.size(); i++) {
			AbstractModel::OBB bSmallCollider;
			glm::mat4 modelMatrixColliderbWide = glm::mat4(1.0);
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, positionFactories[i]);
			modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(-90.0f),
				glm::vec3(1, 0, 0));
			/*modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(bTallOrientation[i]),
					glm::vec3(0, 1, 0));*/
			addOrUpdateColliders(collidersOBB, "factory-" + std::to_string(i), bSmallCollider, modelMatrixColliderbWide);
			// Set the orientation of collider before doing the scale
			bSmallCollider.u = glm::quat_cast(modelMatrixColliderbWide);
			modelMatrixColliderbWide = glm::scale(modelMatrixColliderbWide, glm::vec3(1.16f, 1.16f, 6.53f));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, Factory.getObb().c);
			bSmallCollider.c = glm::vec3(modelMatrixColliderbWide[3]);
			bSmallCollider.e = Factory.getObb().e * glm::vec3(1.16f, 1.16f, 6.53f);
			std::get<0>(collidersOBB.find("factory-" + std::to_string(i))->second) = bSmallCollider;
		}
		//Collider Fan
		for (int i = 0; i < positionFanAnimateds.size(); i++) {
			AbstractModel::OBB bSmallCollider;
			glm::mat4 modelMatrixColliderbWide = glm::mat4(1.0);
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, positionFanAnimateds[i]);
			//modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(-90.0f),
			//	glm::vec3(1, 0, 0));
			modelMatrixColliderbWide = glm::rotate(modelMatrixColliderbWide, glm::radians(orientationFanAnimateds[i]),
					glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "fan-" + std::to_string(i), bSmallCollider, modelMatrixColliderbWide);
			// Set the orientation of collider before doing the scale
			bSmallCollider.u = glm::quat_cast(modelMatrixColliderbWide);
			modelMatrixColliderbWide = glm::scale(modelMatrixColliderbWide, glm::vec3(0.5f, 0.5f, 0.5f));
			modelMatrixColliderbWide = glm::translate(modelMatrixColliderbWide, fanAnimated.getObb().c);
			bSmallCollider.c = glm::vec3(modelMatrixColliderbWide[3]);
			bSmallCollider.e = fanAnimated.getObb().e * glm::vec3(0.5f, 0.5f, 0.5f);
			std::get<0>(collidersOBB.find("fan-" + std::to_string(i))->second) = bSmallCollider;
		}

		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}

		// Esto es para ilustrar la transformacion inversa de los coliders
		/*glm::vec3 cinv = glm::inverse(becariaCollider.u) * glm::vec4(rockCollider.c, 1.0);
		glm::mat4 invColliderS = glm::mat4(1.0);
		invColliderS = glm::translate(invColliderS, cinv);
		invColliderS =  invColliderS * glm::mat4(becariaCollider.u);
		invColliderS = glm::scale(invColliderS, glm::vec3(rockCollider.ratio * 2.0, rockCollider.ratio * 2.0, rockCollider.ratio * 2.0));
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		sphereCollider.enableWireMode();
		sphereCollider.render(invColliderS);
		glm::vec3 cinv2 = glm::inverse(becariaCollider.u) * glm::vec4(becariaCollider.c, 1.0);
		glm::mat4 invColliderB = glm::mat4(1.0);
		invColliderB = glm::translate(invColliderB, cinv2);
		invColliderB = glm::scale(invColliderB, becariaCollider.e * 2.0f);
		boxCollider.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));
		boxCollider.enableWireMode();
		boxCollider.render(invColliderB);
		// Se regresa el color blanco
		sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
		boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));*/

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
						&& testOBBOBB(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
						&& testSphereSphereIntersection(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
				colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("becaria") == 0)
						modelMatrixbecaria = std::get<1>(jt->second);
					if (jt->first.compare("dart") == 0)
						modelMatrixDart = std::get<1>(jt->second);
				}
			}
		}

		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/
		if(record && modelSelected == 1){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if (saveFrame) {
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}
		}
		else if(keyFramesDartJoints.size() > 0){
			// Para reproducir el frame
			interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
			numPasosDartJoints++;
			if (interpolationDartJoints > 1.0) {
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if (indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}

		if (record && modelSelected == 2) {
			matrixDart.push_back(modelMatrixDart);
			if (saveFrame) {
				appendFrame(myfile, matrixDart);
				saveFrame = false;
			}
		}
		else if (keyFramesDart.size() > 0) {
			// Para reproducir el frame
			interpolationDart = numPasosDart / (float)maxNumPasosDart;
			numPasosDart++;
			if (interpolationDart > 1.0) {
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if (indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}

		// Constantes de animaciones
		rotHelHelY += 0.5;
		animationIndex = 3;

		/*******************************************
		 * State machines
		 *******************************************/

		// State machine for the lambo car
		switch(stateDoor){
		case 0:
			dorRotCount += 0.5;
			if(dorRotCount > 75)
				stateDoor = 1;
			break;
		case 1:
			dorRotCount -= 0.5;
			if(dorRotCount < 0){
				dorRotCount = 0.0;
				stateDoor = 0;
			}
			break;
		}

		glfwSwapBuffers(window);


		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source2Pos[0] = modelMatrixDart[3].x;
		source2Pos[1] = modelMatrixDart[3].y;
		source2Pos[2] = modelMatrixDart[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);

		source3Pos[0] = modelMatrixLambo[3].x;
		source3Pos[1] = modelMatrixLambo[3].y;
		source3Pos[2] = modelMatrixLambo[3].z;
		alSourcefv(source[3], AL_POSITION, source3Pos);

		// Listener for the Third person camera
		listenerPos[0] = modelMatrixbecaria[3].x;
		listenerPos[1] = modelMatrixbecaria[3].y;
		listenerPos[2] = modelMatrixbecaria[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixbecaria[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixbecaria[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		/*listenerPos[0] = camera->getPosition().x;
		listenerPos[1] = camera->getPosition().y;
		listenerPos[2] = camera->getPosition().z;
		alListenerfv(AL_POSITION, listenerPos);
		listenerOri[0] = camera->getFront().x;
		listenerOri[1] = camera->getFront().y;
		listenerOri[2] = camera->getFront().z;
		listenerOri[3] = camera->getUp().x;
		listenerOri[4] = camera->getUp().y;
		listenerOri[5] = camera->getUp().z;*/
		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

void prepareScene(){

	skyboxSphere.setShader(&shaderSkybox);

	modelRock.setShader(&shaderMulLighting);

	modelAircraft.setShader(&shaderMulLighting);

	terrain.setShader(&shaderTerrain);

	// Helicopter
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLampPost2.setShader(&shaderMulLighting);

	//Grass
	modelGrass.setShader(&shaderMulLighting);

	//becaria
	becariaModelAnimate.setShader(&shaderMulLighting);

	// Buildings of scene
	buildingTall.setShader(&shaderMulLighting);
	buildingSmall.setShader(&shaderMulLighting);
	buildingWide.setShader(&shaderMulLighting);
	Factory.setShader(&shaderMulLighting);
	fanAnimated.setShader(&shaderMulLighting);
	goop.setShader(&shaderMulLighting);
	tree.setShader(&shaderMulLighting);
	wallEastWest.setShader(&shaderMulLighting);
	wallNorthSouth.setShader(&shaderMulLighting);
}

void prepareDepthScene(){

	skyboxSphere.setShader(&shaderDepth);

	modelRock.setShader(&shaderDepth);

	modelAircraft.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	// Helicopter
	modelHeliChasis.setShader(&shaderDepth);
	modelHeliHeli.setShader(&shaderDepth);
	// Lamborginhi
	modelLambo.setShader(&shaderDepth);
	modelLamboLeftDor.setShader(&shaderDepth);
	modelLamboRightDor.setShader(&shaderDepth);
	modelLamboFrontLeftWheel.setShader(&shaderDepth);
	modelLamboFrontRightWheel.setShader(&shaderDepth);
	modelLamboRearLeftWheel.setShader(&shaderDepth);
	modelLamboRearRightWheel.setShader(&shaderDepth);

	// Dart Lego
	modelDartLegoBody.setShader(&shaderDepth);
	modelDartLegoMask.setShader(&shaderDepth);
	modelDartLegoHead.setShader(&shaderDepth);
	modelDartLegoLeftArm.setShader(&shaderDepth);
	modelDartLegoRightArm.setShader(&shaderDepth);
	modelDartLegoLeftHand.setShader(&shaderDepth);
	modelDartLegoRightHand.setShader(&shaderDepth);
	modelDartLegoLeftLeg.setShader(&shaderDepth);
	modelDartLegoRightLeg.setShader(&shaderDepth);

	//Lamp models
	modelLamp1.setShader(&shaderDepth);
	modelLamp2.setShader(&shaderDepth);
	modelLampPost2.setShader(&shaderDepth);

	//Grass
	modelGrass.setShader(&shaderDepth);

	//becaria
	becariaModelAnimate.setShader(&shaderDepth);

	// Buildings of scene
	buildingTall.setShader(&shaderDepth);
	buildingSmall.setShader(&shaderDepth);
	buildingWide.setShader(&shaderDepth);
	Factory.setShader(&shaderDepth);
	fanAnimated.setShader(&shaderDepth);
	goop.setShader(&shaderDepth);
	tree.setShader(&shaderDepth);
	wallEastWest.setShader(&shaderDepth);
	wallNorthSouth.setShader(&shaderDepth);
}

void renderScene(bool renderParticles){
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	//Rock render
	/*matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
	modelRock.render(matrixModelRock);*/
	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	/*********************************************
	*  Render de los edificios de la escena
	**********************************************/
	// Edificios altos
	for (int i = 0; i < positionBuildingTalls.size(); i++) {
		positionBuildingTalls[i].y = terrain.getHeightTerrain(positionBuildingTalls[i].x, positionBuildingTalls[i].z);
		buildingTall.setPosition(positionBuildingTalls[i]);
		buildingTall.setScale(glm::vec3(2.0f, 2.0f, 4.0f));
		buildingTall.setOrientation(glm::vec3(-90.0f, 0.0f, 0.0f));
		buildingTall.render();
	}
	// Edificios bajos
	for (int i = 0; i < positionBuildingSmalls.size(); i++) {
		positionBuildingSmalls[i].y = terrain.getHeightTerrain(positionBuildingSmalls[i].x, positionBuildingSmalls[i].z);
		buildingSmall.setPosition(positionBuildingSmalls[i]);
		buildingSmall.setScale(glm::vec3(2.0f, 2.0f, 2.0f));
		buildingSmall.setOrientation(glm::vec3(-90.0f, 0.0f, 0.0f));
		buildingSmall.render();
	}
	// Edificios anchos
	for (int i = 0; i < positionBuildingWides.size(); i++) {
		positionBuildingWides[i].y = terrain.getHeightTerrain(positionBuildingWides[i].x, positionBuildingWides[i].z);
		buildingWide.setPosition(positionBuildingWides[i]);
		buildingWide.setScale(glm::vec3(2.0f, 2.0f, 2.0f));
		buildingWide.setOrientation(glm::vec3(-90.0f, orientationBuildingWides[i], 0.0f));
		buildingWide.render();
	}
	// Edificios fabricas
	for (int i = 0; i < positionFactories.size(); i++) {
		positionFactories[i].y = terrain.getHeightTerrain(positionFactories[i].x, positionFactories[i].z);
		Factory.setPosition(positionFactories[i]);
		Factory.setScale(glm::vec3(1.16f, 1.16f, 6.53f));
		Factory.setOrientation(glm::vec3(-90.0f, 0.0f, 0.0f));
		Factory.render();
	}
	// Fan animated
	for (int i = 0; i < positionFanAnimateds.size(); i++) {
		positionFanAnimateds[i].y = terrain.getHeightTerrain(positionFanAnimateds[i].x, positionFanAnimateds[i].z);
		positionFanAnimateds[i].y += 50;
		fanAnimated.setPosition(positionFanAnimateds[i]);
		fanAnimated.setScale(glm::vec3(2.0f, 2.0f, 2.0f));
		fanAnimated.setOrientation(glm::vec3(-90.0f, orientationFanAnimateds[i], 0.0f));
		fanAnimated.setAnimationIndex(0);
		fanAnimated.render();
	}
	// Charcos
	for (int i = 0; i < positionGoopPuddles.size(); i++) {
		positionGoopPuddles[i].y = terrain.getHeightTerrain(positionGoopPuddles[i].x, positionGoopPuddles[i].z);
		goop.setPosition(positionGoopPuddles[i]);
		goop.setScale(glm::vec3(8.4f, 8.4f, 0.25f));
		goop.setOrientation(glm::vec3(-90.0f, orientationGoopPuddles[i], 0.0f));
		goop.render();
	}
	// Paredes este - oeste
	for (int i = 0; i < positionWallEastWests.size(); i++) {
		positionWallEastWests[i].y = terrain.getHeightTerrain(positionWallEastWests[i].x, positionWallEastWests[i].z);
		wallEastWest.setPosition(positionWallEastWests[i]);
		wallEastWest.setScale(glm::vec3(0.25f, 0.25f, 0.25f));
		wallEastWest.setOrientation(glm::vec3(-90.0f, 0.0f, 0.0f));
		wallEastWest.render();
	}
	// Pares norte - sur
	for (int i = 0; i < positionWallNorthSouths.size(); i++) {
		positionWallNorthSouths[i].y = terrain.getHeightTerrain(positionWallNorthSouths[i].x, positionWallNorthSouths[i].z);
		wallNorthSouth.setPosition(positionWallNorthSouths[i]);
		wallNorthSouth.setScale(glm::vec3(0.25f, 0.25f, 0.25f));
		wallNorthSouth.setOrientation(glm::vec3(-90.0f, 0.0f, 0.0f));
		wallNorthSouth.render();
	}

	// Render the lamps
	for (int i = 0; i < lamp2Position.size(); i++){
		lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
		modelLamp2.setPosition(lamp2Position[i]);
		modelLamp2.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLamp2.render();
		modelLampPost2.setPosition(lamp2Position[i]);
		modelLampPost2.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelLampPost2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
		modelLampPost2.render();
	}

	// Grass
	glDisable(GL_CULL_FACE);
	glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
	grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
	modelGrass.setPosition(grassPosition);
	modelGrass.render();
	glEnable(GL_CULL_FACE);

	// Fountain
	/*glDisable(GL_CULL_FACE);
	modelFountain.render(modelMatrixFountain);
	glEnable(GL_CULL_FACE);*/

	// Dart lego
	// Se deshabilita el cull faces IMPORTANTE para la capa
	//glDisable(GL_CULL_FACE);
	//modelMatrixDart[3][1] = terrain.getHeightTerrain(modelMatrixDart[3][0], modelMatrixDart[3][2]);
	//glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
	//modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
	//modelDartLegoBody.render(modelMatrixDartBody);
	//glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
	//modelDartLegoHead.render(modelMatrixDartHead);
	//modelDartLegoMask.render(modelMatrixDartHead);
	//glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
	//modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
	//modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
	//modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
	//glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
	//modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
	//modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
	//modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
	//modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
	//glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
	//modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
	//modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
	//modelDartLegoRightArm.render(modelMatrixDartRightArm);
	//glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
	//modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
	//modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
	//modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
	//modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
	//modelDartLegoRightHand.render(modelMatrixDartRightHand);
	//glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
	//modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
	//modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
	//modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
	//glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
	//modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
	//modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
	//modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
	//modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
	//// Se regresa el cull faces IMPORTANTE para la capa
	//glEnable(GL_CULL_FACE);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	modelMatrixbecaria[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixbecaria[3][0], modelMatrixbecaria[3][2]);
	tmv = currTime - startTimeJump;
	if(modelMatrixbecaria[3][1] < terrain.getHeightTerrain(modelMatrixbecaria[3][0], modelMatrixbecaria[3][2])){
		isJump = false;
		modelMatrixbecaria[3][1] = terrain.getHeightTerrain(modelMatrixbecaria[3][0], modelMatrixbecaria[3][2]);
	}
	//modelMatrixbecaria[3][1] = terrain.getHeightTerrain(modelMatrixbecaria[3][0], modelMatrixbecaria[3][2]);
	glm::mat4 modelMatrixbecariaBody = glm::mat4(modelMatrixbecaria);
	modelMatrixbecariaBody = glm::scale(modelMatrixbecariaBody, glm::vec3(0.0021, 0.0021, 0.0021));
	becariaModelAnimate.setAnimationIndex(animationIndex);
	becariaModelAnimate.render(modelMatrixbecariaBody);

	/**********
	 * Update the position with alpha objects
	 */
	// Update the aircraft
	blendingUnsorted.find("aircraft")->second = glm::vec3(modelMatrixAircraft[3]);
	// Update the lambo
	blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	// Update the helicopter
	blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	// Arboles
	for (int i = 0; i < positionTrees.size(); i++) {
		positionTrees[i].y = terrain.getHeightTerrain(positionTrees[i].x, positionTrees[i].z);
		tree.setPosition(positionTrees[i]);
		tree.setScale(glm::vec3(0.25f, 0.25f, 0.25f));
		tree.setOrientation(glm::vec3(-90.0f, 0.0f, 0.0f));
		tree.render();
	}
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if(it->second.first.compare("aircraft") == 0){
			// Render for the aircraft model
			/*glm::mat4 modelMatrixAircraftBlend = glm::mat4(modelMatrixAircraft);
			modelMatrixAircraftBlend[3][1] = terrain.getHeightTerrain(modelMatrixAircraftBlend[3][0], modelMatrixAircraftBlend[3][2]) + 2.0;
			modelAircraft.render(modelMatrixAircraftBlend);*/
		}
		//else if(it->second.first.compare("lambo") == 0){
		//	// Lambo car
		//	glm::mat4 modelMatrixLamboBlend = glm::mat4(modelMatrixLambo);
		//	modelMatrixLamboBlend[3][1] = terrain.getHeightTerrain(modelMatrixLamboBlend[3][0], modelMatrixLamboBlend[3][2]);
		//	modelMatrixLamboBlend = glm::scale(modelMatrixLamboBlend, glm::vec3(1.3, 1.3, 1.3));
		//	modelLambo.render(modelMatrixLamboBlend);
		//	glActiveTexture(GL_TEXTURE0);
		//	glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboBlend);
		//	modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08676, 0.707316, 0.982601));
		//	modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		//	modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08676, -0.707316, -0.982601));
		//	modelLamboLeftDor.render(modelMatrixLamboLeftDor);
		//	modelLamboRightDor.render(modelMatrixLamboBlend);
		//	modelLamboFrontLeftWheel.render(modelMatrixLamboBlend);
		//	modelLamboFrontRightWheel.render(modelMatrixLamboBlend);
		//	modelLamboRearLeftWheel.render(modelMatrixLamboBlend);
		//	modelLamboRearRightWheel.render(modelMatrixLamboBlend);
		//	// Se regresa el cull faces IMPORTANTE para las puertas
		//}
		//else if(it->second.first.compare("heli") == 0){
		//	// Helicopter
		//	glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		//	modelHeliChasis.render(modelMatrixHeliChasis);

		//	glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		//	modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		//	modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		//	modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		//	modelHeliHeli.render(modelMatrixHeliHeli);
		//}
		//else if(renderParticles && it->second.first.compare("fountain") == 0){
		//	/**********
		//	 * Init Render particles systems
		//	 */
		//	glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
		//	modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
		//	modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
		//	modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
		//	currTimeParticlesAnimation = TimeManager::Instance().GetTime();
		//	if(currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
		//		lastTimeParticlesAnimation = currTimeParticlesAnimation;
		//	//glDisable(GL_DEPTH_TEST);
		//	glDepthMask(GL_FALSE);
		//	// Set the point size
		//	glPointSize(10.0f);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
		//	shaderParticlesFountain.turnOn();
		//	shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
		//	shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
		//	shaderParticlesFountain.setInt("ParticleTex", 0);
		//	shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
		//	shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
		//	glBindVertexArray(VAOParticles);
		//	glDrawArrays(GL_POINTS, 0, nParticles);
		//	glDepthMask(GL_TRUE);
		//	//glEnable(GL_DEPTH_TEST);
		//	shaderParticlesFountain.turnOff();
		//	/**********
		//	 * End Render particles systems
		//	 */
		//}
		//else if(renderParticles && it->second.first.compare("fire") == 0){
		//	/**********
		//	 * Init Render particles systems
		//	 */
		//	lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
		//	currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

		//	shaderParticlesFire.setInt("Pass", 1);
		//	shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
		//	shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_1D, texId);
		//	glEnable(GL_RASTERIZER_DISCARD);
		//	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
		//	glBeginTransformFeedback(GL_POINTS);
		//	glBindVertexArray(particleArray[1-drawBuf]);
		//	glVertexAttribDivisor(0,0);
		//	glVertexAttribDivisor(1,0);
		//	glVertexAttribDivisor(2,0);
		//	glDrawArrays(GL_POINTS, 0, nParticlesFire);
		//	glEndTransformFeedback();
		//	glDisable(GL_RASTERIZER_DISCARD);

		//	shaderParticlesFire.setInt("Pass", 2);
		//	glm::mat4 modelFireParticles = glm::mat4(1.0);
		//	modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		//	modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
		//	shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

		//	shaderParticlesFire.turnOn();
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
		//	glDepthMask(GL_FALSE);
		//	glBindVertexArray(particleArray[drawBuf]);
		//	glVertexAttribDivisor(0,1);
		//	glVertexAttribDivisor(1,1);
		//	glVertexAttribDivisor(2,1);
		//	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
		//	glBindVertexArray(0);
		//	glDepthMask(GL_TRUE);
		//	drawBuf = 1 - drawBuf;
		//	shaderParticlesFire.turnOff();

		//	/****************************+
		//	 * Open AL sound data
		//	 */
		//	source1Pos[0] = modelFireParticles[3].x;
		//	source1Pos[1] = modelFireParticles[3].y;
		//	source1Pos[2] = modelFireParticles[3].z;
		//	alSourcefv(source[1], AL_POSITION, source1Pos);

		//	/**********
		//	 * End Render particles systems
		//	 */
		//}

	}
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
