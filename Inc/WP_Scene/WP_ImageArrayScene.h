#include <WP_Scene/WP_Scene.h>
#include <vector>
#include <WP_Graphics/WP_Image.h>
#include <WP_Particle/WP_Particle.h>

struct WP_ImageArrayScene : WP_Scene
{
	struct instancedData
	{
		glm::mat4 matrix;
		GLfloat index;
	};
	struct instancedTintData
	{
		glm::mat4 matrix;
		GLfloat index;
		glm::vec3 color;
	};
	typedef struct VertexBatched
	{
		glm::vec2 pos;
		glm::vec3 col;
		glm::vec2 texCoord;
		GLint batchedImage;
	};

struct WP_ImageArray
{
	GLuint m_imageArrayID;

	int m_width;
	int m_height;
	int m_channels;

	std::vector<std::string> m_imageName;
	
	void LoadImages(const std::string& _filePath, unsigned int _imageCount,GLuint _width, 
		GLuint _height, GLuint _channels);
};

private:
	std::vector<WP_Image> m_images;
	WP_ImageArray m_imageArray;

	int m_maxParticles = 80000;

	WP_ParticleSystem m_particleSystem;

	int m_numberOfParticles = 100;
	float m_particleSystemSpeed = 0.1f;

	int m_renderType = 0;

	int m_numImages = 0;

	GLuint m_defaultImageShaderID;
	GLuint m_defaultImageShaderMVPLoc;
	GLuint m_defaultImageTextureLoc;

	GLuint m_arrayImageShaderID;
	GLuint m_arrayImageMVPLoc;
	GLuint m_arrayImageTextureLoc;
	GLuint m_arrayLayerIndexLoc;

	GLuint m_arrayImageColorShaderID;
	GLuint m_arrayImageColorMVPLoc;
	GLuint m_arrayImageColorTextureLoc;
	GLuint m_arrayLayerColorIndexLoc;
	GLuint m_arrayColorTintLoc;

	GLuint m_instancedShaderID;
	GLuint m_instancedVBO;
	GLuint m_instancedTextureLoc;

	GLuint m_instancedTintImageShaderID;
	GLuint m_instancedTintVBO;
	GLuint m_instancedTintImageTextureLoc;

	GLuint m_batchedTintImageShaderID;
	GLuint m_batchedTintImageTextureLoc;

	std::vector<instancedData> modelMatrices;
	std::vector<instancedTintData> tintData;

	std::vector<VertexBatched> batchedData;
	std::vector<GLuint> batchedIndices;
	GLuint m_batchedVertexArray;
	GLuint m_batchedVertexBuffer;
	GLuint m_batchedIndicesBuffer;


private:
	void RenderDefault();
	void RenderArray();
	void RenderArrayTint();
	void RenderArrayInstanced();
	void RenderArrayInstancedTint();
	void RenderImage(const glm::mat4& _mvp, int _imageIndex);
	void RenderImageArray(const glm::mat4& _mvp, int _imageIndex);
	void RenderImageArrayTint(const glm::mat4& _mvp, int _imageIndex, const glm::vec3& _colorTint);
	void RenderArrayBatched();
public:
	void StartScene();
	void PreSceneInitialisation();
	void UpdateScene();
	void RenderImGui();
	void DeallocateScene();
};