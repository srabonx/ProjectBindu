#include "Include/Transformable.h"

namespace BINDU
{
	void Transformable::UpdateTransform()
	{

		const D2D1_POINT_2F     origin = { m_position.x + m_origin.x , m_position.y + m_origin.y };

		const D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(m_translation.x, m_translation.y);
		const D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(m_rotation, origin);
		const D2D1_MATRIX_3X2_F scaling = D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y, origin);

		m_transform = scaling * rotation * translation;

	}

	Vec2f Transformable::getCenter() const
	{
		Vec2f tmpCenter{};
		tmpCenter.x = m_position.x + m_size.width / 2;
		tmpCenter.y = m_position.y + m_size.height / 2;

		return tmpCenter;
	}
};