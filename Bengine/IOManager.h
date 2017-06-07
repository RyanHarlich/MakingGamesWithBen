#pragma once

#include <string>
#include <vector>


namespace Bengine {


	/* NEW */
	struct DirEntry {
		std::string path;
		bool isDirectory;
	};
	/* NEW: end of new */


	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

		static bool readFileToBuffer(std::string filePath, std::string& buffer);

		/* NEW */
		static bool getDirectoryEntries(const char* path, std::vector<DirEntry>& rvEntries);
		/* NEW */
		static bool makeDirectory(const char* path);
	};

}

