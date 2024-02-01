#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>

#include "Animator.h"
#include "Layer.h"
#include "Sprite.h"
#include "TImer.h"

namespace BINDU
{

	struct TileFrame
	{
		int tileId{};
		int duration{};
		Timer		frameTimer;
	};

	struct Tile
	{

	public:

		int			id{};
		Vec2f		position{};
		Bnd_Size_F	size{};
		Sprite		sprite{};
		bool		isAnimated{ false };
		Timer		frameTimer;
		Animator    animator;
		int			currentFrame{};
		int			frameCount{};
		std::vector<TileFrame> frames;

	};

	struct TileSet
	{
	public:

		const char* name = nullptr;
		int			tileWidth{};
		int			tileHeight{};
		int			firstGid{};
		int			lastGid{};
		int			tileCount{};
		int			columnCount{};
		int			rowCount{};

		std::string tileRenderSize ;

		std::string source{};

		std::vector < std::unique_ptr<Tile>> tiles;

		Texture		tileTexture;

	};

	class Scene
	{
	private:


		bool	m_isActive{};

		std::string m_guid{};

		std::vector<std::unique_ptr<TileSet>>	 m_tileSets;

		std::vector<std::unique_ptr<Layer>> m_layers;

		int		m_wTileWidth{};
		int		m_wTileHeight{};

	public:

		Scene() = default;
		~Scene() = default;

		void			onLoadResource();

		void			Update(float dt) const;
		void			Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const;
		void			ProcessInput() const;

		void			AddLayer(std::unique_ptr<Layer> sceneLayer, const char* guid);

		std::unique_ptr<Layer> RemoveLayer(const char* guid);

		Layer*			getLayer(const char* guid);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline int		getLayerCount() const { return static_cast<int>(m_layers.size()); }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

		void		setTileSets(std::vector<std::unique_ptr<TileSet>> tileSets) { m_tileSets = std::move(tileSets); }
		std::vector<std::unique_ptr<TileSet>>& getTileSets() { return m_tileSets; }

		inline void		setWorldTileWidth(const int width) { m_wTileWidth = width; }
		inline int		getWorldTileWidth() const { return m_wTileWidth; }

		inline void		setWorldTileHeight(const int height) { m_wTileHeight = height; }
		inline int		getWorldTileHeight() const { return m_wTileHeight; }


	};// Class

};

#endif // SCENE_H
