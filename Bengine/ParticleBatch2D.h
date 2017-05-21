#pragma once

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

#include <glm/glm.hpp>

namespace Bengine {

	// If particle is small it will be more cache friendly, because if a cache line is 64 bytes, then if Particle is small will be getting more particles per cache line fetched
	class Particle2D {
	public:
		friend class ParticleBatch2D;

		void update(float deltaTime);

	private:
		glm::vec2 m_position = glm::vec2(0.0f);
		glm::vec2 m_velocity = glm::vec2(0.0f);
		ColorRGBA8 m_color;
		float m_life = 0.0f; // life-time, if life is greater than 0 it is active
		float m_width = 0.0f;
	};


	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate, GLTexture texture);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

		// activate a particle until its life decays to 0
		void addParticle(const glm::vec2& position, 
			const glm::vec2& velocity,
			const ColorRGBA8& color,
			float width);

	private:
		int findFreeParticle();

		float m_decayRate = 0.1f;
		// Avoids calling new and delete
		Particle2D* m_particles = nullptr; // Buffer of particles, max is 10,000
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		GLTexture m_texture;
	};

}