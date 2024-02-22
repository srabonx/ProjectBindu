#pragma once

#define BINDU_MAX_KEYS 52
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

#define BND_UP 26
#define BND_DOWN 27
#define BND_LEFT 28
#define BND_RIGHT 29

#define BND_0	30
#define BND_1	31
#define BND_2	32
#define BND_3	33
#define BND_4	34
#define BND_5	35
#define BND_6	36
#define BND_7	37
#define BND_8	38
#define BND_9	39
#define BND_MINUS	40
#define BND_PLUS	41

#define BND_SHIFT	42
#define BND_CONTROL	43
#define BND_ALT		44
#define BND_SPACE	45
#define BND_ESCAPE	46
#define BND_CAPSLOCK 47
#define BND_TAB		48
#define BND_ENTER	49
#define BND_BACKSPACE 50
#define BND_TILDE	51

#define BND_BTN_LEFT 0
#define BND_BTN_MIDDLE 1
#define BND_BTN_RIGHT 2
#define BND_BTN_X1 3
#define BND_BTN_X2 4

#include <cstdint>
#include <Windows.h>

#include "Vector.h"

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

	struct MouseInputMap
	{
		ButtonState buttons[BINDU_MAX_MOUSE_BUTTONS];
		Vec2f position;
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
		static Vec2f getMousePosition();
		
		static bool isMouseButtonPressed(uint32_t buttonCode);
		static bool isMouseButtonReleased(uint32_t buttonCode);
		static bool isMouseButtonHold(uint32_t buttonCode);
		
	};// class

};// namespace
