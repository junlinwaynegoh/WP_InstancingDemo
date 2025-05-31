#include <WP_Particle/WP_Particle.h>
#include <random>

/*!
	Some random functions from chatgpt
*/
glm::vec2 randomDirection2D() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 2.0f * 3.14f);

	float angle = dist(gen);
	return glm::vec2(cos(angle), sin(angle));  // Already normalized
}
glm::vec2 randomPos(float min, float max) {
	static std::random_device rd;  // Seed
	static std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_real_distribution<float> dist(min, max);

	return glm::vec2(dist(gen), dist(gen));
}
glm::vec3 randomColor() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	return glm::vec3(dist(gen), dist(gen), dist(gen));
}
int randomInt(int min, int max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

void WP_ParticleSystem::GenerateParticles(unsigned int
	_numParticles, int _maxImage)
{
	DestroyParticles();

	for (unsigned int i = 0; i < _numParticles; ++i)
	{
		WP_Particle m_particle;
		
		m_particle.pos = randomPos(-1.8,1.8);
		m_particle.direction = randomDirection2D();
		m_particle.col = randomColor();
		m_particle.imageGenerated = randomInt(0, _maxImage);

		m_particles.push_back(m_particle);
	}
}
void WP_ParticleSystem::RunParticleSystem(float
	_multiplier)
{
	for (unsigned int i = 0; i < m_particles.size(); ++i)
	{
		m_particles[i].pos += m_particles[i].direction * _multiplier;
		if (m_particles[i].pos.x > 1.8f || m_particles[i].pos.x < -1.8f)
		{
			m_particles[i].direction.x *= -1;
			m_particles[i].col = randomColor();
		}
		if (m_particles[i].pos.y > 1.8f || m_particles[i].pos.y < -1.8f)
		{
			m_particles[i].direction.y *= -1;
			m_particles[i].col = randomColor();
		}
	}
}

void WP_ParticleSystem::DestroyParticles()
{
	m_particles.clear();
}