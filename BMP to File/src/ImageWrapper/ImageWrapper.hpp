#ifndef _IMAGEWRAPPER_HPP_
#define _IMAGEWRAPPER_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include "../../../SharedCode/BinaryFile/BinaryFile.hpp"

class ImageWrapper {
	private:
		sf::Image img;
		BinaryFile outputFile;

		bool loadedFlag;

	public:
		ImageWrapper();
		~ImageWrapper() {}

		void Load(const std::string& pathWname);
		void ExportFile(bool codingType, bool grayscale);
};

#endif
