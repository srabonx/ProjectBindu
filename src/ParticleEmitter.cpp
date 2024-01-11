#include "Include/Bindu.h"

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
		SpriteBatch::Init();
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
//					SpriteBatch::UpdateSprite(i, NULL, m_particlePool[i].startColor, m_particlePool[i].startScale, m_particlePool[i].rotation, m_particlePool[i].velocity);
					continue;
				}
				currentAlive++;

				m_particlePool[i].Update(dt);

				D2D1_RECT_F dstRect{};
				dstRect.left = m_particlePool[i].position.x;
				dstRect.top = m_particlePool[i].position.y;
				dstRect.right = m_particlePool[i].position.x + m_particlePool[i].size.x;
				dstRect.bottom = m_particlePool[i].position.y + m_particlePool[i].size.y;

				SpriteBatch::UpdateSprite(i, &dstRect, m_particlePool[i].startColor, m_particlePool[i].startScale, m_particlePool[i].rotation, m_particlePool[i].velocity);
				
			}
		}
		m_currentAlive = currentAlive;
	}

	void ParticleEmitter::Draw(Graphics* graphics)
	{
		if (m_currentAlive > 0)
		{
/*			for (int i = 0; i < m_particlePool.size(); i++)
			{
				if (m_particlePool[i].isAlive)
					SpriteBatch::DrawSelected(i);
				else
					continue;
			} */
			SpriteBatch::Draw(graphics);
		}
	}

	void ParticleEmitter::Add()
	{
		float PI_OV_180 = static_cast<float> (3.14159265f / 180.f);

		Particle& particle = m_particlePool[m_index];

		BND_COLOR startColor{};
		BND_COLOR endColor{};


		if (m_particleProps.colorRandomnessRange.min > 0 || m_particleProps.colorRandomnessRange.max > 0)
		{
			startColor.r = RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max);
			startColor.g = RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max);
			startColor.b = RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max);
			startColor.a = RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max);

			endColor.r = RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max);
			endColor.g = RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max);
			endColor.b = RandomNumber::Get(m_particleProps.colorRandomnessRange.min, m_particleProps.colorRandomnessRange.max);
			endColor.a = RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max);
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
				startColor.a = RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max);
				endColor.a = RandomNumber::Get(m_particleProps.colorOpacityRange.min, m_particleProps.colorOpacityRange.max);

			}
		}


		particle.position = m_position;
		particle.size = m_particleProps.size;
		particle.velocity = m_particleProps.velocity;
		particle.lifeTime = (m_particleProps.lifeTime+RandomNumber::Get(0,3)) ;
		particle.lifeRemaining = particle.lifeTime;
		particle.minTimetoChangeColor = m_particleProps.minTimetoChangeColor;
		particle.startColor = startColor;
		particle.endColor = endColor;
		particle.startScale = m_particleProps.startScale;
		particle.endScale = m_particleProps.endScale;
		particle.rotation = m_particleProps.rotation;
		particle.rotationRate = m_particleProps.rotationRate + (RandomNumber::Get(0,20));
		particle.isAlive = true;
		particle.fadeOut = m_particleProps.fadeOut;


		D2D1_RECT_F dstRect{};
		dstRect.left = m_position.x;
		dstRect.top = m_position.y;
		dstRect.right = m_position.x + particle.size.x;
		dstRect.bottom = m_position.y + particle.size.y;

		// Set linear velocity
		float vx = static_cast<float> (cos(RandomNumber::Get(m_sprayDirection, m_sprayDirection + m_spread) * PI_OV_180));
		float vy = static_cast<float> (sin(RandomNumber::Get(m_sprayDirection, m_sprayDirection + m_spread) * PI_OV_180));

		particle.velocity = { vx * particle.velocity.x + RandomNumber::Get(0,5),vy * particle.velocity.y + RandomNumber::Get(0,5) };

		UpdateSprite(m_index, &dstRect , particle.startColor, particle.startScale, particle.rotation, particle.velocity);

		
		// Book keeping

		m_index = --m_index % m_particlePool.size();

		++m_currentAlive;

		if (m_currentAlive > m_maxParticles)
			m_currentAlive = m_maxParticles;

//		++m_currentParticles;

//		if (m_currentParticles > m_maxParticles)
//			m_currentParticles = m_maxParticles;

		if (m_index == 0)
			m_index = m_maxParticles - 1;

	}

	void ParticleEmitter::LoadParticleSprite(const wchar_t* filename)
	{
		LoadFromFile(filename);
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

		D2D1_RECT_F dstRect{};

		dstRect.left = m_position.x;
		dstRect.top = m_position.y;
		dstRect.right = m_position.x + m_particleProps.size.x;
		dstRect.bottom = m_position.y + m_particleProps.size.y;

		for (int i = 0; i < value; i++)
		{
			BND_COLOR color{};

			SetSprite(dstRect, NULL, color, 1, 0, { 0,0 });
		}
	}

	void ParticleEmitter::Particle::Update(float dt)
	{

			if (lifeRemaining < 1) 
			{
				isAlive = false;
				startColor.a = 0.f;
			}

			position.x += velocity.x * dt;
			position.y += velocity.y * dt;

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

	void ParticleEmitter::Particle::Reset()
	{

	}


};