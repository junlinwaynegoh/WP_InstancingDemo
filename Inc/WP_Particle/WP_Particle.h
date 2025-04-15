#include <vector>
#include <glm.hpp>


struct WP_Particle
{

	glm::vec2 pos;
	glm::vec2 direction;
	glm::vec3 col;
};

struct WP_ParticleSystem
{
	std::vector<WP_Particle> m_particles;

	void GenerateParticles(unsigned int 
		_numberParticles);
	void RunParticleSystem(float 
		_multiplier);
	void DestroyParticles();

};