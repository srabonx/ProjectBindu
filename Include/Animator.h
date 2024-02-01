#ifndef ANIMATOR_H
#define ANIMATOR_H
#include <map>
#include <string>

#include "BND_Helper.h"
#include "TImer.h"
#include "Vector.h"

namespace BINDU
{

	class Animator
	{
	private:
		struct AnimDetails
		{
			int m_index{};
			int m_frameCount{};
		};


		std::map<std::string, AnimDetails> m_animationMap;

		int		m_totalFrame{};
		int		m_totalColumn{};
		int		m_currentFrame{};
		int		m_currentRow{};

		Vec2f	m_framePosition{};
		Bnd_Size_F m_frameSize{};

		int		m_frameTimeMs{};
		Timer	m_frameTimer;

	public:

		Animator() = default;
		~Animator() = default;

		bool				LoadAnimationFile(const char* filename);
		void				Animate(Bnd_Rect_F& srcRect);

		// Accessor / Mutator functions

		inline void			setFramePosition(const Vec2f& position) { m_framePosition = position; }
		inline void			setFramePosition(const float x, const float y) { m_framePosition = { x,y }; }
		inline Vec2f		getFramePosition() const { return m_framePosition; }

		inline void			setFrameSize(const Bnd_Size_F& size) { m_frameSize = size; }
		inline void			setFrameSize(const float width, const float height) { m_frameSize = { width,height }; }
		inline Bnd_Size_F	getFrameSize() const { return m_frameSize; }

		inline void			setTotalFrame(int count) { m_totalFrame = count; }
		inline int			getTotalFrame() const { return m_totalFrame; }

		inline void			setTotalColumn(int count) { m_totalColumn = count; }
		inline int			getTotalColumn() const { return m_totalColumn; }

		inline void			setCurrentFrame(int frame) { m_currentFrame = frame; }
		Bnd_Rect_F			getCurrentFrame() const;

		inline void			setCurrentRow(int row) { m_currentRow = row; }
		inline int			getCurrentRow() const { return m_currentRow; }

		inline void			setFrameTime(int ms) { m_frameTimeMs = ms; }

		void				setAnimation(const char* animation);

		Bnd_Rect_F			getFrame(const int row, const int column) const;
		

	};

	
};
#endif // ANIMATOR_H
