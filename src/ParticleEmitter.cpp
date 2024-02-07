#include "Include/ParticleEmitter.h"

#include "Include/Random.h"

namespace BINDU
{

	ParticleEmitter::ParticleEmitter()
	{
		
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}

	void ParticleEmitter::Init(const ParticleProps& particleProps)
	{
		m_spriteBatch.Init();
		m_particleProps = particleProps;
	}

	void ParticleEmitter::Update(float dt)
	{

		if (m_onLoop)
		{
			if (m_currentAlive < m_particlePool.size())
			{
				if (m_timer.stopwatch(m_emissionInterval))
				{
					Generate();
				}
			}
		}
			

		int currentAlive{ 0 };

		if (m_currentAlive > 0) {

			for (int i = 0; i < m_particlePool.size(); i++)
			{


				if (!m_particlePool[i].isAlive)
				{
					continue;
				}
				currentAlive++;

				m_particlePool[i].Update(dt);

				m_spriteBatch.UpdateSprite(i,1,&m_particlePool[i].dstRect,nullptr, m_particlePool[i].startColor, m_particlePool[i].startScale, m_particlePool[i].rotation);
				
			}
		}
		m_currentAlive = currentAlive;
	}

	void ParticleEmitter::Draw(Graphics* graphics, const D2D1_MATRIX_3X2_F& cameraMatrix)
	{
		if (m_currentAlive > 0)
		{
			m_spriteBatch.Draw(graphics,cameraMatrix);
		}
	}

	void ParticleEmitter::Add()
	{
		const float PI_OV_180 = (3.14159265f / 180.f);

		Particle& particle = m_particlePool[m_index];

		BND_COLOR startColor{};
		BND_COLOR endColor{};


		if (m_particleProps.colorRandomnessRange.min > 0 || m_particleProps.colorRandomnessRange.max > 0)
		{
			startColor.r = static_cast<float>(RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max));
			startColor.g = static_cast<float>(RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max));
			startColor.b = static_cast<float>(RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max));
			startColor.a = static_cast<float>(RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max));

			endColor.r = static_cast<float>(RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max));
			endColor.g = static_cast<float>(RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max));
			endColor.b = static_cast<float>(RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max));
			endColor.a = static_cast<float>(RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max));
		}
		else
		{
			startColor.r = m_particleProps.startColor.r;
			startColor.g = m_particleProps.startColor.g;
			startColor.b = m_particleProps.startColor.b;
			startColor.a = m_particleProps.startColor.a;
			
			endColor.r = m_particleProps.endColor.r;
			endColor.g = m_particleProps.endColor.g;
			endColor.b = m_particleProps.endColor.b;
			endColor.a = m_particleProps.endColor.a;

			if (m_particleProps.colorOpacityRange.min > 0 || m_particleProps.colorOpacityRange.max > 0)
			{
				startColor.a = static_cast<float>(RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max));
				endColor.a = static_cast<float>(RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max));

			}
		}


		particle.dstRect.x = m_position.x;
		particle.dstRect.y = m_position.y;
		particle.dstRect.w = m_particleProps.size.x;
		particle.dstRect.h = m_particleProps.size.y;

		particle.velocity = m_particleProps.velocity;
		particle.lifeTime = (m_particleProps.lifeTime+RandomNumber::Get(0,3)) ;
		particle.lifeRemaining = particle.lifeTime;
		particle.minTimetoChangeColor = m_particleProps.minTimetoChangeColor;
		particle.startColor = startColor;
		particle.endColor = endColor;
		particle.startScale = m_particleProps.startScale;
		particle.endScale = m_particleProps.endScale;
		particle.rotation = m_particleProps.rotation;
		particle.rotationRate = m_particleProps.rotationRate * (RandomNumber::Get(0,2));
		particle.isAlive = true;
		particle.fadeOut = m_particleProps.fadeOut;

		// Set linear velocity
		float vx = static_cast<float> (cos(RandomNumber::Get(m_sprayDirection, m_sprayDirection + m_spread) * PI_OV_180));
		float vy = static_cast<float> (sin(RandomNumber::Get(m_sprayDirection, m_sprayDirection + m_spread) * PI_OV_180));

		particle.velocity = { vx * particle.velocity.x + RandomNumber::Get(0,5),vy * particle.velocity.y + RandomNumber::Get(0,5) };

		m_spriteBatch.UpdateSprite(m_index,1,&particle.dstRect , nullptr,particle.startColor, particle.startScale, particle.rotation);

		
		// Book-keeping

		m_index = --m_index % m_particlePool.size();

		++m_currentAlive;

		if (m_currentAlive > m_maxParticles)
			m_currentAlive = m_maxParticles;


		if (m_index == 0)
			m_index = m_maxParticles - 1;

	}

	void ParticleEmitter::SetTexture(const Texture& texture)
	{
		m_spriteBatch.SetTexture(texture);
	}

	void ParticleEmitter::Generate()
	{
		for (int i = 0; i < m_emissionRate; i++)
			Add();
	}

	void ParticleEmitter::setMax(int value)
	{
		m_maxParticles = value;
		m_particlePool.resize(value);
		m_index = value - 1;


		for (int i = 0; i < value; i++)
		{
			BND_COLOR color{};

			m_spriteBatch.AddSprite({}, nullptr, color, 1, 0);
		}
	}

	void ParticleEmitter::Particle::Update(float dt)
	{

			if (lifeRemaining < 1) 
			{
				isAlive = false;
				startColor.a = 0.f;
			}

			dstRect.x += velocity.x * dt;
			dstRect.y += velocity.y * dt;
			
			rotation += rotationRate * dt;

			if (rotation > 360.f)
				rotation = 0.f;

			lifeRemaining -= dt;


			if (lifeRemaining < minTimetoChangeColor)
			{
				startColor.r = endColor.r + (startColor.r - endColor.r) * 0.99f;
				startColor.g = endColor.g + (startColor.g - endColor.g) * 0.99f;
				startColor.b = endColor.b + (startColor.b - endColor.b) * 0.99f;

				if(!fadeOut)
				startColor.a = endColor.a + (startColor.a - endColor.a) * 0.99f;
			}

			if (fadeOut)
			{
				if (lifeRemaining < (lifeTime * 0.40f))
					startColor.a *= 0.99f;

				if (startColor.a <= 0.f)
					lifeRemaining = 0.f;
			}

			// TODO: add scale changing;
			if (lifeRemaining < (lifeTime * 0.40f))
				startScale = endScale + (startScale - endScale) * 0.99f;

	}



};