#include "Include/SoundSystem.h"

#include <iostream>

namespace BINDU
{

	Sound::~Sound()
	{
		if(m_sound != nullptr)
		{
			m_channel->stop();
			m_sound->release();
		}
	}


	float Sound::getVolume() const
	{
		float vol{ 0.0f };
		m_channel->getVolume(&vol);

		return vol;
	}

	SoundSystem::SoundSystem()
	{
		if(FMOD::System_Create(&m_system) != FMOD_OK)
		{
			std::cerr << "Error creating FMOD system!" << std::endl;
			return;
		}

		int driverCount = 0;
		m_system->getNumDrivers(&driverCount);

		if(driverCount == 0)
		{
			std::cerr << "Error no available driver!" << std::endl;
			return;
		}

	}

	SoundSystem::~SoundSystem()
	{
		m_sounds.clear();

		if (m_system->release() != FMOD_OK)
			std::cerr << "Error releasing FMOD system" << std::endl;
	}

	void SoundSystem::Init() const
	{
		if(m_system->init(36,FMOD_INIT_NORMAL,nullptr) != FMOD_OK)
		{
			std::cerr << "Error initializing FMOD system!" << std::endl;
		}
	}

	void SoundSystem::Update() const
	{
		if(m_system->update() != FMOD_OK)
		{
			std::cerr << "Error updating FMOD system!" << std::endl;
		}
	}

	bool SoundSystem::Load(const char* filename, const char* filepath)
	{
		if (filename == nullptr || filepath == nullptr) return false;

		std::unique_ptr<Sound> sound = std::make_unique<Sound>();
		sound->setName(filename);


		if (m_system->createSound(filepath, FMOD_DEFAULT, nullptr, &sound->m_sound) != FMOD_OK)
		{
			std::cerr << "Error creating sound!" << std::endl;
			return false;
		}

		m_sounds.push_back(std::move(sound));

		return true;
	}

	bool SoundSystem::LoadSound(Sound& sound,const char* filepath) const
	{
		if (filepath == nullptr) return false;

		if(m_system->createSound(filepath,FMOD_DEFAULT,nullptr, &sound.m_sound) != FMOD_OK)
		{
			std::cerr << "Error creating sound: " + std::string(filepath) << std::endl;
			return false;
		}

		return true;
	}

	bool SoundSystem::Play(const char* name, bool loop,float volume)
	{
		Sound* sound = FindSound(name);

		if (!sound) return false;


		if (!loop)
			sound->m_sound->setMode(FMOD_LOOP_OFF);
		else
		{
			sound->m_sound->setMode(FMOD_LOOP_NORMAL);
			sound->m_sound->setLoopCount(-1);
		}

		if (m_system->playSound(sound->m_sound, nullptr, false, &sound->m_channel) != FMOD_OK)
		{
			std::cerr << "Error playing sound!" << std::endl;
			return false;
		}

		if(sound->m_channel->setVolume(volume) != FMOD_OK)
		{
			std::cerr << "Error setting volume!" << std::endl;
			return false;
		}

		return true;
	}

	bool SoundSystem::PlaySound(Sound& sound, bool loop) const
	{
		if (sound.m_sound == nullptr) return false;

		if (!loop)
			sound.m_sound->setMode(FMOD_LOOP_OFF);
		else
		{
			sound.m_sound->setMode(FMOD_LOOP_NORMAL);
			sound.m_sound->setLoopCount(-1);
		}

		if (m_system->playSound(sound.m_sound, nullptr, false, &sound.m_channel) != FMOD_OK)
		{
			std::cerr << "Error playing sound!" << std::endl;
			return false;
		}

		return true;
	}

	void SoundSystem::Stop(const char* name)
	{
		if (!isPlaying(name)) return;

		Sound* sound = FindSound(name);

		if (!sound) return;

		sound->m_channel->stop();
	}

	void SoundSystem::StopAll() const
	{
		for(const auto& sound:m_sounds)
		{
			sound->m_channel->stop();
		}
	}

	void SoundSystem::StopAllExcept(const char* name) const
	{
		for(const auto& sound:m_sounds)
		{
			if (sound->m_name != std::string(name))
				sound->m_channel->stop();
		}
	}

	bool SoundSystem::isPlaying(const char* name)
	{
		Sound* sound = FindSound(name);

		if (!sound) return false;

		bool isPlaying{};

		sound->m_channel->isPlaying(&isPlaying);

		return isPlaying;
	}

	bool SoundSystem::SoundExists(const char* name) const
	{
		for (const auto& sound : m_sounds)
			if (sound->m_name == std::string(name))
				return true;

		return false;
	}

	Sound* SoundSystem::FindSound(const char* name)
	{
		const auto found = std::find_if(m_sounds.begin(), m_sounds.end(),
			[&](const std::unique_ptr<Sound>& sound) ->bool {return sound->m_name == std::string(name); });

		if (found != m_sounds.end())
			return found->get();
		
		return nullptr;
	}
};
