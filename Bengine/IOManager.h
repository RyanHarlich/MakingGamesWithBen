#pragma once

#include <string>
#include <vector>


namespace Bengine {



	struct DirEntry {
		std::string path;
		bool isDirectory;
	};



	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

		static bool readFileToBuffer(std::string filePath, std::string& buffer);

		static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);

		static bool makeDirectory(const char* path);
	};

}

