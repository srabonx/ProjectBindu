#include "Include/Input.h"

#include <cstdint>
#include <windowsx.h>

namespace BINDU
{


 KeyboardInputMap Input::keyboard;
 MouseInputMap Input::mouse;

 KeyState Input::getKeyState(uint32_t keycode)
 {
	 return keyboard.keys[keycode];
 }

 bool Input::isKeyPressed(uint32_t keycode)
 {
	 bool res = keyboard.keys[keycode].isDown;
	 keyboard.keys[keycode].isDown = false;
	 return res && !keyboard.keys[keycode].wasDown;
 }

 bool Input::isKeyReleased(uint32_t keycode)
 {
	 bool res = keyboard.keys[keycode].wasDown;
	 keyboard.keys[keycode].wasDown = false;
	 return res && !keyboard.keys[keycode].isDown;
 }

 bool Input::isKeyHold(uint32_t keycode)
 {
	 return (keyboard.keys[keycode].isDown);
 }

 void Input::processKeyboardInput(uint32_t VKcode, bool wasDown, bool isDown)
 {
	 if (wasDown != isDown)
	 {
		 if (VKcode >= 'A' && VKcode <= 'Z')
		 {
			 uint32_t bnd_keyCode = VKcode - 'A';
			 keyboard.keys[bnd_keyCode].isDown = isDown;
			 keyboard.keys[bnd_keyCode].wasDown = wasDown;
		 }
	 }
 }

 Position Input::getMousePosition()
 {
	 return mouse.position;
 }

 bool Input::isMouseButtonPressed(uint32_t buttonCode)
 {
	 bool res = mouse.buttons[buttonCode].isDown;
	 mouse.buttons[buttonCode].isDown = false;
	 return res && !mouse.buttons[buttonCode].wasDown;
 }

 bool Input::isMouseButtonReleased(uint32_t buttonCode)
 {
	 bool res = mouse.buttons[buttonCode].wasDown;
	 mouse.buttons[buttonCode].wasDown = false;
	 return res && !mouse.buttons[buttonCode].isDown;
 }

 bool Input::isMouseButtonHold(uint32_t buttonCode)
 {
	 return mouse.buttons[buttonCode].isDown;
 }

 void Input::processMouseInput(WPARAM wParam, LPARAM lParam)
 {
	 mouse.buttons[BND_BTN_LEFT].wasDown = mouse.buttons[BND_BTN_LEFT].isDown;
	 mouse.buttons[BND_BTN_MIDDLE].wasDown = mouse.buttons[BND_BTN_MIDDLE].isDown;
	 mouse.buttons[BND_BTN_RIGHT].wasDown = mouse.buttons[BND_BTN_RIGHT].isDown;
	 mouse.buttons[BND_BTN_X1].wasDown = mouse.buttons[BND_BTN_X1].isDown;
	 mouse.buttons[BND_BTN_X2].wasDown = mouse.buttons[BND_BTN_X2].isDown;

	 mouse.buttons[BND_BTN_LEFT].isDown = wParam & MK_LBUTTON;
	 mouse.buttons[BND_BTN_MIDDLE].isDown = wParam & MK_MBUTTON;
	 mouse.buttons[BND_BTN_RIGHT].isDown = wParam & MK_RBUTTON;
	 mouse.buttons[BND_BTN_X1].isDown = wParam & MK_XBUTTON1;
	 mouse.buttons[BND_BTN_X2].isDown = wParam & MK_XBUTTON2;
	 
	 updateMousePosition(lParam);

 }

 void Input::updateMousePosition(LPARAM lParam)
 {
	 mouse.position.x = GET_X_LPARAM(lParam);
	 mouse.position.y = GET_Y_LPARAM(lParam);

 }

};