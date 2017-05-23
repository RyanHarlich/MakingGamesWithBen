#pragma once

#include <functional>

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

#include <glm/glm.hpp>

namespace Bengine {


	// If particle is small it will be more cache friendly, because if a cache line is 64 bytes, then if Particle is small will be getting more particles per cache line fetched
	class Particle2D {
		public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color;
		float life = 0.0f; // life-time, if life is greater than 0 it is active
		float width = 0.0f;
	};



	inline void defualtParticleUpdate(Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
	}




	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate, GLTexture texture,
			std::function<void(Particle2D&, float)> updateFunc = defualtParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

		// activate a particle until its life decays to 0
		void addParticle(const glm::vec2& position, 
			const glm::vec2& velocity,
			const ColorRGBA8& color,
			float width);

	private:
		int findFreeParticle();



		std::function<void(Particle2D&, float)> m_updateFunc;
		float m_decayRate = 0.1f;
		Particle2D* m_particles = nullptr;
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		GLTexture m_texture;
	};

}