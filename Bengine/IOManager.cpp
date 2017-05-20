#include <fstream>

#include "IOManager.h"




namespace Bengine {

	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer) {

		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		file.seekg(0, std::ios::end);
		std::streamsize fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		fileSize -= file.tellg();

		buffer.resize((const int)fileSize);

		//reads number of bytes into buffer
		file.read((char *)&buffer[0], fileSize);

		file.close();

		return true;
	}

}