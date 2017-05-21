#include "AudioEngine.h"

#include "BengineErrors.h"

namespace Bengine {

	// Can play multiple sound effect on differnt channels
	void SoundEffect::play(int loops /* = 0 */) {
		// play on what channel (-1 for the first free channel)
		// what Mix_Chunk*
		// loop (-1 for ever, 0 for 1 time, 1 will play 2 times since it loops once and so on)
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
			if (Mix_PlayChannel(0, m_chunk, loops) == -1) { // override channel 0 as a backup
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}

		// can pause and repeat but have to keep track of channel, Mix_PlayChannel returns channel or -1 for error
	}



	// Can only play one music at a time
	void Music::play(int loops /* = -1 */) {
		if (Mix_PlayMusic(m_music, loops) == -1) {
			fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
		}
	}





	void Music::pause() {
		Mix_PauseMusic();
	}





	void Music::stop() {
		Mix_HaltMusic();
	}





	void Music::resume() {
		Mix_ResumeMusic();
	}





	AudioEngine::AudioEngine() {
	}





	AudioEngine::~AudioEngine() {
		// Extra safty net
		destory();
	}





	void AudioEngine::init() {
		// Parameter can be a bitwise combination of MIX_INIT_FAC, MIX_INIT_MOD, MX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			fatalError("Mix_Init error" + std::string(Mix_GetError()));
		}
		
		// for mp3 use twice the default frequency which is 22050
		// channels, 1 for mono, 2 for stereo
		// default format
		// last argument needs to be a power of 2, not sure what it is of bytes are in a chunck of audio
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}





	void AudioEngine::destory() {
		if (m_isInitialized) {
			m_isInitialized = false;
			Mix_Quit();
		}
	}






	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath) {

		// Try to find the audio in the cache
		auto it = m_effectMap.find(filePath);

		SoundEffect effect;

		if (it == m_effectMap.end()) {
			// Failed to find it, must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str()); // can load any file including wav that was initialized at the beginning
			if (chunk == nullptr) { 
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			effect.m_chunk = chunk;
			m_effectMap[filePath] = chunk; // associate array: will create a new node
		}
		else {
			// Its already cached
			effect.m_chunk = it->second;
		}
		return effect;
	}





	Music AudioEngine::loadMusic(const std::string& filePath) {

		// Try to find the audio in the cache
		auto it = m_musicMap.find(filePath);

		Music music;

		if (it == m_musicMap.end()) {
			// Failed to find it, must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str()); // can load any file that was initialized at the beginning
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}

			music.m_music = mixMusic;
			m_musicMap[filePath] = mixMusic; // associate array: will create a new node
		}
		else {
			// Its already cached
			music.m_music = it->second;
		}
		return music;
	}
}