#include <tile.h>
#include <gameManager.h>
#include <camera.h>

#define MAP_WIDTH  11
#define MAP_HEIGHT 11

void Tile::parseShader()
{
	shader->bind();

	Camera* camera = GameManager::getInstance().getMainCamera();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);
	model = glm::scale(model, glm::vec3(mesh.scale));

	glUniformMatrix4fv(shader->getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));

	glUniform1i(shader->getUniform("tex0"), 0);
	glUniform1i(shader->getUniform("DebugSelect"), DebugSelect);
	glUniform1i(shader->getUniform("selected"), tileData->isOccupied);
	mesh.bind_texture();
}


void TileData::getNeighborData(std::vector<TileData*>& data)
{
	struct Dir
	{
		int first;
		int second;

		Dir(int f, int s)
		{
			first = f;
			second = s;
		}

	};

	std::vector<Dir> directions;
	directions.push_back(Dir(0, 1)); //North
	directions.push_back(Dir(1, 0)); //East
	directions.push_back(Dir(0, -1));//South
	directions.push_back(Dir(-1, 0));//West

	int currentX = xGrid;
	int currentY = yGrid;

	auto& gm = GameManager::getInstance();

	for (auto& dir : directions)
	{

		int neighbourX = currentX + dir.first;
		int neighbourY = currentY + dir.second;

		if (neighbourX >= 0 && neighbourX < MAP_WIDTH && neighbourY >= 0 && neighbourY < MAP_HEIGHT)
		{
			TileData* tile = gm.findByGrid(neighbourX, neighbourY);
			data.push_back(tile);
		}
	}

}