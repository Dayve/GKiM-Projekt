#ifndef _IMAGEWRAPPER_HPP_
#define _IMAGEWRAPPER_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

#include "../ArithmeticCoder/ArithmeticCoder.hpp"

class ImageWrapper {
	private:
		sf::Image img;
		bool loadedFlag;
		std::vector<sf::Uint8> buffer;
		const int NR_BITS = 5;

		ArithmeticCoder AC;

	public:
		ImageWrapper();
		~ImageWrapper();

		void Load(const std::string& filename);
		void ExportFile(bool codingType, bool grayscale, const std::string& dataDir);
};

#endif
