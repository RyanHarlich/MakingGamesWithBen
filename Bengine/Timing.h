#pragma once 

#include <SDL/SDL.h>


namespace Bengine {

	class FpsLimiter {
	public:
		FpsLimiter();

		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void begin();

		float end();


	private:
		void calculateFPS();


		float m_maxFPS;
		Uint32 m_startTicks;
		float m_fps;
		Uint32 m_frameTime;


	};

}