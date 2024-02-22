#pragma once
#include "Graphics.h"
#include "Range.h"
#include "SpriteBatch.h"
#include "TImer.h"
#include "Vector.h"

namespace BINDU
{


	struct ParticleProps
	{

		Vec2f		size{ 1.f,1.f };
		Vec2f		velocity{};

		float		startScale{ 1 };
		float		endScale{ 1 };

		float		rotation{};
		float		rotationRate{};

		BND_COLOR	startColor{};
		BND_COLOR	endColor{};

		iRange		colorRandomnessRange{0,0};

		iRange		colorOpacityRange{ 0,0 };

		float		minTimetoChangeColor{ 0 };								// in seconds

		bool		fadeOut{ false };

		float		lifeTime{};												// in seconds
		
	};

	class ParticleEmitter : public SceneObject
	{

	private:

		struct Particle
		{
		public:
			Bnd_Rect_F	dstRect{};
			Vec2f		velocity{};

			float		startScale{};
			float		endScale{};

			float		rotation{};
			float		rotationRate{};

			BND_COLOR	startColor{};
			BND_COLOR	endColor{};

			float		lifeTime{};												// in seconds
			float		lifeRemaining{};
			bool		fadeOut{ false };

			float		minTimetoChangeColor{};									// in seconds

			bool		isAlive{ false };

			void		Update(float dt);

		};

		
		
	private:
		Timer						m_timer{};
		std::vector<Particle>		m_particlePool{};

		int							m_maxParticles{ 1 };
		int							m_currentParticles{};
		int							m_sprayDirection{ 0 };
		int							m_spread{ 1 };

		int							m_currentAlive{};

		int							m_emissionInterval{ 10 };
		int							m_emissionRate{ 1 };

		unsigned int				m_index{};

		bool						m_onLoop{ true };

		ParticleProps				m_particleProps{};

		SpriteBatch					m_spriteBatch;

	private:

		void			Add();

	public:
		ParticleEmitter();
		~ParticleEmitter();

		void			Init(const ParticleProps& particleProps);

		void			Update(float dt) override;
		virtual void	Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix) override;
		virtual void	ProcessInput() override
		{
			
		}

		void			SetTexture(const Texture& texture);

		void			Generate();

		inline void		setEmissionInterval(int ms) { m_emissionInterval = ms; }
		inline void		setEmissionRate(int count) { m_emissionRate = count; }

		void			setMax(int value);
		inline void		setDirection(int degree) { m_sprayDirection = degree; }
		inline void		setSpread(int value) { m_spread = value; }
		inline void		onLoop(bool value) { m_onLoop = value; }

		inline void		ChangeParticleColor(const BND_COLOR& color) { m_particleProps.startColor = color; }
		inline void		ChangeParticleVelocity(const Vec2f& velocity) { m_particleProps.velocity = velocity; }

	};// Class

};// Namespace
