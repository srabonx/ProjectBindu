
#include "Include/Layer.h"

#include "Include/CollisionHandler.h"


namespace BINDU
{
	void Layer::Update(float dt)
	{
		for(const auto& m:m_objects)
		{
			if (m->isActive())
			{
				m->setTranslation(m_offsetX, m_offsetY);
				m->UpdateWithChild(dt);
			}
				
		}

	}

	void Layer::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) const
	{
		for (const auto& m : m_objects)
		{

			if (m->isActive())
			{
				D2D1_MATRIX_3X2_F cameraMatrixWithParallax = cameraMatrix;
				cameraMatrixWithParallax.dx = cameraMatrix.dx * m_parallaxFactor.x;
				cameraMatrixWithParallax.dy = cameraMatrix.dy * m_parallaxFactor.y;

				if(m_showDebug)
				{
					m->UpdateTransform();
					graphics->getRenderTarget()->SetTransform(m->getTransform() * cameraMatrixWithParallax);
					graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
					graphics->getRenderTarget()->DrawRectangle({ m->getCollider().x,m->getCollider().y,m->getCollider().x + m->getCollider().w,m->getCollider().y+m->getCollider().h }, graphics->getSolidColorBrush());
					graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
					graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));

				}

				m->DrawWithChild(graphics, cameraMatrixWithParallax);

			}
		}
	}

	void Layer::ProcessInput() const
	{
		for (const auto& m : m_objects)
		{
			if (m->isActive())
				m->ProcessAllInput();
		}
	}

	void Layer::AddObject(std::unique_ptr<SceneObject> sceneObject, const char* guid)
	{
		sceneObject->setGuid(guid);
		m_objects.push_back(std::move(sceneObject));
	}

	std::unique_ptr<SceneObject> Layer::RemoveObject(const char* guid)
	{
		std::unique_ptr<SceneObject> result = nullptr;

		if (getObjectCount() > 0)
		{
			const auto found = std::find_if(m_objects.begin(), m_objects.end(),
				[&](const std::unique_ptr<SceneObject>& obj) -> bool {return obj->getGuid() == guid; });

			if (found != m_objects.end())
			{
				result = std::move(*found);

				m_objects.erase(found);
			}
		}

		return result;
	}

	SceneObject* Layer::getObject(const char* guid)
	{
		if (getObjectCount() > 0)
		{
			const auto found = std::find_if(m_objects.begin(), m_objects.end(),
				[&](const std::unique_ptr<SceneObject>& obj) -> bool {return obj->getGuid() == guid; });

			if (*found)
				return found->get();
		}

		return nullptr;
	}

	void Layer::CheckCollision()
	{

		for(const auto& m: m_objects)
		{
			for(const auto& m2: m_objects)
			{
				if (m != m2 )
				{
					if (m->isCollideAble() && m2->isCollideAble())
					{
						if(CollisionHandler::RectVsRect(m->getCollider(), m2->getCollider()))
						{
							m->CheckCollision(*m2);
							m2->CheckCollision(*m);
						}

					}
				}

			}
		}

	}
};
