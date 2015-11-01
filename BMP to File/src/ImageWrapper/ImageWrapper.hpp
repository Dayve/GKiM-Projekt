#ifndef _IMAGEWRAPPER_HPP_
#define _IMAGEWRAPPER_HPP_

#include <SFML/Graphics.hpp>

class ImageWrapper {
	private:
		sf::Image img;

	public:
		void Load(const std::string &filename);
};

#endif
