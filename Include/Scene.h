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

	struct TileSet;

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
		int			gId{};
		Vec2f		position{};
		Bnd_Size_F	size{};
		Sprite		sprite{};
		bool		isAnimated{ false };
		bool		isCollideAble{ true };
		Vec2f		collisionNormal{};
		Timer		frameTimer;
		Animator    animator;
		int			currentFrame{};
		int			frameCount{};
		std::vector<TileFrame> frames;
		TileSet*	tileSet;
	};

	struct TileSet
	{
	public:

		std::string name{};
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
	protected:


		bool	m_isActive{};

		std::string m_guid{};

		std::vector<std::unique_ptr<TileSet>>	 m_tileSets;

		std::vector<std::unique_ptr<Layer>> m_layers;

		int		m_wTileWidth{};
		int		m_wTileHeight{};

	public:

		Scene() = default;
		virtual ~Scene() = default;

		virtual void	onLoadResource();
		virtual void    onReleaseResource();

		virtual void	Update(float dt) ;
		virtual void	Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) ;
		virtual void	ProcessInput() ;

		void			AddLayer(std::unique_ptr<Layer> sceneLayer, const char* guid);

		std::unique_ptr<Layer> RemoveLayer(const char* guid);

		Layer*			getLayer(const char* guid);

		inline void		setActive(const bool value) { m_isActive = value; }

		inline bool		isActive() const { return m_isActive; }

		inline int		getLayerCount() const { return static_cast<int>(m_layers.size()); }

		inline void		setGuid(const char* guid) { m_guid = guid; }

		inline std::string getGuid() const { return m_guid; }

		void		addTileSet(std::unique_ptr<TileSet> tileSet) { m_tileSets.push_back(std::move(tileSet)); }
		std::vector<std::unique_ptr<TileSet>>& getTileSets() { return m_tileSets; }

		inline void		setWorldTileWidth(const int width) { m_wTileWidth = width; }
		inline int		getWorldTileWidth() const { return m_wTileWidth; }

		inline void		setWorldTileHeight(const int height) { m_wTileHeight = height; }
		inline int		getWorldTileHeight() const { return m_wTileHeight; }


	};// Class

};

#endif // SCENE_H
