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

	GameObject* ptr = nullptr;
	size_t size;
	auto& gameManager = GameManager::getInstance();
	gameManager.getGameObjects(&ptr, size);

	if (ptr == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		(ptr + i)->parseShader();
		(ptr + i)->mesh.bind();
	}

}

void Scene::picking_phase_render_map()
{
	pickingShader.bind();
	parse_picking_shader_info(pickingShader, mapMesh, *sceneCamera);
	mapMesh.bind();
	GameObject* ptr = nullptr;
	size_t size;
	auto& gameManager = GameManager::getInstance();
	gameManager.getGameObjects(&ptr, size);

	if (ptr == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		(ptr + i)->parsePickingShader(pickingShader);
		(ptr + i)->mesh.bind();
	}
}

void Scene::initialise_camera(float width, float height)
{
	this->width = width;
	this->height = height;

	sceneCamera = new Camera(width, height);
	auto& gameManager = GameManager::getInstance();
	gameManager.setMainCamera(sceneCamera);
}

bool Scene::load_resources()
{
	const std::streamsize MODEL_LOAD_BUFFER_SIZE = 65536;
	const unsigned int MAP_PLATFORM_SIZE = 64;

	MeshData mapData;
	MeshData tileData;

	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/ROMAN_BATTLEFIELD.obj", mapData);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/Pillar.obj", tileData);

	defaultShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/vertex.vert", RESOURCES_PATH "shaders/fragment.frag");
	pickingShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/pickingVertex.vert", RESOURCES_PATH "shaders/pickingFragment.frag");

	tileTexture.initialise(RESOURCES_PATH "textures/test.png");

	mapMesh.initialise(mapData);


	initialise_map(tileData, &defaultShader);

	std::cout << "Scene Loaded Succesfully" << std::endl;

	std::cout << "Render objects count:" << GameManager::getInstance().getRenderObjectCount() << std::endl;

	return true;
}

void Scene::debug_input(GLFWwindow* window)
{
	int currentKey = glfwGetKey(window, GLFW_KEY_G);
	if (currentKey == GLFW_PRESS && previous_key != GLFW_PRESS)
	{
		std::cout << "Pressed" << std::endl;
	}

	previous_key = currentKey;

}

void Scene::update()
{
	// Update and call functions from current Game phase
	// tick rate


	gameStateManager.tick();


}

void Scene::spawn_debug_unit()
{
	
}

void Scene::initialise_map(MeshData& tileMesh, Shader* shader)
{
	unsigned int objCounter = 0;

	float X_OFFSET = -0.25f;
	float Y_OFFSET = 0.5f;

	float X_STEP = 0.85f;
	float Y_STEP = 0.75f;

	auto& gameManager = GameManager::getInstance();

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{

			GameObject& go = gameManager.instantiate();

			go.mesh.initialise(tileMesh);
			go.shader = shader;
			go.mesh.position = glm::vec3((x * X_STEP) + X_OFFSET, 0, (-y * Y_STEP) + Y_OFFSET);
			go.mesh.scale = 0.8f;
			go.mesh.assign_texture(tileTexture);
		}
	}
}
