#pragma once

#include <string>
#include <map>

#include <SDL/SDL_mixer.h>



namespace Bengine {

	class SoundEffect {
	public:
		friend class AudioEngine;

		/// @param loops: If loops == -1, loop forever,
		/// otherwise play it loops + 1 time
		void play(int loops = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;
	};


	class Music {
	public: 
		friend class AudioEngine;

		/// @param loops: If loops == -1, loop forever,
		/// otherwise play it loops time
		void play(int loops = -1);


		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* m_music = nullptr;
	};


	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destory();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:
		std::map<std::string, Mix_Chunk*> m_effectMap;  // key, value
		std::map<std::string, Mix_Music*> m_musicMap;
		bool m_isInitialized = false;
	};

}