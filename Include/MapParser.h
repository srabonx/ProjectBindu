#ifndef MAPPARSER_H
#define MAPPARSER_H

#include "Scene.h"
#include "TileLayer.h"
#include "tinyxml2.h"

namespace BINDU
{


	class MapParser
	{
	private:
		std::string				 m_resourcePath;

	private:

		void					 ParseMap(const char* filepath,Scene& scene);

		std::unique_ptr<TileSet> ParseTileSet(tinyxml2::XMLElement* tileSetElement);

		std::unique_ptr<TileLayer> ParseLayer(tinyxml2::XMLElement* tileLayerElement, std::vector<std::unique_ptr<TileSet>> * tileSets);

	public:
		MapParser() = default;
		MapParser(const MapParser&) = delete;
		MapParser& operator =(const MapParser&) = delete;
		~MapParser() = default;


		void					Load(const char* filepath,Scene& scene);									// filepath of .xml or .tmx containing the map information

		inline void				setResourcePath(const char* resourceRootPath) { m_resourcePath = resourceRootPath; }
		inline std::string		getResourcePath() const { return m_resourcePath; }

	};

};


#endif // MAPPARSER_H
