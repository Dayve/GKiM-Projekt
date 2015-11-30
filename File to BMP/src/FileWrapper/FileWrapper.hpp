#ifndef _FILEWRAPPER_HPP_
#define _FILEWRAPPER_HPP_

#include <SFML/Graphics.hpp>

class FileWrapper {
	public:
		void Load(const std::string& filename);
		void ExportBMP(const std::string& dataDir);

};

#endif
