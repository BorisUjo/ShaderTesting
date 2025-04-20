#include <scene.h>
#include <camera.h>
Scene::Scene()
{
	
}

Scene::~Scene()
{
	delete sceneCamera;
}

void Scene::render_map()
{
	defaultShader.bind();
	parse_shader_info(defaultShader, mapMesh, *sceneCamera);
	mapMesh.bind();
}

void Scene::set_camera_params(float width, float height)
{
	this->width = width;
	this->height = height;

	sceneCamera = new Camera(width, height);

}

bool Scene::load_resources()
{
	const std::streamsize MODEL_LOAD_BUFFER_SIZE = 65536;
	const unsigned int MAP_PLATFORM_SIZE = 64;

	MeshData mapData;
	MeshData tileData;

	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/ROMAN_BATTLEFIELD.obj", mapData);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/Pillar.obj", tileData);

	mapMesh.initialise(mapData);
	initialise_platform(mapTiles, tileData);

	defaultShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/vertex.vert", RESOURCES_PATH "shaders/fragment.frag");

	std::cout << "Scene Loaded Succesfully" << std::endl;


	return true;
}

void Scene::initialise_platform(RenderObject* objects, MeshData& data)
{
	unsigned int objCounter = 0;

	float X_OFFSET = -0.25f;
	float Y_OFFSET = 0.5f;

	float X_STEP = 0.85f;
	float Y_STEP = 0.75f;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			objects[objCounter].initialise(data);
			objects[objCounter].position = glm::vec3((x * X_STEP) + X_OFFSET, 0, (-y * Y_STEP) + Y_OFFSET);
			objects[objCounter].scale = 0.8f;
			objCounter++;
		}
	}
}