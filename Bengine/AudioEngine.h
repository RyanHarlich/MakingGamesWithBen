#pragma once

#include <string>
#include <map>

#include <SDL/SDL_mixer.h>

/// Doxign style commenting

namespace Bengine {

	class SoundEffect {
	public:
		friend class AudioEngine;

		/// Plays the effect file
		/// @param loops: If loops == -1, loop forever,
		/// otherwise play it loops + 1 time
		void play(int loops = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;
	};


	class Music {
	public: 
		friend class AudioEngine;

		/// Plays the audio file
		/// @param loops: If loops == -1, loop forever,
		/// otherwise play it loops time
		void play(int loops = -1);

		// Like global functions: Only for the current song playing, not for this music
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
		// can play multiple effects at once
		std::map<std::string, Mix_Chunk*> m_effectMap;  // key, value
		// can only play one music at a time
		std::map<std::string, Mix_Music*> m_musicMap;
		bool m_isInitialized = false;
	};

}