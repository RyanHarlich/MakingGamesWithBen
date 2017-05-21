#pragma once 



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
		unsigned int m_startTicks; 
		float m_fps;
		float m_frameTime;


	};

}