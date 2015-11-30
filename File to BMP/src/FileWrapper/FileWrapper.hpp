#ifndef _FILEWRAPPER_HPP_
#define _FILEWRAPPER_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include "../../../SharedCode/BinaryFile/BinaryFile.hpp"

class FileWrapper {
	private:
		BinaryFile inputFile;
		sf::Image resultImg;

		bool loadedFlag;

	public:
		FileWrapper();
		~FileWrapper() {}

		void Load(const std::string& fullPath);
		void ExportBMP(const std::string& dataDir);

};

#endif
