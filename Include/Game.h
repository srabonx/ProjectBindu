#pragma once

namespace BINDU {

	class Game
	{
	public:
		virtual bool Preload() = 0;
		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void ProcessInput() = 0;
	};

};
