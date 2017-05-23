#pragma once 

/* NEW: moved from Timing.cpp */
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

		/* NEW: changed to Uint32 */
		//unsigned int m_startTicks; 
		Uint32 m_startTicks;


		float m_fps;


		/* NEW: changed to Uint32 */
		//float m_frameTime;
		Uint32 m_frameTime;


	};

}