#include "ParticleBatch2D.h"



namespace Bengine {


	void Particle2D::update(float deltaTime) {
		m_position += m_velocity * deltaTime;
	}




		
	ParticleBatch2D::ParticleBatch2D()
	{
	}





	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles; // free memory, need square brackets or it wont call all destructors of all elements of the array
	}




	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture) {
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles]; // allocated an array on the heap
		m_decayRate = decayRate;
		m_texture = texture;
	}







	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; ++i) {
			// check if particle is active
			if (m_particles[i].m_life > 0.0f) { // here is a pointer to an array of particles, the array is not full of particle pointers because no loop with new was implemented
				m_particles[i].update(deltaTime);
				m_particles[i].m_life -= m_decayRate * deltaTime; // timestep scalar the decay rate since it is frame based
				
			}
		}
	}






	void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
		
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		for (int i = 0; i < m_maxParticles; ++i) {
			auto& p = m_particles[i];
			// check if particle is active
			if (p.m_life > 0.0f) { 
				glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.m_color);				
			}
		}

	}






	// activate a particle until its life decays to 0
	void ParticleBatch2D::addParticle(const glm::vec2& position,
		const glm::vec2& velocity,
		const ColorRGBA8& color,
		float width) {

		int particleIndex = findFreeParticle();

		auto& p = m_particles[particleIndex];

		p.m_life = 1.0f;
		p.m_position = position;
		p.m_velocity = velocity;
		p.m_color = color;
		p.m_width = width;

	}





	int ParticleBatch2D::findFreeParticle() {
		
		// Search from last free particle to max amount of particles (ex. 51-100)
		for (int i = m_lastFreeParticle; i < m_maxParticles; ++i) {
			// if particle is inactive
			if (m_particles[i].m_life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		// If no free particles, look before last free particle (0-50)
		for (int i = 0; i < m_lastFreeParticle; ++i) {
			// if particle is inactive
			if (m_particles[i].m_life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		// No particles are free, overwrite first particle
		return 0;
	}
}