#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <memory>
#include <string>
#include <vector>

#include "fmod.hpp"

namespace BINDU
{

	class Sound
	{
	private:
		std::string m_name{};

		FMOD::Sound*	m_sound = nullptr;
		FMOD::Channel*	m_channel = nullptr;

		friend class SoundSystem;
	public:
		Sound() = default;
		~Sound();

		inline std::string getName() const { return m_name; }
		inline void		   setName(const char* name) { m_name = name; }

		inline void			setVolume(const float volume) const { m_channel->setVolume(volume); }
		float				getVolume() const;
		
	};

	class SoundSystem
	{
	private:
		FMOD::System*		m_system = nullptr;

		std::vector<std::unique_ptr<Sound>> m_sounds;

	public:
		SoundSystem();
		~SoundSystem();

		void	Init() const;

		void	Update() const;

		bool	Load(const char* filename, const char* filepath);

		bool	LoadSound(Sound& sound,const char* filepath) const;

		bool	Play(const char* name, bool loop = false, float volume = 0.5f);

		bool	PlaySound(Sound& sound,bool loop) const;

		void	Stop(const char* name);

		void    StopAll() const;

		void	StopAllExcept(const char* name) const;

		bool	isPlaying(const char* name);

		bool	SoundExists(const char* name) const;

		Sound*  FindSound(const char* name);

	};
};

#endif // SOUNDSYSTEM_H
