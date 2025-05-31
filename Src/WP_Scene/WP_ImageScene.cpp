#include <WP_Scene/WP_ImageScene.h>
#include <WP_Graphics/WP_Shader.h>
#include <WP_Graphics/WP_Primitive.h>
#include <gtc/matrix_transform.hpp>
#include <filesystem>
#include <imgui.h>


void WP_ImageScene::PreSceneInitialisation()
{
	m_imageShaderID =
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageShader");
	m_mvpLocation = glGetUniformLocation(m_imageShaderID, "MVP");
	m_textureLocation = glGetUniformLocation(m_imageShaderID, "uTexture");


	std::string path = "../Assets/Images";

	int count = 0;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			++count;
		}
	}

	m_images.reserve(count);

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			m_images.push_back(WP_Image());
			std::string fileName = entry.path().filename().string();
			m_images[m_images.size() - 1].LoadImageIntoOpenGL(path + "/" + fileName, fileName);
		}
	}

}

void WP_ImageScene::RenderImage(const glm::mat4& _mvp)
{
	glUseProgram(m_imageShaderID);
	GLuint textureLoc = 0;
	
	glUniform1i(m_textureLocation, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D, m_images[m_currentImageIndex].GetImage());

	glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, (const GLfloat*)&_mvp);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);
	glDrawElements(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0);
}

void WP_ImageScene::UpdateScene()
{
	glm::mat4 mvp;
	mvp = glm::mat4(1.f);
	mvp = glm::scale(mvp, glm::vec3(0.5f, 0.5f, 1.f));

	RenderImage(mvp);
}

void WP_ImageScene::RenderImGui()
{
	ImGui::Begin("Image Choice");

	for (int i = 0 ; i <  m_images.size();++i)
	{
		if (ImGui::Button(m_images[i].GetImageName().c_str()))
		{
			m_currentImageIndex = i;
		}
	}
	
	ImGui::End();
}