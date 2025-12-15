#include <scene.h>
#include <camera.h>
#include <fstream>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui_internal.h>
#define RESERVE_TILE_LENGTH 5
#define STARTER_DATA 3
#define MAP_WIDTH  11
#define MAP_HEIGHT 11
using json = nlohmann::json;


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UnitData, ID, name, health, attackShield, magicShield, attackDamage, magicDamage)

void debug_preview_obj(GameObject* obj);


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
	gameManager.view_unit_deployment(true);
	for (size_t i = 0; i < SHOP_UNIT_COUNT; i++)
	{
		int rnd = rand() % gameManager.getUnits().size();
		std::cout << i+1 << ". " << gameManager.getUnits()[rnd].name << '\n';
	}
}

void Scene::game_battle_phase()
{
	auto& gm = GameManager::getInstance();
	gm.view_unit_deployment(false);
}

void Scene::game_idle_phase()
{
	auto& gm = GameManager::getInstance();

	//gm.deployment();

}

void Scene::mouse_left_click(void)
{
	auto* selection = sceneCamera->get_selected_object();
	auto& gameManager = GameManager::getInstance();

	if (selection == nullptr)
	{
		return;
	}

	selection->OnEntityPressed();

	if (UnitController* unit = dynamic_cast<UnitController*>(selection))
	{
		if (gameStateManager.GetGameState() == IDLE)
		{
			gameManager.view_unit_deployment(true);
		}
	}

}

void Scene::mouse_right_click(void)
{
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
			TileData* data = t->tileData;
			gameManager.selectedUnit->move_to_tile(data);
			if (gameStateManager.GetGameState() == IDLE)
			{
				gameManager.view_unit_deployment(false);
			}
			
				
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
		 
	}

}

void Scene::render_imgui()
{

	auto& gameManager = GameManager::getInstance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	{
		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoCollapse;

		// Each frame:
		ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x / 2) - 150, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Always);

		float max, min, curr;
		gameStateManager.get_game_state_values(&curr, &max, &min);

		ImGui::Begin("Game State",0,flags);
		ImGui::Text("Current State: %s", gameStateManager.state_to_string());
		ImGui::Text("Time to next state: %.2f", max - curr);
		ImGui::ProgressBar((curr / max));

		ImGui::End();
	}


	if (gameManager.selectedUnit != nullptr)
	{
		auto& selected = gameManager.selectedUnit;
		preview_unit_information(selected);
	}

}

void debug_preview_obj(GameObject* obj)
{
		//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Each frame:
		/*ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200, ImGui::GetIO().DisplaySize.y - 100), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Always);*/

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 1164, ImGui::GetIO().DisplaySize.y - 200));

		{
			float newX = obj->mesh.position.x;
			float newY = obj->mesh.position.y;
			float newZ = obj->mesh.position.z;


			ImGui::Begin("Object Properties");
			if (ImGui::SliderFloat("X:", &newX, -30.0f, 30.0f) || ImGui::SliderFloat("Y:", &newY, -30.0f, 30.0f) || ImGui::SliderFloat("Z:", &newZ, -30.0f, 30.0f))
			{
				obj->mesh.position = glm::vec3(newX, newY, newZ);
			}


			if (ImGui::Button("Export"))
			{

			}


			ImGui::End();
		}
}

// should be called once, on entity press probably
void Scene::preview_unit_information(UnitController* selected)
{

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 1164, ImGui::GetIO().DisplaySize.y - 200));
	ImGui::SetNextWindowSize(ImVec2(250, 300));
	{
		ImGui::Begin("Unit Information");

		ImGui::Text("Name: %s", selected->unit->battleData.name);
		ImGui::Text("Health: %d", selected->unit->battleData.health);
		ImGui::Text("Physical Attack: %d", selected->unit->battleData.attackDamage);
		ImGui::Text("Magic Attack: %d", selected->unit->battleData.magicDamage);

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

void do_something()
{
	
}

bool Scene::initialise()
{
	auto& gameManager = GameManager::getInstance();

	const std::streamsize MODEL_LOAD_BUFFER_SIZE = 65536;
	const unsigned int MAP_PLATFORM_SIZE = 64;

	m_player1.initialise(false);
	m_player2.initialise();

	MeshData map_data;
	MeshData dock_path_data;
	MeshData dock_logs_data;
	MeshData map_tile_data;

	defaultShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/vertex.vert", RESOURCES_PATH "shaders/fragment.frag");
	defaultUnitShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/defaultUnitVertex.vert", RESOURCES_PATH "shaders/defaultUnitFragment.frag");
	pickingShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/pickingVertex.vert", RESOURCES_PATH "shaders/pickingFragment.frag");
	highlightShader.loadShaderProgramFromFile(RESOURCES_PATH "shaders/highlight.vert", RESOURCES_PATH "shaders/highlight.frag");

	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/TFT/TEST_main_part_map_tft.obj", map_data);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/TFT/dock_path.obj", dock_path_data);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/TFT/dock_log_pillars.obj", dock_logs_data);
	loadModelFromFile(MODEL_LOAD_BUFFER_SIZE, RESOURCES_PATH "models/TFT/map_tile.obj", map_tile_data);

	mapTexture.initialise(RESOURCES_PATH "textures/map_wip_bcolor1.png");
	dockPathTexture.initialise(RESOURCES_PATH "textures/test_bcolor.png");
	dockLogsTexture.initialise(RESOURCES_PATH "textures/PILLAR_TEST_bcolor.png");
	dummyTexture.initialise(RESOURCES_PATH "textures/PILLAR_TEST_bcolor.png");
	//mapTileTexture.initialise(RESOURCES_PATH "textures/map_tile_bcolor.png");
	mapTileTexture.initialise(RESOURCES_PATH "textures/tile_test.png");
	

	std::ifstream in_units(RESOURCES_PATH "json/units.json");
	
	json unitsJson;
	in_units >> unitsJson;
	gameManager.setUnits(unitsJson.get<std::vector<UnitData>>());

	in_units.close();

	gameManager.setHighlightShader(highlightShader);


	// debug
	//Unit test(gameManager.getUnits()[0], &defaultShader);

	auto& map = gameManager.instantiate<GameObject>();
	auto& logs = gameManager.instantiate<GameObject>();
	auto& dock = gameManager.instantiate<GameObject>();

	map.initialise(map_data, defaultShader, highlightShader, mapTexture, 2.5f);
	logs.initialise(dock_logs_data, defaultShader, highlightShader, dockLogsTexture, 2.5f);
	dock.initialise(dock_path_data, defaultShader, highlightShader, dockPathTexture, 2.5f);
	int tileIndex = 0;

	const float offsetX = 20.230f;
	const float offsetY = 15.384f;

	// Generiraj tileset za mapu

	for (size_t y = 0; y < 11; y++)
	{
		for (size_t x = 0; x < 11; x++)
		{
			

			auto& tile = gameManager.instantiate<Tile>();
			tile.initialise(map_tile_data, defaultShader, highlightShader, mapTileTexture, 2.0f);
			float xPos = -(x * 3.0f) + offsetX;
			float yPos = -(y * 3.0f) + offsetY;
			auto& pos = glm::vec3(yPos, 1.0f, xPos);
			m_tiles[tileIndex].initialise(x, y, pos);

			tile.mesh.position = pos;
			tile.tileData = &(m_tiles[tileIndex]);
			tileIndex++;
		}
	}

	initialise_reserve_tiles(map_tile_data, &defaultShader, m_player1);


	gameStateManager.onIdleStateEntered = std::bind(&Scene::game_idle_phase, this);
	gameStateManager.onBattleStateEntered = std::bind(&Scene::game_battle_phase, this);
	gameStateManager.onShopStateEntered = std::bind(&Scene::game_shopping_phase, this);

	m_player1.add_unit(gameManager.getUnits()[0], &defaultUnitShader, &dummyTexture);

	// REFACTOR: to add a unit, player should only need the json data for the unit, a texture and shader manager should handle the rest based on the Unit ID; 
	// Player.AddUnit() ---> handle initialising and dictionary lookup of texture/shader/mesh data
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
	auto& gm = GameManager::getInstance();
	gm.tickManager.tick();
	gameStateManager.tick();
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


	float X_STEP = 4.00f;
	float Y_STEP = 4.00f;

	auto& gameManager = GameManager::getInstance();

	for (int y = 0, i = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{

			Tile& go = gameManager.instantiate<Tile>();
			float xP = (float)(x * X_STEP);
			float yP = (float)(-y * Y_STEP);
			glm::vec3 pos = glm::vec3(yP, 0.0f, xP);

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
		glm::vec3 pos = glm::vec3(22.5f, 1.5f, i * 4.0f);
		auto& go = gameManager.instantiate<ReserveTile>();
		go.initialise(mesh, defaultShader, highlightShader, mapTileTexture, 2.0f);
		go.mesh.position = pos;

		player.add_reserve_tile(&go, i);
	}

}
