#include <WP_Scene/WP_Scene.h>
#include <WP_Graphics/WP_Image.h>
#include <vector>

struct WP_ImageScene : WP_Scene
{
private:
	GLuint m_imageShaderID;
	GLuint m_mvpLocation;
	GLuint m_textureLocation;

	std::vector<WP_Image> m_images;

	unsigned int m_currentImageIndex;

private:
	void RenderImage(const glm::mat4& _mvp);

public:
	void PreSceneInitialisation();
	void UpdateScene();
	void RenderImGui();
};