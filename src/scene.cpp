#include <scene.h>
#include <camera.h>
#include <fstream>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#define RESERVE_TILE_LENGTH 5
#define STARTER_DATA 3
using json = nlohmann::json;


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UnitData, ID, name, health, attackShield, magicShield, attackDamage, magicDamage)

std::ostream& operator<<(std::ostream& os, const glm::vec3 vec)
{
	os << "X:" << vec.x << '|';
	os << "Y:" << vec.y << '|';
	os << "Z:" << vec.z << '|';

	return os;

}
std::ostream& operator<<(std::ostream& os, const UnitData& data)
{
	os << "UNIT DATA" << '\n';
	os << "name:" << data.name << '\n';
	os << "health: " << data.health << '\n';
	os << "attack shield: " << data.attackShield << '\n';
	os << "magic shield: " << data.magicShield << '\n';
	os << "attack damage: " << data.attackDamage << '\n';
	os << "magic damage: " << data.magicDamage << '\n';

	return os;

}

void unit_to_json(const UnitData& data)
{
	std::ifstream in(RESOURCES_PATH "json/units.json");
	json j_loaded;
	in >> j_loaded;
	std::vector<UnitData> loaded_units =j_loaded.get<std::vector<UnitData>>();
	loaded_units.push_back(data);

	json final = loaded_units;

	std::ofstream out(RESOURCES_PATH "json/units.json");
	out << final.dump(4);

	in.close();
	out.close();
}



void Scene::debug_write_unit_data_json()
{
	std::cout << "-------------------" << std::endl;
	std::cout << "UNIT CREATION DATA: " << std::endl;

	UnitData data;

	std::cout << "UNIT NAME: ";
	std::cin >> data.name;

	std::cout << "UNIT HP: ";
	std::cin >> data.health;
	std::cout << "UNIT ATTACK: ";
	std::cin >> data.attackDamage;
	std::cout << "UNIT MAGIC ATTACK: ";
	std::cin >> data.magicDamage;
	std::cout << "UNIT SHIELD: ";
	std::cin >> data.attackShield;
	std::cout << "UNIT MAGIC SHIELD: " ;
	std::cin >> data.magicShield;

	std::cout << "-------------------" << std::endl << std::endl;


	std::cout << data << std::endl;

	unit_to_json(data);

	char answer = ' ';
	while (answer != 'Y' && answer != 'N')
	{
		std::cout << "Y/N (Y-write more unit data, N- exit write mode)" << std::endl;
		std::cin >> answer;
	}

	if (answer == 'N')
	{
		jsonWriteUnit = false;
	}



}

std::vector<UnitData> Scene::generate_starter_units(std::vector<UnitData> data)
{
	std::vector<UnitData> starterData;

	for (size_t i = 0; i < STARTER_DATA; i++)
	{
		int rnd = rand() % data.size();
		starterData.push_back(data[rnd]);
	}

	return starterData;
}

void Scene::debug_switch_game_state()
{

	std::cout << "Debug Switch State" << "\n";
	auto nextState = gameStateManager.GetGameState() + 1;

	if (nextState > 2)
	{
		nextState = 0;
	}

	gameStateManager.debug_change_state((GameStateManager::GameState)nextState);
}

void Scene::game_shopping_phase()
{
	auto& gameManager = GameManager::getInstance();

	for (size_t i = 0; i < SHOP_UNIT_COUNT; i++)
	{
		int rnd = rand() % gameManager.getUnits().size();
		std::cout << i+1 << ". " << gameManager.getUnits()[rnd].name << '\n';
	}
}

void Scene::game_battle_phase()
{

}

void Scene::game_idle_phase()
{
	
}

void Scene::mouse_left_click(void)
{
	auto* selection = sceneCamera->get_selected_object();

	if (selection == nullptr)
	{
		return;
	}
	else if (GameObject* obj = dynamic_cast<GameObject*>(selection))
	{	
		obj->OnEntityPressed();
	}
	
}

void Scene::mouse_right_click(void)
{

	if (gameStateManager.GetGameState() == GameStateManager::GameState::GameStateBattle)
	{
		return;
	}

	auto& gameManager = GameManager::getInstance();
	auto* selection = sceneCamera->get_selected_object();


	if (selection == nullptr)
	{
		return;
	}
	else if (gameManager.selectedUnit != nullptr && selection != nullptr)
	{
		if (Tile* t = dynamic_cast<Tile*>(selection))
		{
			TileData& data = *t->tileData;
			gameManager.selectedUnit->move_to_tile(data);
			gameManager.selectedUnit = nullptr; // after move, "deselect" unit
		}
	}

}


Scene::Scene()	
{

}

Scene::~Scene()
{
	delete sceneCamera;
}

void Scene::render_scene()
{
	GameObject** objects = nullptr;
	size_t size;
	auto& gameManager = GameManager::getInstance();
	gameManager.getGameObjects(&objects, size);

	if (objects == nullptr)
	{
		return;
	}

	// loop over all gameobjects

	GameObject* highlightTarget = nullptr;

	for (size_t i = 0; i < size; i++)
	{
		if (objects[i]->isSelected())
		{
			highlightTarget = objects[i];
		}

		objects[i]->render();

	}

	if (highlightTarget != nullptr)
	{
		highlightTarget->highlight();
	}

}

void Scene::render_imgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	{
		//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Each frame:
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, ImGui::GetIO().DisplaySize.y - 100), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;        // Disable resizing
		window_flags |= ImGuiWindowFlags_NoMove;          // Disable manual moving
		window_flags |= ImGuiWindowFlags_NoCollapse;      // Disable collapsing
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		window_flags |= ImGuiWindowFlags_NoNavFocus;

		// Optional: remove title bar or decorations
		// window_flags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::Begin("End Turn", nullptr, window_flags);
		if (ImGui::Button("Next Turn", ImVec2(280, 35)))
		{
			turnManager.end_turn();
		}
		ImGui::End();
	}


	
}

void Scene::picking_phase_render_scene()
{
	pickingShader.bind();
	//parse_picking_shader_info(pickingShader, mapMesh, *sceneCamera);
	//mapMesh.bind();
	GameObject** objects = nullptr;
	size_t size;
	auto& gameManager = GameManager::getInstance();
	gameManager.getGameObjects(&objects, size);

	if (objects == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		GameObject* go = objects[i];
		go->parsePickingShader(pickingShader);
		go->mesh.bind();
	}
}

void Scene::initialise_camera(float width, float height)
{
	this->m_screenWidth = width;
	this->m_screenHeight = height;

	sceneCamera = new Camera(width, height);
	auto& gameManager = GameManager::getInstance();
	gameManager.setMainCamera(sceneCamera);
}

bool Scene::initialise()
{

	// TO DO (1/10/2025 - 20/10/2025):
	/*
		- Spawn new units in the reserve section ( consider making tiles a gameobject)  [DONE]
		- imGUI setup																	[DONE]
		- setup new map																	[NOT DONE]
		- gameobject better constructor													[NOT DONE]
		- Implement turn based                  										[NOT DONE]
		- Implement enemy ai                    										[NOT DONE]
		- simple skybox																	[NOT DONE]
		- Mock battle test																[NOT DONE]
		- basic item setup																[NOT DONE]
		- finalise game loop															[NOT DONE]
	
	*/


	auto& gameManager = GameManager::getInstance();

	const std::streamsize MODEL_LOAD_BUFFER_SIZE = 65536;
	const unsigned int MAP_PLATFORM_SIZE = 64;

	MeshData mapData;
	MeshData tileData;
	MeshData testDummyData;

	MeshData mapDataTFT;

	defaultShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/vertex.vert", RESOURCES_PATH "shaders/fragment.frag");
	pickingShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/pickingVertex.vert", RESOURCES_PATH "shaders/pickingFragment.frag");
	highlightShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/highlight.vert", RESOURCES_PATH "shaders/highlight.frag");

	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/Debug/ROMAN_BATTLEFIELD.obj", mapData);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/Debug/TEST_DUMMY.obj", testDummyData);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/Debug/tile.obj", tileData);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/TFT/TEST_main_part_map_tft.obj", mapDataTFT);

	tileTexture.initialise(RESOURCES_PATH "textures/test.png");
	dummyTexture.initialise(RESOURCES_PATH "textures/test1.png");
	mapTexture.initialise(RESOURCES_PATH "textures/map_wip_bcolor.png");

	std::ifstream in_units(RESOURCES_PATH "json/units.json");
	
	json unitsJson;
	in_units >> unitsJson;
	gameManager.setUnits(unitsJson.get<std::vector<UnitData>>());

	in_units.close();

	initialise_map(tileData, &defaultShader);
	initialise_reserve_tiles(tileData, &defaultShader, m_player1);

	m_player1.initialise(false); // real player
	m_player2.initialise(true); // terminator

	gameManager.setHighlightShader(highlightShader);

	// debug
	//Unit test(gameManager.getUnits()[0], &defaultShader);

	//auto& map = gameManager.instantiate<GameObject>();
	//map.mesh.initialise(mapDataTFT);
	//map.mesh.scale = 2.0f;
	//map.shader = &defaultShader;
	//map.highlightShader = gameManager.getHighlightShader();
	//map.mesh.assign_texture(mapTexture);

	// REFACTOR: to add a unit, player should only need the json data for the unit, a texture and shader manager should handle the rest based on the Unit ID; 
	// Player.AddUnit() ---> handle initialising and dictionary lookup of texture/shader/mesh data
	auto& startingDataP1 = generate_starter_units(gameManager.getUnits());
	auto& startingDataP2 = generate_starter_units(gameManager.getUnits());

	for (auto& unit : startingDataP1)
	{
		m_player1.add_unit(unit, &defaultShader, &dummyTexture);
	}
	for (auto& unit : startingDataP2)
	{
		m_player2.add_unit(unit, &defaultShader, &dummyTexture);
	}



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
	
}


void Scene::spawn_debug_unit()
{
	
}

void Scene::write_to_json()
{
	jsonWriteUnit = !jsonWriteUnit;
}

void Scene::initialise_map(MeshData& tileMesh, Shader* shader)
{
	unsigned int objCounter = 0;

	float X_OFFSET = 0.0f;
	float Y_OFFSET = 0.0f;

	float X_STEP = 4.00f;
	float Y_STEP = 4.00f;

	auto& gameManager = GameManager::getInstance();

	for (int y = 0, i = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{

			Tile& go = gameManager.instantiate<Tile>();

			glm::vec3 pos = glm::vec3((x * X_STEP) + X_OFFSET, 0, (-y * Y_STEP) + Y_OFFSET);

			m_tiles[i].initialise(x, y, pos);

			go.mesh.initialise(tileMesh);
			go.shader = shader;
			go.highlightShader = gameManager.getHighlightShader();
			go.mesh.position = pos;
			go.mesh.scale = 1.5f;
			go.mesh.assign_texture(tileTexture);
			go.tileData = &(m_tiles[i]);
			i++;
		}
	}
}

void Scene::initialise_reserve_tiles(MeshData& mesh, Shader* shader, Player& player)
{
	auto& gameManager = GameManager::getInstance();

	for (int i = 0; i < RESERVE_TILE_LENGTH; i++)
	{
		glm::vec3 pos = glm::vec3(i * 4.0f, 0, 8.0f);
		auto& go = gameManager.instantiate<ReserveTile>();
		go.mesh.initialise(mesh);
		go.shader = shader;
		go.highlightShader = gameManager.getHighlightShader();
		go.mesh.position = pos;
		go.mesh.scale = 1.5f;
		go.mesh.assign_texture(tileTexture);

		player.add_reserve_tile(&go, i);
	}

}
