#include <fstream>
#include <filesystem> /* std::tr2::sys:: */


#include "IOManager.h"



namespace fs = std::tr2::sys;


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

	




	bool IOManager::readFileToBuffer(std::string filePath, std::string& buffer) {
		
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



	bool IOManager::getDirectoryEntries(const char * path, std::vector<DirEntry>& rvEntries) {
		auto dpath = fs::path(path);
		// Must be directory
		if (!fs::is_directory(dpath)) return false;

		for (auto it = fs::directory_iterator(dpath); it != fs::directory_iterator(); ++it) {
			rvEntries.emplace_back();
			rvEntries.back().path = it->path().string();
			if (is_directory(it->path())) {
				rvEntries.back().isDirectory = true;
			}
			else {
				rvEntries.back().isDirectory = false;
			}
		}
		return true;
	}




	bool IOManager::makeDirectory(const char * path) {
		return fs::create_directory(fs::path(path));
	}

}