#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h" // still need this to use the function with forward declaration


namespace Bengine {

	// This also works instead of wrapping the namespace around everything, but would have to say Bengine:: alot
	Bengine::ParticleEngine2D::ParticleEngine2D()
	{
	}






	Bengine::ParticleEngine2D::~ParticleEngine2D()
	{
		for (auto& b : m_batches) {
			delete b;
		}
	}





	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch) {
		m_batches.push_back(particleBatch);
	}





	void ParticleEngine2D::update(float deltaTime) {
		for (auto& b : m_batches) {
			b->update(deltaTime);
		}
	}






	void ParticleEngine2D::draw(SpriteBatch* spriteBatch) {
		for (auto& b : m_batches) {
			spriteBatch->begin();
			b->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}
	}

}