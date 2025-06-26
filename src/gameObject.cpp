#include <gameManager.h>
#include <gameObject.h>
#include <camera.h>
void GameObject::OnEntityPressed()
{
	std::cout << "Entity Pressed" << std::endl;
}

void GameObject::parseShader()
{
	shader->bind();

	Camera* camera = GameManager::getInstance().getMainCamera();

	if (camera->selectedObjectID == mesh.meshID)
	{
		
	}

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);

	glUniformMatrix4fv(shader->getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));
	glUniform3f(shader->getUniform("modelPos"), mesh.position.x, mesh.position.y, mesh.position.z);
	glUniform1f(shader->getUniform("scale"), mesh.scale);

	glUniform1i(shader->getUniform("tex0"),0);
	mesh.bind_texture();
}

void GameObject::parsePickingShader(Shader& pickingShader)
{
	pickingShader.bind();

	Camera* camera = GameManager::getInstance().getMainCamera();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);
	glUniformMatrix4fv(pickingShader.getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));
	glUniform3f(pickingShader.getUniform("modelPos"), mesh.position.x, mesh.position.y, mesh.position.z);
	glUniform1f(pickingShader.getUniform("scale"), mesh.scale);

	glUniform1ui(pickingShader.getUniform("objectIndex"), mesh.meshID);
	glUniform1ui(pickingShader.getUniform("drawIndex"), mesh.meshID);

}

void GameObject::initialise()
{
}

void GameObject::update()
{
}