#pragma once

#include <string>
#include <vector>


namespace Bengine {

	class IOManager
	{
	public:
		static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);

		/* NEW: overloaded */
		static bool readFileToBuffer(std::string filePath, std::string& buffer);
	};

}

