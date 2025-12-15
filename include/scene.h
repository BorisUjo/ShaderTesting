#pragma once
#include <iostream>
#include <demoShaderLoader.h>
#include <meshInformation.h>
#include <renderObject.h>
#include <tile.h>
#include <GLFW/glfw3.h>
#include <gameManager.h>
#include <texture.h>
#include <gameStateManager.h>
#include <nlohmann/json.hpp>
#include <unit.h>
#include <reserveTile.h>
#include <player.h>

class Scene
{
public:
	Scene();
	~Scene();
	void render_scene(); // default framebuffer
	void render_imgui();
	void picking_phase_render_scene();
	void initialise_camera(float width, float height);
	void debug_input(GLFWwindow* window);
	void update();
	bool initialise();

	void add_unit(Player& player, UnitData& data);
	void spawn_debug_unit(void);
	void write_to_json(void);
	void debug_switch_game_state(void);

	void game_shopping_phase(void);
	void game_battle_phase(void);
	void game_idle_phase(void);

	void mouse_left_click(void);
	void mouse_right_click(void);



private:
	void initialise_map(MeshData& tileMesh, Shader* shader);
	void initialise_reserve_tiles(MeshData& mesh, Shader* shader, Player& player);
	void debug_write_unit_data_json();
	void preview_unit_information(UnitController* selected);
	std::vector<UnitData> generate_starter_units(std::vector<UnitData> data);

public:
	Camera* sceneCamera;
private:

	const uint8_t SHOP_UNIT_COUNT = 4;

	int previous_key = 0; // use for reference on input system
	
	GameStateManager gameStateManager;
	RenderObject mapMesh;
	RenderObject testDummy;

	Shader defaultShader;
	Shader defaultUnitShader;
	Shader pickingShader;
	Shader highlightShader;

	Texture tileTexture;
	Texture dummyTexture;

	Texture mapTexture;
	Texture dockLogsTexture;
	Texture dockPathTexture;
	Texture mapTileTexture;


	float m_screenWidth = 0.0f;
	float m_screenHeight = 0.0f;

	TileData m_tiles[121];

	// player 1 -> player
	// player 2 -> NPC

	Player m_player1;
	Player m_player2;
	

	// debug

	bool jsonWriteUnit = false;

};