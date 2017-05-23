#include "UploadStances.h"

#include <Bengine/ResourceManager.h>


UploadStances::UploadStances()
{
}


UploadStances::~UploadStances()
{
}





NumStances UploadStances::uploadZombieStances(std::unordered_map<unsigned int, GLuint>& zombieStanceIDs) {

	NumStances numStances;


	int i = 0;
	numStances.fMove = i; // assign first move element;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_0.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_1.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_2.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_3.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_4.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_5.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_6.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_7.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_8.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_9.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_10.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_11.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_12.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_13.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_14.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_15.png").id;
	zombieStanceIDs[i++] = Bengine::ResourceManager::getTexture("Textures/Agent/Zombie/Moving/skeleton-move_16.png").id;
	numStances.lMove = i - 1; // assign last move element;



	return numStances;
}












NumStances UploadStances::uploadHumanStances(std::unordered_map<unsigned int, GLuint>& humanStanceIDs) {

	NumStances numStances;


	// Sprite Sheet
	numStances.fMove = 1; // assign first move element;
	humanStanceIDs[0] = Bengine::ResourceManager::getTexture("Textures/Agent/Human/Move/walking1.png").id;
	numStances.lMove = 6; // assign last move element;



	return numStances;


}















NumStances UploadStances::uploadPlayerStances(std::unordered_map<unsigned int, GLuint>& playerStanceIDs) {

	NumStances numStances;
	int counter = 0;

	// Idles needs to go first so idle is element 0 to start off with
	playerPistolIdle(playerStanceIDs, numStances, counter);
	playerPistolMove(playerStanceIDs, numStances, counter);
	playerPistolShoot(playerStanceIDs, numStances, counter);

	playerShotgunIdle(playerStanceIDs, numStances, counter);
	playerShotgunMove(playerStanceIDs, numStances, counter);
	playerShotgunShoot(playerStanceIDs, numStances, counter);

	playerRifleIdle(playerStanceIDs, numStances, counter);
	playerRifleMove(playerStanceIDs, numStances, counter);
	playerRifleShoot(playerStanceIDs, numStances, counter);

	return numStances;
}





void UploadStances::playerPistolIdle(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {

	numStances.fPistolIdle = counter;  // assign fist idle element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_2.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_3.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_4.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_5.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_6.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_7.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_8.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_9.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_10.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_11.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_12.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_13.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_14.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_15.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_16.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_17.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_18.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/idle/survivor-idle_handgun_19.png").id;
	numStances.lPistolIdle = counter - 1; // assign last idle element

}

void UploadStances::playerPistolMove(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {




	numStances.fPistolMove = counter;  // assign fist move element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_2.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_3.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_4.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_5.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_6.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_7.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_8.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_9.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_10.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_11.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_12.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_13.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_14.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_15.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_16.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_17.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_18.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/move/survivor-move_handgun_19.png").id;
	numStances.lPistolMove = counter - 1; // assign last move element


}






void UploadStances::playerPistolShoot(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {


	numStances.fPistolShoot = counter;  // assign fist shoot element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/shoot/survivor-shoot_handgun_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/shoot/survivor-shoot_handgun_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/handgun/shoot/survivor-shoot_handgun_2.png").id;
	numStances.lPistolShoot = counter - 1; // assign last shoot element


}






void UploadStances::playerShotgunIdle(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {

	numStances.fShotgunIdle = counter;  // assign fist idle element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_2.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_3.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_4.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_5.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_6.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_7.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_8.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_9.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_10.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_11.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_12.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_13.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_14.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_15.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_16.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_17.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_18.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/idle/survivor-idle_shotgun_19.png").id;
	numStances.lShotgunIdle = counter - 1; // assign last idle element

}






void UploadStances::playerShotgunMove(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {
	numStances.fShotgunMove = counter;  // assign fist move element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_2.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_3.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_4.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_5.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_6.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_7.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_8.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_9.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_10.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_11.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_12.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_13.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_14.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_15.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_16.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_17.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_18.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/move/survivor-move_shotgun_19.png").id;
	numStances.lShotgunMove = counter - 1; // assign last move element
}







void UploadStances::playerShotgunShoot(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {
	numStances.fShotgunShoot = counter;  // assign fist shoot element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/shoot/survivor-shoot_shotgun_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/shoot/survivor-shoot_shotgun_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/shotgun/shoot/survivor-shoot_shotgun_2.png").id;
	numStances.lShotgunShoot = counter - 1; // assign last shoot element
}







void UploadStances::playerRifleIdle(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {
	numStances.fRifleIdle = counter;  // assign fist idle element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_2.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_3.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_4.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_5.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_6.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_7.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_8.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_9.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_10.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_11.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_12.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_13.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_14.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_15.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_16.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_17.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_18.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/idle/survivor-idle_rifle_19.png").id;
	numStances.lRifleIdle = counter - 1; // assign last idle element
}








void UploadStances::playerRifleMove(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {
	numStances.fRifleMove = counter;  // assign fist move element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_2.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_3.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_4.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_5.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_6.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_7.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_8.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_9.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_10.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_11.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_12.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_13.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_14.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_15.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_16.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_17.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_18.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/move/survivor-move_rifle_19.png").id;
	numStances.lRifleMove = counter - 1; // assign last move element
}







void UploadStances::playerRifleShoot(std::unordered_map<unsigned int, GLuint>& playerStanceIDs, NumStances& numStances, int& counter) {
	numStances.fRifleShoot = counter;  // assign fist shoot element
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/shoot/survivor-shoot_rifle_0.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/shoot/survivor-shoot_rifle_1.png").id;
	playerStanceIDs[counter++] = Bengine::ResourceManager::getTexture("Textures/Agent/Player/rifle/shoot/survivor-shoot_rifle_2.png").id;
	numStances.lRifleShoot = counter - 1; // assign last shoot element

}






