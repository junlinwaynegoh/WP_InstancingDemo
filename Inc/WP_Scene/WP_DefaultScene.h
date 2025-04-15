#include <WP_Scene/WP_Scene.h>
#include <GL/glew.h>
#include <glm.hpp>

struct WP_DefaultScene : WP_Scene
{
private:
	GLuint m_defaultShaderID;
	float yValue;
	float xValue;
	GLuint m_mvpLocation;

private:
	void RenderSquare(const glm::mat4& _mvp);
	void RenderTriangle(const glm::mat4& _mvp);

public:
	void PreSceneInitialisation();
	void StartScene();
	void RenderImGui();
	void UpdateScene();
};