#include <WP_Scene/WP_DefaultScene.h>
#include <WP_Graphics/WP_Shader.h>
#include <gtc/matrix_transform.hpp>
#include <WP_Graphics/WP_Primitive.h>
#include <imgui.h>

void WP_DefaultScene::PreSceneInitialisation()
{
	m_defaultShaderID =
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultShader");

    m_mvpLocation = glGetUniformLocation(m_defaultShaderID, "MVP");
}
void WP_DefaultScene::StartScene()
{
    yValue = 0;
    xValue = 0;
}
void WP_DefaultScene::RenderImGui()
{
    ImGui::Begin("Moving Objects Window");
    ImGui::SliderFloat("yPos", &yValue, -2.f, 2.f);
    ImGui::SliderFloat("xPos", &xValue, -2.f, 2.f);
    ImGui::End();
}
void WP_DefaultScene::RenderSquare(const glm::mat4& _mvp)
{
    glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, (const GLfloat*)&_mvp);
    glBindVertexArray(Primitives::Primitive::square.vertexArray);
    glDrawElements(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0);
}
void WP_DefaultScene::RenderTriangle(const glm::mat4& _mvp)
{
    glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, (const GLfloat*)&_mvp);
    glBindVertexArray(Primitives::Primitive::triangle.vertexArray);
    glDrawElements(GL_TRIANGLES, Primitives::Primitive::triangle.indicesSize, GL_UNSIGNED_INT, 0);
}
void WP_DefaultScene::UpdateScene()
{

    glm::mat4 mvp;
    mvp = glm::mat4(1.f);
    mvp = glm::translate(mvp, glm::vec3(xValue, yValue, 0.0f));

    glUseProgram(m_defaultShaderID);

    RenderSquare(mvp);
    RenderTriangle(mvp);
}