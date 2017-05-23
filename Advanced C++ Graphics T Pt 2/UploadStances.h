#pragma once

#include <unordered_map>

#include <GL/glew.h>


struct NumStances {

	// Normal
	unsigned int fMove;
	unsigned int lMove;
	unsigned int fIdle;
	unsigned int lIdle;
	unsigned int fShoot;
	unsigned int lShoot;

	// Special Weopons
	// Pistol
	unsigned int fPistolMove;
	unsigned int lPistolMove;
	unsigned int fPistolIdle;
	unsigned int lPistolIdle;
	unsigned int fPistolShoot;
	unsigned int lPistolShoot;

	// Shotgun
	unsigned int fShotgunMove;
	unsigned int lShotgunMove;
	unsigned int fShotgunIdle;
	unsigned int lShotgunIdle;
	unsigned int fShotgunShoot;
	unsigned int lShotgunShoot;

	// Rifle
	unsigned int fRifleMove;
	unsigned int lRifleMove;
	unsigned int fRifleIdle;
	unsigned int lRifleIdle;
	unsigned int fRifleShoot;
	unsigned int lRifleShoot;

};


class UploadStances
{
public:
	UploadStances();
	~UploadStances();

	NumStances uploadZombieStances(std::unordered_map<unsigned int, GLuint>& zombieStanceIDs);

	NumStances uploadHumanStances(std::unordered_map<unsigned int, GLuint>& humanStanceIDs);

	NumStances uploadPlayerStances(std::unordered_map<unsigned int, GLuint>& playerStanceIDs);

	void playerPistolIdle(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerPistolMove(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerPistolShoot(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerShotgunIdle(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerShotgunMove(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerShotgunShoot(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerRifleIdle(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerRifleMove(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);

	void playerRifleShoot(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter);
};

