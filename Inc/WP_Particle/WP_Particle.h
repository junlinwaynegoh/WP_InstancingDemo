#ifndef WP_PARTICLE_H
#define WP_PARTICLE_H

#include <vector>
#include <glm.hpp>


struct WP_Particle
{

	glm::vec2 pos;
	glm::vec2 direction;
	glm::vec3 col;
	int imageGenerated;
};

struct WP_ParticleSystem
{
	std::vector<WP_Particle> m_particles;

	void GenerateParticles(unsigned int 
		_numberParticles,int _maxImage = 0);
	void RunParticleSystem(float 
		_multiplier);
	void DestroyParticles();

};

#endif