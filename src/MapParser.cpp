
#include "Include/MapParser.h"

#include <cassert>
#include <sstream>
#include <memory>

namespace BINDU
{

	void MapParser::Load(const char* filepath,Scene& scene)
	{
		ParseMap(filepath,scene);
	}

	void MapParser::ParseMap(const char* filepath,Scene& scene)
	{
		tinyxml2::XMLDocument doc;

		doc.LoadFile(filepath);

		assert(!doc.Error());

		tinyxml2::XMLElement* root = doc.RootElement();

		scene.setWorldTileWidth(root->IntAttribute("tilewidth"));
		scene.setWorldTileHeight(root->IntAttribute("tileheight"));

		std::vector<std::unique_ptr<TileSet>> tileSets ;

		for (tinyxml2::XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
		{
			if (std::strcmp(e->Value(), "tileset") == 0)
			{
				tileSets.push_back(ParseTileSet(e));
			}
		}

		for (tinyxml2::XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
		{
			if (std::strcmp(e->Value(), "layer") == 0)
				scene.AddLayer(ParseLayer(e,&tileSets), e->Attribute("name"));
		}

		scene.setTileSets(std::move(tileSets));

	}

	std::unique_ptr<TileSet> MapParser::ParseTileSet(tinyxml2::XMLElement* tileSetElement)
	{
		std::unique_ptr<TileSet> tileSet = std::make_unique<TileSet>();

		tileSet->name = tileSetElement->Attribute("name");
		tileSet->tileWidth = std::strtol(tileSetElement->Attribute("tilewidth"), nullptr, 0);
		tileSet->tileHeight = std::strtol(tileSetElement->Attribute("tileheight"),nullptr,0);
		tileSet->tileCount = std::strtol(tileSetElement->Attribute("tilecount"), nullptr,0);
		tileSet->columnCount = std::strtol(tileSetElement->Attribute("columns"), nullptr, 0);
		tileSet->rowCount = tileSet->tileCount / tileSet->columnCount;
		tileSet->firstGid = tileSetElement->IntAttribute("firstgid");
		tileSet->lastGid = tileSet->firstGid + tileSet->tileCount - 1;

		if(tileSetElement->Attribute("tilerendersize") != nullptr)
		{
			tileSet->tileRenderSize = tileSetElement->Attribute("tilerendersize");
		}

		for (tinyxml2::XMLElement* e = tileSetElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
		{
			if (std::strcmp(e->Value(), "image") == 0)
			{
				tileSet->source = m_resourcePath + e->Attribute("source");
			}
			if (std::strcmp(e->Value(), "tile") == 0)
			{
				std::unique_ptr<Tile> tile = std::make_unique<Tile>();


				tile->id = e->IntAttribute("id");
				tile->isAnimated = true;

				for(tinyxml2::XMLElement* a = e->FirstChildElement(); a!= nullptr; a = a->NextSiblingElement() )
				{
					if (std::strcmp(a->Value(), "animation") == 0)
					{
						for(tinyxml2::XMLElement* f = a->FirstChildElement() ; f!=nullptr; f = f->NextSiblingElement())
						{
							TileFrame tileFrame;

							tileFrame.tileId = f->IntAttribute("tileid");
							tileFrame.duration = f->IntAttribute("duration");

							tile->frames.push_back(tileFrame);
						}						
					}
				}
				tile->frameCount = static_cast<int>(tile->frames.size());
				tileSet->tiles.push_back(std::move(tile));
			}
		}



		return tileSet;

	}

	std::unique_ptr<TileLayer> MapParser::ParseLayer(tinyxml2::XMLElement* tileLayerElement, std::vector<std::unique_ptr<TileSet>>* tileSets)
	{
		std::unique_ptr<TileLayer> mpLayer = std::make_unique<TileLayer>();

		mpLayer->setGuid(tileLayerElement->Attribute("name"));

		mpLayer->setWidth(std::strtol(tileLayerElement->Attribute("width"), nullptr, 0));
		mpLayer->setHeight(std::strtol(tileLayerElement->Attribute("height"), nullptr, 0));

		float parallaxX = tileLayerElement->FloatAttribute("parallaxx",1.0f);
		float parallaxY = tileLayerElement->FloatAttribute("parallaxy",1.0f);

		mpLayer->setOffsetX(tileLayerElement->FloatAttribute("offsetx"));
		mpLayer->setOffsetY(tileLayerElement->FloatAttribute("offsety"));

		mpLayer->setActive(tileLayerElement->IntAttribute("visible", 1));

		mpLayer->setParallaxFactor({ parallaxX,parallaxY });

		tinyxml2::XMLElement* data = nullptr;

		for (tinyxml2::XMLElement* e = tileLayerElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
		{
			if (std::strcmp(e->Value(), "data")==0)
			{
				data = e;
				break;
			}
		}

		std::string mapMatrix(data->GetText());

		std::istringstream iss(mapMatrix);

		std::string id;

		std::vector<std::vector<int>> mapData(mpLayer->getHeight(), std::vector<int>(mpLayer->getWidth(), 0));

		for(int row = 0 ; row < mpLayer->getHeight(); row++)
		{
			for(int column = 0; column < mpLayer->getWidth();column++)
			{
				std::getline(iss, id, ',');
				std::stringstream converter(id);
				converter >> mapData[row][column];

				if (!iss.good())
					break;

			}
		}

		for (auto& m:*tileSets)
		{
			TileSet* pTileSet = m.get();
			mpLayer->setTileSet(pTileSet);
		}

		mpLayer->setMapData(mapData);

		return std::move(mpLayer);
	}
};