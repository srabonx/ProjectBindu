#ifndef TILELAYER_H
#define TILELAYER_H

#include "Layer.h"
#include "Scene.h"
#include "Sprite.h"

namespace  BINDU
{


	using TileMap = std::vector<std::unique_ptr<Tile>>;

	class TileLayer: public Layer
	{
	private:
		std::vector<std::vector<int>>							   m_mapData;
		TileMap													   m_tiles;
		std::vector<TileSet*>									   m_tileSets;

		int			m_width{};
		int			m_height{};

		int			m_tileWidth{};
		int			m_tileHeight{};

		Timer       m_timer;

	public:

		TileLayer() = default;
		TileLayer(const TileLayer&) = delete;
		TileLayer& operator = (TileLayer&) = delete;
		~TileLayer() = default;

		void			onLoadResource();

		void			onResetTilemap();

		void			Update(float dt) override;

		void			Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const override;

		void			ProcessInput() const override;

		inline void		setWidth(const int width) { m_width = width; }
		inline int		getWidth() const { return m_width; }

		inline void		setHeight(const int height) { m_height = height; }
		inline int		getHeight() const { return m_height; }

		inline void		setMapData(const std::vector<std::vector<int>>& mapData) { m_mapData = mapData; }
		std::vector<std::vector<int>>& getMapData() { return m_mapData; }

		void			setTileSet(TileSet* tileSets) { m_tileSets.push_back(tileSets); }


		inline Tile* getTile(int row, int column) const { row = row * (m_width - 1); row = row + column; return m_tiles[row].get(); }

		TileMap&		getTileMap() { return m_tiles; }
		inline void		setTileMap(TileMap tileMap) { m_tiles = std::move(tileMap); }

		inline void		setWorldTileWidth(const int width) { m_tileWidth = width; }
		inline int		getWorldTileWidth() const { return m_tileWidth; }

		inline void		setWorldTileHeight(const int height) { m_tileHeight = height; }
		inline int		getWorldTileHeight() const { return m_tileHeight; }

	};

	
};


#endif // TILELAYER_H
