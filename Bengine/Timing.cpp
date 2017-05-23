#include "Timing.h"

/* NEW: moved to Timing.h */
//#include <SDL/SDL.h>




namespace Bengine {

	FpsLimiter::FpsLimiter() {

	}






	void FpsLimiter::init(float maxFPS) {
		setMaxFPS(maxFPS);
	}






	void FpsLimiter::setMaxFPS(float maxFPS) {
		m_maxFPS = maxFPS;
	}






	void FpsLimiter::begin() {
		m_startTicks = SDL_GetTicks();
	}







	float FpsLimiter::end() {
		calculateFPS();

		// Limits the frame rate

		/* NEW: changed to Uint32 */
		//float frameTicks = (float)SDL_GetTicks() - (float)m_startTicks;
		Uint32 frameTicks = SDL_GetTicks() - m_startTicks;


		if (1000.0f / m_maxFPS > frameTicks) {
			SDL_Delay((Uint32)(1000.0f / m_maxFPS - frameTicks));
		}
		return m_fps;
	}








	void FpsLimiter::calculateFPS() {
		static const int NUM_SAMPLES = 10;


		/* NEW: changed to Uint32 */
		//static float frameTimes[NUM_SAMPLES];
		static Uint32 frameTimes[NUM_SAMPLES];


		static int currentFrame = 0;


		/* NEW: changed to Uint32 */
		//static float prevTicks = (float)SDL_GetTicks();
		static Uint32 prevTicks = SDL_GetTicks();


		/* NEW: changed to Uint32 */
		//float currentTicks;
		Uint32 currentTicks;

		/* NEW: changed to Uint32 */
		//currentTicks = (float)SDL_GetTicks();
		currentTicks = SDL_GetTicks();


		m_frameTime = currentTicks - prevTicks;


		// when currentFrame becomes 11, 11 % 10 = 1
		frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;
		
		prevTicks = currentTicks;


		int count;
		++currentFrame;


		if (currentFrame < NUM_SAMPLES) {	
			count = currentFrame; //only for the first 10 frames, then it is maxed out at 10
		}
		else {		
			count = NUM_SAMPLES; // maxes out at 10, from here it always averages the max 10
		}



		float frameTimeAverage = 0;
		for (int i = 0; i < count; ++i) {
			frameTimeAverage += frameTimes[i];
		}
		frameTimeAverage /= count;



		if (frameTimeAverage > 0) {
			m_fps = 1000.0f / frameTimeAverage;
		}
		else {
			m_fps = 60.0f;
		}
	}

}