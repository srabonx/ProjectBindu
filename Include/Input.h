#include "Bindu.h"
#pragma once

#define BINDU_MAX_KEYS 26
#define BINDU_MAX_MOUSE_BUTTONS 5

#define BND_A 0
#define BND_B 1
#define BND_C 2
#define BND_D 3
#define BND_E 4
#define BND_F 5
#define BND_G 6
#define BND_H 7
#define BND_I 8
#define BND_J 9
#define BND_K 10
#define BND_L 11
#define BND_M 12
#define BND_N 13
#define BND_O 14
#define BND_P 15
#define BND_Q 16
#define BND_R 17
#define BND_S 18
#define BND_T 19
#define BND_U 20
#define BND_V 21
#define BND_W 22
#define BND_X 23
#define BND_Y 24
#define BND_Z 25

#define BND_BTN_LEFT 0
#define BND_BTN_MIDDLE 1
#define BND_BTN_RIGHT 2
#define BND_BTN_X1 3
#define BND_BTN_X2 4

namespace BINDU {

	struct KeyState														// State of the keyboard keys
	{
		bool wasDown, isDown;
	};

	struct KeyboardInputMap												
	{
		KeyState keys[BINDU_MAX_KEYS];
	};

	struct ButtonState													// State of the mouse buttons
	{
		bool wasDown, isDown;
	};

	struct Position														// Mouse position
	{
		int x, y;
	};

	struct MouseInputMap
	{
		ButtonState buttons[BINDU_MAX_MOUSE_BUTTONS];
		Position position;
	};

	class Input
	{
	friend class Engine;
	private:
		static void processKeyboardInput(uint32_t VKcode, bool wasDown, bool isDown);		// Processes Keyboard input from window message
		static void processMouseInput(WPARAM wParam, LPARAM lParam);						// Processes Mouse input from window message
		static void updateMousePosition(LPARAM lParam);										// Updates mouse position

		static KeyboardInputMap keyboard;

		static MouseInputMap mouse;
	public:
		Input(){}
		~Input(){}

		static KeyState getKeyState(uint32_t keycode);

		static bool isKeyPressed(uint32_t keycode);
		static bool isKeyReleased(uint32_t keycode);
		static bool isKeyHold(uint32_t keycode);
		static Position getMousePosition();
		
		static bool isMouseButtonPressed(uint32_t buttonCode);
		static bool isMouseButtonReleased(uint32_t buttonCode);
		static bool isMouseButtonHold(uint32_t buttonCode);
		
	};// class

};// namespace
