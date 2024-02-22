
#include "Include/Scene.h"

#include <locale>
#include <sstream>

#include "Include/BitmapLoader.h"
#include "Include/TileLayer.h"

namespace BINDU
{
	void Scene::onLoadResource()
	{
		for(const auto& tileset:m_tileSets)
		{

			std::wostringstream wstm;

			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());

			for (const char i : tileset->source)
				wstm << ctfacet.widen(i);

			tileset->tileTexture.LoadFromFile(tileset->source.c_str());
		}

		for(const auto& layer:m_layers)
		{
			if (TileLayer* tLayer = dynamic_cast<TileLayer*>(layer.get()))
			{
				tLayer->setWorldTileWidth(m_wTileWidth);
				tLayer->setWorldTileHeight(m_wTileHeight);
				tLayer->onLoadResource();
			}
			else
				layer->onLoadResource();
		}


	}

	void Scene::onReleaseResource()
	{
		for (const auto& layer : m_layers)
			layer->onReleaseResource();

		m_layers.clear();
	}

	void Scene::Update(float dt)
	{
		for(const auto& m: m_layers)
		{
			if (m->isActive() && m->isUpdating())
			{
				m->Update(dt);
				m->CheckCollision();
			}
		}
	}

	void Scene::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
	{
		for (const auto& m : m_layers)
		{
			if (m->isActive())
			{
				m->Draw(graphics, cameraMatrix);
			}
		}
	}

	void Scene::ProcessInput()
	{
		for (const auto& m : m_layers)
		{
			if (m->isActive() && m->isUpdating())
				m->ProcessInput();
		}
	}

	void Scene::AddLayer(std::unique_ptr<Layer> sceneLayer, const char* guid)
	{
		sceneLayer->setGuid(guid);
		m_layers.push_back(std::move(sceneLayer));
	}

	std::unique_ptr<Layer> Scene::RemoveLayer(const char* guid)
	{
		std::unique_ptr<Layer> result = nullptr;

		if(getLayerCount() > 0)
		{
			const auto found = std::find_if(m_layers.begin(), m_layers.end(),
				[&](const std::unique_ptr<Layer>& layer) -> bool {return layer->getGuid() == guid; });


			if (found != m_layers.end())
			{
				result = std::move(*found);

				m_layers.erase(found);
			}

		}

		return result;

	}

	Layer* Scene::getLayer(const char* guid)
	{
		if (getLayerCount() > 0)
		{
			const auto found = std::find_if(m_layers.begin(), m_layers.end(),
				[&](const std::unique_ptr<Layer>& layer) -> bool {return layer->getGuid() == guid; });

			if (*found)
				return found->get();
		}

		return nullptr;
	}

};