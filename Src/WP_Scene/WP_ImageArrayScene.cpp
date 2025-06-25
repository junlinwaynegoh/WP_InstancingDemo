#include <WP_Scene/WP_ImageArrayScene.h>
#include <filesystem>
#include <stb_image.h>
#include <WP_Graphics/WP_Primitive.h>
#include <gtc/matrix_transform.hpp>
#include <WP_Graphics/WP_Shader.h>
#include <imgui.h>

void WP_ImageArrayScene::WP_ImageArray::LoadImages(const std::string& _filePath, unsigned int _imageCount, GLuint _width,
	GLuint _height, GLuint _channels)
{
	glGenTextures(1, &m_imageArrayID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_imageArrayID);

	m_width = _width;
	m_height = _height;
	m_channels = _channels;

	// Allocate storage for the texture array
	if (_channels == 4)
	{
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, m_width, m_height, m_channels);
	}
	else if(_channels == 3)
	{
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, m_width, m_height, m_channels);
	}
	else
	{
		std::cout << "invalid image" << std::endl;
	}

	int count = 0;


	for (const auto& entry : std::filesystem::directory_iterator(_filePath)) {
		if (entry.is_regular_file()) {
			std::string filePath = _filePath;
			filePath += "/";
			filePath += entry.path().filename().string().c_str();

			unsigned char*  imageData = stbi_load(filePath.c_str(),
				&m_width, &m_height, &m_channels, 0);

			// Upload image data to the texture array layer
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, count++,
				m_width, m_height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

			// Free the image data after uploading
			stbi_image_free(imageData);
		}
	}

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void WP_ImageArrayScene::RenderImage(const glm::mat4& _mvp,int _imageIndex)
{
	GLuint textureLoc = 0;

	glUniform1i(m_defaultImageTextureLoc, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D, m_images[_imageIndex].GetImage());

	glUniformMatrix4fv(m_defaultImageShaderMVPLoc, 1, GL_FALSE, (const GLfloat*)&_mvp);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);
	glDrawElements(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0);
}

void WP_ImageArrayScene::RenderImageArray(const glm::mat4& _mvp, int _imageIndex)
{
	glUniform1i(m_arrayLayerIndexLoc, _imageIndex);

	glUniformMatrix4fv(m_arrayImageMVPLoc, 1, GL_FALSE, (const GLfloat*)&_mvp);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);
	glDrawElements(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0);
}

void WP_ImageArrayScene::RenderImageArrayTint(const glm::mat4& _mvp, int _imageIndex, const glm::vec3& _colorTint)
{
	glUniform1i(m_arrayLayerColorIndexLoc, _imageIndex);
	glUniform3fv(m_arrayColorTintLoc,1,(const GLfloat*)&_colorTint);
	glUniformMatrix4fv(m_arrayImageColorMVPLoc, 1, GL_FALSE, (const GLfloat*)&_mvp);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);
	glDrawElements(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0);
}

void WP_ImageArrayScene::RenderDefault()
{
	glUseProgram(m_defaultImageShaderID);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);

	for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
	{
		glm::mat4 mvp;
		mvp = glm::mat4(1.f);
		mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
			m_particleSystem.m_particles[i].pos.y, 0.0f));
		mvp = glm::scale(mvp, glm::vec3(0.1f));
		RenderImage(mvp, m_particleSystem.m_particles[i].imageGenerated);
	}
}

void WP_ImageArrayScene::RenderArray()
{
	glUseProgram(m_arrayImageShaderID);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);

	GLuint textureLoc = 0;
	glUniform1i(m_arrayImageTextureLoc, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_imageArray.m_imageArrayID);

	for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
	{
		glm::mat4 mvp;
		mvp = glm::mat4(1.f);
		mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
			m_particleSystem.m_particles[i].pos.y, 0.0f));
		mvp = glm::scale(mvp, glm::vec3(0.1f));
		RenderImageArray(mvp, m_particleSystem.m_particles[i].imageGenerated);
	}
}

void WP_ImageArrayScene::RenderArrayTint()
{
	glUseProgram(m_arrayImageColorShaderID);
	glBindVertexArray(Primitives::Primitive::square.vertexArray);

	GLuint textureLoc = 0;
	glUniform1i(m_arrayImageTextureLoc, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_imageArray.m_imageArrayID);

	for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
	{
		glm::mat4 mvp;
		mvp = glm::mat4(1.f);
		mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
			m_particleSystem.m_particles[i].pos.y, 0.0f));
		mvp = glm::scale(mvp, glm::vec3(0.1f));
		RenderImageArrayTint(mvp, m_particleSystem.m_particles[i].imageGenerated,m_particleSystem.m_particles[i].col);
	}
}

void WP_ImageArrayScene::RenderArrayInstanced()
{
	glUseProgram(m_instancedShaderID);

	glBindVertexArray(Primitives::Primitive::square.vertexArray);

	GLuint textureLoc = 0;
	glUniform1i(m_instancedTextureLoc, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_imageArray.m_imageArrayID);

	for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
	{
		glm::mat4 mvp;
		mvp = glm::mat4(1.f);
		mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
			m_particleSystem.m_particles[i].pos.y, 0.0f));
		mvp = glm::scale(mvp, glm::vec3(0.1f));

		modelMatrices.push_back({ mvp, (float)m_particleSystem.m_particles[i].imageGenerated });
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_instancedVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, modelMatrices.size()
		* sizeof(instancedData), modelMatrices.data());

	modelMatrices.clear();

	unsigned int vertexBindingLocation = 3;

	for (GLuint i = 0; i < 4; i++) {
		glEnableVertexAttribArray(vertexBindingLocation + i);
		glVertexAttribPointer(vertexBindingLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(instancedData), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(vertexBindingLocation + i, 1);
	}

	unsigned int fragmentBindingLocation = 7;
	glEnableVertexAttribArray(fragmentBindingLocation);
	glVertexAttribPointer(fragmentBindingLocation, 1, GL_FLOAT, GL_FALSE, sizeof(instancedData), (void*)(sizeof(glm::mat4)));
	glVertexAttribDivisor(fragmentBindingLocation, 1);

	glDrawElementsInstanced(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0, m_particleSystem.m_particles.size());
}

void WP_ImageArrayScene::RenderArrayInstancedTint()
{
	glUseProgram(m_instancedTintImageShaderID);

	glBindVertexArray(Primitives::Primitive::square.vertexArray);

	GLuint textureLoc = 0;
	glUniform1i(m_instancedTintImageTextureLoc, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_imageArray.m_imageArrayID);

	for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
	{
		glm::mat4 mvp;
		mvp = glm::mat4(1.f);
		mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
			m_particleSystem.m_particles[i].pos.y, 0.0f));
		mvp = glm::scale(mvp, glm::vec3(0.1f));

		tintData.push_back({ mvp, (GLfloat)m_particleSystem.m_particles[i].imageGenerated,m_particleSystem.m_particles[i].col});
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_instancedTintVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, tintData.size()
		* sizeof(instancedTintData), tintData.data());

	tintData.clear();

	unsigned int vertexBindingLocation = 3;

	for (GLuint i = 0; i < 4; i++) {
		glEnableVertexAttribArray(vertexBindingLocation + i);
		glVertexAttribPointer(vertexBindingLocation + i, 4, GL_FLOAT, GL_FALSE, sizeof(instancedTintData), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(vertexBindingLocation + i, 1);
	}

	unsigned int fragmentBindingLocation = 7;
	glEnableVertexAttribArray(fragmentBindingLocation);
	glVertexAttribPointer(fragmentBindingLocation, 1, GL_FLOAT, GL_FALSE, sizeof(instancedTintData), (void*)(sizeof(glm::mat4)));
	glVertexAttribDivisor(fragmentBindingLocation, 1);

	unsigned int fragmentColorBindingLocation = 8;
	glEnableVertexAttribArray(fragmentColorBindingLocation);
	glVertexAttribPointer(fragmentColorBindingLocation, 3, GL_FLOAT, GL_FALSE, sizeof(instancedTintData), (void*)(sizeof(glm::mat4) + sizeof(GLfloat)));
	glVertexAttribDivisor(fragmentColorBindingLocation, 1);

	glDrawElementsInstanced(GL_TRIANGLES, Primitives::Primitive::square.indicesSize, GL_UNSIGNED_INT, 0, m_particleSystem.m_particles.size());
}
void WP_ImageArrayScene::RenderArrayBatched()
{

	glUseProgram(m_batchedTintImageShaderID);

	const Vertex SquareVertices[4] =
	{
		{ { -1.f,   -1.f}  , { 1.f,0.f,0.f} , {0,1}},
		{ { 1.f ,   -1.f}  , { 0.f,1.f,0.f} , {1,1}},
		{ { 1.f ,   1.f}   , { 0.f,0.f,1.f} , {1,0}},
		{ { -1.f,   1.f}   , { 1.f,0.f,0.f} , {0,0}}
	};

	glBindVertexArray(m_batchedVertexArray);

	GLuint vposbindLocation = 0;
	GLuint vcolbindLocation = 1;
	GLuint vtexbindLocation = 2;
	GLuint vLayerIndexLocation = 3;

	GLuint textureLoc = 0;
	glUniform1i(m_batchedTintImageTextureLoc, textureLoc);
	glActiveTexture(textureLoc);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_imageArray.m_imageArrayID);

	for (int i = 0; i < m_particleSystem.m_particles.size(); ++i)
	{
		glm::mat4 mvp;
		mvp = glm::mat4(1.f);
		mvp = glm::translate(mvp, glm::vec3(m_particleSystem.m_particles[i].pos.x,
			m_particleSystem.m_particles[i].pos.y, 0.0f));
		mvp = glm::scale(mvp, glm::vec3(0.1f));

		for (int x = 0; x < 4; ++x)
		{
			batchedData.push_back(VertexBatched());
			VertexBatched& currentBatchedData = batchedData[batchedData.size() - 1];
			currentBatchedData.col = m_particleSystem.m_particles[i].col;
			currentBatchedData.pos = mvp * glm::vec4(SquareVertices[x].pos.x,SquareVertices[x].pos.y, 0, 1);
			currentBatchedData.texCoord = SquareVertices[x].texCoord;
			currentBatchedData.batchedImage = m_particleSystem.m_particles[i].imageGenerated;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_batchedVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, batchedData.size()
		* sizeof(VertexBatched), batchedData.data());

	batchedData.clear();

	glEnableVertexAttribArray(vposbindLocation);
	glVertexAttribPointer(vposbindLocation, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexBatched), (void*)offsetof(VertexBatched, pos));

	glEnableVertexAttribArray(vcolbindLocation);
	glVertexAttribPointer(vcolbindLocation, 3, GL_FLOAT, GL_FALSE,
		sizeof(VertexBatched), (void*)offsetof(VertexBatched, col));

	glEnableVertexAttribArray(vtexbindLocation);
	glVertexAttribPointer(vtexbindLocation, 2, GL_FLOAT, GL_FALSE,
		sizeof(VertexBatched), (void*)offsetof(VertexBatched, texCoord));

	glEnableVertexAttribArray(vLayerIndexLocation);
	glVertexAttribPointer(vLayerIndexLocation, 1, GL_FLOAT, GL_FALSE,
		sizeof(VertexBatched), (void*)offsetof(VertexBatched, batchedImage));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_batchedIndicesBuffer);

	glDrawElements(GL_TRIANGLES, m_particleSystem.m_particles.size() * 6, GL_UNSIGNED_INT, 0);
}

void WP_ImageArrayScene::PreSceneInitialisation()
{
	m_defaultImageShaderID =
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageShader");
	m_defaultImageShaderMVPLoc = glGetUniformLocation(m_defaultImageShaderID, "MVP");
	m_defaultImageTextureLoc = glGetUniformLocation(m_defaultImageShaderID, "uTexture");

	m_arrayImageShaderID = 
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageArrayShader");
	m_arrayImageMVPLoc = glGetUniformLocation(m_arrayImageShaderID, "MVP");
	m_arrayImageTextureLoc = glGetUniformLocation(m_arrayImageShaderID, "uTextureArray");
	m_arrayLayerIndexLoc = glGetUniformLocation(m_arrayImageShaderID, "ulayerIndex");

	m_instancedShaderID = 
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageParticleInstancedArray");
	m_instancedTextureLoc = glGetUniformLocation(m_instancedShaderID, "uTextureArray");

	m_instancedTintImageShaderID =
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageParticleInstancedArrayColor");
	m_instancedTintImageTextureLoc = glGetUniformLocation(m_instancedTintImageShaderID, "uTextureArray");

	m_arrayImageColorShaderID = 
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageArrayColorShader");
	m_arrayImageColorMVPLoc = glGetUniformLocation(m_arrayImageColorShaderID, "MVP");
	m_arrayImageColorTextureLoc = glGetUniformLocation(m_arrayImageColorShaderID, "uTextureArray");
	m_arrayLayerColorIndexLoc = glGetUniformLocation(m_arrayImageColorShaderID, "ulayerIndex");
	m_arrayColorTintLoc = glGetUniformLocation(m_arrayImageColorShaderID, "uColorTint");

	m_batchedTintImageShaderID =
		WP_ShaderManager::GetInstance().AddShader("../Shaders/DefaultImageArrayColorBatched");
	m_arrayImageColorTextureLoc = glGetUniformLocation(m_batchedTintImageTextureLoc, "uTextureArray");

	std::string path = "../Assets/ImagesArray";

	glGenBuffers(1, &m_instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles
		* sizeof(instancedData), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_instancedTintVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_instancedTintVBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles 
		* sizeof(instancedTintData), NULL, GL_DYNAMIC_DRAW);



	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			++m_numImages;
		}
	}

	m_images.reserve(m_numImages);

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			m_images.push_back(WP_Image());
			std::string fileName = entry.path().filename().string();
			m_images[m_images.size() - 1].LoadImageIntoOpenGL(path + "/" + fileName, fileName);
		}
	}

	m_imageArray.LoadImages(path, m_numImages, m_images[0].GetWidth(),
		m_images[0].GetHeight(), m_images[0].GetChannels());



	modelMatrices.reserve(m_maxParticles);
	batchedData.reserve(m_maxParticles * 4);
	batchedIndices.reserve(m_maxParticles * 6);

	glGenVertexArrays(1, &m_batchedVertexArray);
	glBindVertexArray(m_batchedVertexArray);

	glGenBuffers(1, &m_batchedVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_batchedVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * m_maxParticles * sizeof(VertexBatched),
		NULL, GL_DYNAMIC_DRAW);

	const GLuint squareIndices[6] = {
	   0, 1, 2,
	   2, 3, 0
	};

	for (int i = 0; i < m_maxParticles; ++i)
	{
		for (int x = 0; x < 6; ++x)
		{
			batchedIndices.push_back(squareIndices[x] + i * 4);
		}
	}

	glGenBuffers(1, &m_batchedIndicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_batchedIndicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * m_maxParticles * sizeof(GLuint),
		batchedIndices.data(), GL_STATIC_DRAW);

}

void WP_ImageArrayScene::StartScene()
{
	m_particleSystem.GenerateParticles(m_numberOfParticles, m_numImages-1);
}

void WP_ImageArrayScene::RenderImGui()
{
	ImGui::Begin("ParticleSettings");
	if (ImGui::SliderInt("Number Particles", &m_numberOfParticles, 0, m_maxParticles))
	{
		m_particleSystem.DestroyParticles();
		m_particleSystem.GenerateParticles(m_numberOfParticles,m_numImages-1);
	}

	int currentSelection = static_cast<int>(m_renderType);

	const char* renderType[] = { "Render Default", "Render Array", "Render Instanced","Render Array Instanced Tint","Render Array Tint", "Render Array Batched"};
	if (ImGui::Combo("render type", &currentSelection, renderType, IM_ARRAYSIZE(renderType)))
	{
		m_renderType = (currentSelection);
	}
	ImGui::End();
}

void WP_ImageArrayScene::UpdateScene()
{
	m_particleSystem.RunParticleSystem(m_particleSystemSpeed);

	switch (m_renderType)
	{
	case 0:
	{
		RenderDefault();
		break;
	}
	case 1:
	{
		RenderArray();
		break;
	}
	case 2:
	{
		RenderArrayInstanced();
		break;
	}
	case 3:
	{
		RenderArrayInstancedTint();
		break;
	}
	case 4:
	{
		RenderArrayTint();
		break;
	}
	case 5:
	{
		RenderArrayBatched();
		break;
	}
	default:
	{
		break;
	}
	}
}

void WP_ImageArrayScene::DeallocateScene()
{

	glDeleteBuffers(1, &m_instancedVBO);
}