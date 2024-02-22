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
			 const uint32_t bnd_keyCode = VKcode - 'A';
			 keyboard.keys[bnd_keyCode].isDown = isDown;
			 keyboard.keys[bnd_keyCode].wasDown = wasDown;
		 }
		 else if(VKcode == VK_UP)
		 {
			 keyboard.keys[BND_UP].wasDown = wasDown;
			 keyboard.keys[BND_UP].isDown = isDown;
		 }
		 else if (VKcode == VK_DOWN)
		 {
			 keyboard.keys[BND_DOWN].wasDown = wasDown;
			 keyboard.keys[BND_DOWN].isDown = isDown;
		 }
		 else if (VKcode == VK_LEFT)
		 {
			 keyboard.keys[BND_LEFT].wasDown = wasDown;
			 keyboard.keys[BND_LEFT].isDown = isDown;
		 }
		 else if (VKcode == VK_RIGHT)
		 {
			 keyboard.keys[BND_RIGHT].wasDown = wasDown;
			 keyboard.keys[BND_RIGHT].isDown = isDown;
		 }
		 else if(VKcode >= '0' && VKcode <='9')
		 {
			 const uint32_t bnd_keyCode = VKcode - '0' + BND_0;
			 keyboard.keys[bnd_keyCode].isDown = isDown;
			 keyboard.keys[bnd_keyCode].wasDown = wasDown;
		 }
		 else if(VKcode == VK_OEM_MINUS)
		 {
			 keyboard.keys[BND_MINUS].wasDown = wasDown;
			 keyboard.keys[BND_MINUS].isDown = isDown;
		 }
		 else if (VKcode == VK_OEM_PLUS)
		 {
			 keyboard.keys[BND_PLUS].wasDown = wasDown;
			 keyboard.keys[BND_PLUS].isDown = isDown;
		 }
		 else if (VKcode == VK_SHIFT)
		 {
			 keyboard.keys[BND_SHIFT].wasDown = wasDown;
			 keyboard.keys[BND_SHIFT].isDown = isDown;
		 }
		 else if (VKcode == VK_ESCAPE)
		 {
			 keyboard.keys[BND_ESCAPE].wasDown = wasDown;
			 keyboard.keys[BND_ESCAPE].isDown = isDown;
		 }
		 else if (VKcode == VK_SPACE)
		 {
			 keyboard.keys[BND_SPACE].wasDown = wasDown;
			 keyboard.keys[BND_SPACE].isDown = isDown;
		 }
		 else if (VKcode == VK_RETURN)
		 {
			 keyboard.keys[BND_ENTER].wasDown = wasDown;
			 keyboard.keys[BND_ENTER].isDown = isDown;
		 }

	 }
 }

 Vec2f Input::getMousePosition()
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
	 mouse.position.x = static_cast<float>(GET_X_LPARAM(lParam));
	 mouse.position.y = static_cast<float>(GET_Y_LPARAM(lParam));

 }

};