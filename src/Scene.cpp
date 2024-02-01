
#include "Include/Scene.h"

#include <locale>
#include <sstream>

#include "Include/BitmapLoader.h"
#include "Include/TileLayer.h"

namespace BINDU
{
	void Scene::onLoadResource()
	{
		for(auto& m:m_tileSets)
		{

			std::wostringstream wstm;

			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());

			for (const char i : m->source)
				wstm << ctfacet.widen(i);

			m->tileTexture.LoadFromFile(m->source.c_str());
		}

		for(auto& m:m_layers)
		{
			if (TileLayer* layer = dynamic_cast<TileLayer*>(m.get()))
			{
				layer->setWorldTileWidth(m_wTileWidth);
				layer->setWorldTileHeight(m_wTileHeight);
				layer->onLoadResource();
			}
		}


	}

	void Scene::Update(float dt) const
	{
		for(const auto& m: m_layers)
		{
			if (m->isActive())
			{
				m->Update(dt);
				m->CheckCollision();
			}
		}
	}

	void Scene::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const
	{
		for (const auto& m : m_layers)
		{
			if (m->isActive())
			{
				m->Draw(graphics, cameraMatrix);
			}
		}
	}

	void Scene::ProcessInput() const
	{
		for (const auto& m : m_layers)
		{
			if (m->isActive())
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