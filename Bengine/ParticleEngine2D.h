#pragma once

#include <vector>



namespace Bengine {

	class ParticleBatch2D; // forward declarations make compile time better
	class SpriteBatch;     // if the function is used then have to include file such as in the .cpp file this also needs to be within the namespace Bengine for it to be ok in the .cpp

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		// After adding a particle batch, the ParticleEngine2D becomes
		// responsible for deallocation.
		void addParticleBatch(ParticleBatch2D* particleBatch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch2D*> m_batches;
	};

}