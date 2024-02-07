
#include "Include/TileLayer.h"

namespace BINDU
{
	void TileLayer::onLoadResource()
	{
		//for (int i = 0; i < m_height; i++)
		//	m_tiles.push_back(std::vector<std::unique_ptr<Tile>>(m_width));

		for (int row = 0; row < m_height; row++)
		{
			for (int column = 0; column < m_width; column++)
			{

					int tileNum = m_mapData[row][column];

					if (tileNum == 0)
					{
						m_tiles.push_back(nullptr);
						continue;
					}

					TileSet* tileSet = nullptr;

					for (auto& m : m_tileSets)
					{
						if (tileNum <= m->lastGid && tileNum >= m->firstGid)
						{
							tileSet = m;
							break;
						}
					}

					tileNum -= tileSet->firstGid;

					std::unique_ptr<Tile> tile = std::make_unique<Tile>();


					for (auto& m : tileSet->tiles)
					{
						if (m->id == tileNum)
						{
							tile->frames = m->frames;
							tile->frameCount = m->frameCount;
							tile->currentFrame = m->currentFrame;
							tile->isAnimated = m->isAnimated;
						}
					}

					tile->id = tileNum;

					if(tileSet->tileRenderSize == "grid")
					{
						tile->position = { m_offsetX + static_cast<float>(m_tileWidth * column), m_offsetY + static_cast<float>(m_tileHeight * row) };
						tile->size = { static_cast<float>(m_tileWidth),static_cast<float>(m_tileHeight) };
					}
					else
					{
						tile->position = { m_offsetX + static_cast<float>(m_tileWidth * column), m_offsetY + static_cast<float>(m_tileHeight * row) };
						tile->size = { static_cast<float>(tileSet->tileWidth),static_cast<float>(tileSet->tileHeight) };
					}

				/*	if (tile->isAnimated)
					{
						tile->animator.setTotalColumn(tileSet->columnCount);
						tile->animator.setTotalFrame(tileSet->tileCount);
						tile->animator.setFrameSize(static_cast<float>(tileSet->tileWidth), static_cast<float>(tileSet->tileHeight));
						tile->animator.setFrameTime(100);
					}*/

					tile->animator.setTotalColumn(tileSet->columnCount);
					tile->animator.setTotalFrame(tileSet->tileCount);
					tile->animator.setFrameSize(static_cast<float>(tileSet->tileWidth), static_cast<float>(tileSet->tileHeight));


					int fx = tileNum % tileSet->columnCount * tileSet->tileWidth;
					int fy = tileNum / tileSet->columnCount * tileSet->tileHeight;

					Bnd_Rect_F srcRect{ static_cast<float>(fx),static_cast<float>(fy),static_cast<float>(tileSet->tileWidth),static_cast<float>(tileSet->tileHeight) };

					tile->sprite.SetTexture(tileSet->tileTexture);

					tile->sprite.setRect(srcRect);

					m_tiles.push_back(std::move(tile));
			}
				
			
		}
	}

	void TileLayer::onResetTilemap()
	{
		m_tiles.clear();
		onLoadResource();
	}

	void TileLayer::Update(float dt)
	{
		Layer::Update(dt);

		for(auto& tile:m_tiles)
		{
			if(tile)
			{
				tile->sprite.setPosition(tile->position);
				tile->sprite.setSize(tile->size);


				if (tile->isAnimated)
				{
					if (tile->frameTimer.stopwatch(tile->frames[tile->currentFrame].duration))
					{
						++tile->currentFrame;
						if (tile->currentFrame >= tile->frameCount)
							tile->currentFrame = 0;
					}

					tile->animator.setCurrentFrame(tile->frames[tile->currentFrame].tileId + 1);

					Bnd_Rect_F srcRect = tile->animator.getCurrentFrame();
					tile->sprite.setRect(srcRect);
				}
			}
		}

	}

	void TileLayer::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const
	{
		Layer::Draw(graphics, cameraMatrix);

		D2D1_MATRIX_3X2_F cameraMatrixWithParallax = cameraMatrix;
		cameraMatrixWithParallax.dx = cameraMatrix.dx * m_parallaxFactor.x;
		cameraMatrixWithParallax.dy = cameraMatrix.dy * m_parallaxFactor.y;

		for(auto& tile:m_tiles)
		{
			if(tile)
			{
				tile->sprite.Draw(graphics, cameraMatrixWithParallax);
			}
		}


	}

	void TileLayer::ProcessInput() const
	{
		Layer::ProcessInput();
	}
};