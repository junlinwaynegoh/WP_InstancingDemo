#include <WP_Scene/WP_Scene.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <WP_Particle/WP_Particle.h>

struct WP_DefaultParticleScene : WP_Scene
{
private:

	struct instancedData
	{
		glm::mat4 matrix;
		glm::vec3 color;
	};

private:
	GLuint m_defaultShaderID;
	GLuint m_instancedShaderID;
	GLuint m_mvpLocation;
	GLuint m_colorLocation;

	GLuint m_optimisedInstancedVBO;
	int m_maxParticles = 500000;

	WP_ParticleSystem m_particleSystem;

	int m_numberOfParticles = 100;
	float m_particleSystemSpeed = 0.1f;

	int m_renderType = 0;

private:
	void RenderTriangle(const glm::mat4& _mvp, const glm::vec3& _color);
	void RenderDefaultNormally();
	void RenderDefaultInstanced();
	void RenderDefaultInstancedOptimised();

public:
	void PreSceneInitialisation();
	void StartScene();
	void RenderImGui();
	void UpdateScene();
	void DeallocateScene();
};