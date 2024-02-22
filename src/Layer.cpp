
#include "Include/Layer.h"

#include "Include/CollisionHandler.h"


namespace BINDU
{


	void Layer::onLoadResource()
	{
		for (const auto& object : m_objects)
			object->onLoadResource();
	}

	void Layer::onReleaseResource()
	{
		for (const auto& object : m_objects)
			object->onReleaseResource();

		m_objects.clear();
	}

	void Layer::Clear()
	{
		m_objects.clear();
	}


	void Layer::Update(float dt)
	{
		if (m_isUpdating)
		{
			for (const auto& m : m_objects)
			{
				if (m)
				{
					if (m->isQueuedForDestroy())
					{
						RemoveObject(m->getGuid().c_str());
					}
					else if (m->isActive())
					{
						m->setTranslation(m_offsetX, m_offsetY);
						m->UpdateWithChild(dt);
					}
				}
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
				

					if(m->getColliderType() == ColliderType::RECT_COLLIDER)
					{
						graphics->getRenderTarget()->SetTransform(m->getTransform() * cameraMatrixWithParallax);
						graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
						graphics->getRenderTarget()->DrawRectangle({ m->getRectCollider().x,m->getRectCollider().y,m->getRectCollider().x + m->getRectCollider().w,m->getRectCollider().y + m->getRectCollider().h }, graphics->getSolidColorBrush());
						graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
						graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
					}
					else if(m->getColliderType() == ColliderType::CIRCLE_COLLIDER)
					{
						graphics->getRenderTarget()->SetTransform(m->getTransform() * cameraMatrixWithParallax);
						graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
						graphics->getRenderTarget()->DrawEllipse(D2D1::Ellipse({ m->getCircleCollider().position.x,m->getCircleCollider().position.y }, m->getCircleCollider().radius, m->getCircleCollider().radius), graphics->getSolidColorBrush());
						graphics->getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
						graphics->getSolidColorBrush()->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
					}


				}

				m->DrawWithChild(graphics, cameraMatrixWithParallax);

			}
		}
	}

	void Layer::ProcessInput() const
	{
		if (m_isUpdating)
		{
			for (const auto& m : m_objects)
			{
				if (m->isActive())
					m->ProcessAllInput();
			}
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

	void Layer::RemoveExcept(const char* guid)
	{
		for(auto itr = m_objects.begin(); itr != m_objects.end(); )
		{
			if ((*itr)->getGuid() != std::string(guid))
				itr = m_objects.erase(itr);
			else
				++itr;
		}
	}

	SceneObject* Layer::getObject(const char* guid)
	{
		if (getObjectCount() > 0)
		{
			const auto found = std::find_if(m_objects.begin(), m_objects.end(),
				[&](const std::unique_ptr<SceneObject>& obj) -> bool {return obj->getGuid() == guid; });

			if (found != m_objects.end())
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

						if (m->getColliderType() == ColliderType::RECT_COLLIDER && m2->getColliderType() == ColliderType::RECT_COLLIDER)
						{
							if (CollisionHandler::RectVsRect(m->getRectCollider(), m2->getRectCollider()))
							{
								m->CheckCollision(*m2);
								m2->CheckCollision(*m);
							}
						}
					}
				}

			}
		}

	}
};
