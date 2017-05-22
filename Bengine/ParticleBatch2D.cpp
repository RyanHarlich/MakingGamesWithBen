#include "ParticleBatch2D.h"



namespace Bengine {

	/* NEW: removed */
	//void Particle2D::update(float deltaTime) {
	//	m_position += velocity * deltaTime;
	//}




		
	ParticleBatch2D::ParticleBatch2D()
	{
	}





	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;
	}



	/* NEW: function pointer argument */
	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, std::function<void(Particle2D&, float)> updateFunc /* = defaultParticleUpdate */) {
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;

		/* NEW */
		m_updateFunc = updateFunc;
	}







	void ParticleBatch2D::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; ++i) {
			// check if particle is active
			if (m_particles[i].life > 0.0f) { 

				/* NEW: removed and changed to function pointer style */
				//m_particles[i].update(deltaTime);
				/* NEW use of function pointer */
				m_updateFunc(m_particles[i], deltaTime);


				m_particles[i].life -= m_decayRate * deltaTime;
				
			}
		}
	}






	void ParticleBatch2D::draw(SpriteBatch* spriteBatch) {
		
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		for (int i = 0; i < m_maxParticles; ++i) {
			auto& p = m_particles[i];
			// check if particle is active
			if (p.life > 0.0f) { 
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);				
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

		p.life = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;

	}





	int ParticleBatch2D::findFreeParticle() {
		
		// Search from last free particle to max amount of particles (ex. 51-100)
		for (int i = m_lastFreeParticle; i < m_maxParticles; ++i) {
			// if particle is inactive
			if (m_particles[i].life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		// If no free particles, look before last free particle (0-50)
		for (int i = 0; i < m_lastFreeParticle; ++i) {
			// if particle is inactive
			if (m_particles[i].life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		// No particles are free, overwrite first particle
		return 0;
	}
}