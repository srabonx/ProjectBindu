#pragma once
#include <timeapi.h>

namespace BINDU {

	class Timer
	{
	private:

		DWORD m_time;
		DWORD m_stopwatch;
		int	  m_interval{ 0 };

	public:
		Timer(void) 
		{
			m_time = GetTickCount();
			resetStopwatch();
		}

		Timer(int interval) :m_interval{ interval }
		{
			m_time = GetTickCount();
			resetStopwatch();
		}
		
		inline bool pulse()
		{
			return stopwatch(m_interval);
		}

		inline DWORD getCurrentTime()								// Get time since system started
		{
			return GetTickCount();
		}

		inline void resetStopwatch()
		{
			m_stopwatch = GetTickCount();
		}

	    inline bool stopwatch(int ms)
		{
			if (GetTickCount() > m_stopwatch + ms)
			{
				resetStopwatch();
				return true;
			}

			return false;
		}
		

	};// class

};// namespace