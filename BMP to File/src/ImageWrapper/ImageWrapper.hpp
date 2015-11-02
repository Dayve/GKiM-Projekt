#ifndef _IMAGEWRAPPER_HPP_
#define _IMAGEWRAPPER_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

class ImageWrapper {
	private:
		sf::Image img;
		bool loadedFlag;
		std::vector<sf::Uint8> buffer;

	public:
		ImageWrapper();
		~ImageWrapper();

		void Load(const std::string& filename);
		void ExportFile(bool codingType, bool grayscale, const std::string& dataDir);
};

#endif
