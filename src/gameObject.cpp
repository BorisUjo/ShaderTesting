#include <gameManager.h>
#include <gameObject.h>
#include <camera.h>
void GameObject::OnEntityPressed()
{
    auto& go = GameManager::getInstance();
	
    if (go.selectedUnit == nullptr)
    {
        return;
    }

    go.selectGameObject(mesh.meshID);
}

void GameObject::OnEntityExit()
{
}

void GameObject::parseShader()
{
	shader->bind();

	Camera* camera = GameManager::getInstance().getMainCamera();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);
    model = glm::scale(model, glm::vec3(mesh.scale));

	glUniformMatrix4fv(shader->getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));

	glUniform1i(shader->getUniform("tex0"),0);
	mesh.bind_texture();
}

void GameObject::parseHighlightShader()
{
	highlightShader->bind();

	Camera* camera = GameManager::getInstance().getMainCamera();
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);
    model = glm::scale(model, glm::vec3(mesh.scale));

	glUniformMatrix4fv(highlightShader->getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));

}

void GameObject::parsePickingShader(Shader& pickingShader)
{
	pickingShader.bind();

	Camera* camera = GameManager::getInstance().getMainCamera();

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, mesh.position);
    model = glm::scale(model, glm::vec3(mesh.scale));

	glUniformMatrix4fv(pickingShader.getUniform("PVM"), 1, false, glm::value_ptr(camera->get_matrix() * model));

	glUniform1ui(pickingShader.getUniform("objectIndex"), mesh.meshID);
	glUniform1ui(pickingShader.getUniform("drawIndex"), mesh.meshID);

}

void GameObject::initialise()
{
    auto& gameManager = GameManager::getInstance();

    gameManager.addObjectID(gameManager.getRenderObjectCount(), this);

}

void GameObject::update()
{
}

void GameObject::render()
{
    Camera* camera = GameManager::getInstance().getMainCamera();


    // TODO: put this is a seperate function 
    if (camera->selectedObjectID == mesh.meshID)
    {
        objectSelected = true;
    }
    else
    {
        objectSelected = false;
        //glStencilMask(0x00);
        parseShader();
        mesh.bind();
    }
}

GameObject* GameObject::get()
{
	return this;
}

void GameObject::setTag(std::string newTag)
{
    gameObjectTag = newTag;
}

void GameObject::highlight()
{
	glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        parseShader();
        mesh.bind(); 

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);      
        glDepthMask(GL_FALSE);   
        glEnable(GL_DEPTH_TEST);

        float originalScale = mesh.scale;
        mesh.scale = originalScale * 1.05f; 
        parseHighlightShader();
        mesh.bind();
        mesh.scale = originalScale; 

        glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
        glDisable(GL_STENCIL_TEST);
}
