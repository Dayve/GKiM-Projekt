#ifndef _IMAGEWRAPPER_HPP_
#define _IMAGEWRAPPER_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include <cmath>	// For pow() in block::set

#include "../../../SharedCode/BinaryFile/BinaryFile.hpp"

class ImageWrapper {
	private:
		sf::Image img;
		BinaryFile outputFile;

		bool loadedFlag;

	public:
		ImageWrapper();
		~ImageWrapper() {}

		void Load(const std::string& fullPath);
		void ExportFile(bool codingType, bool grayscale, const std::string& dataDir);
};

#endif
