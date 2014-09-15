// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#include "SDLKeyboardManager.hpp"
#include "ApplicationBase.hpp"
#include "SDL2/SDL_scancode.h"

SDLKeyboardManager::SDLKeyboardManager()
{
}

SDLKeyboardManager::~SDLKeyboardManager()
{
}

bool SDLKeyboardManager::keyPressed(Key key)
{
    return m_pressedKeys[key];
}

bool SDLKeyboardManager::keyReleased(Key key)
{
    return m_releasedKeys[key];
}

bool SDLKeyboardManager::controlPressed()
{
    return (m_pressedKeys[Key::LCONTROL] || m_pressedKeys[Key::RCONTROL]);
}

bool SDLKeyboardManager::altPressed()
{
    return (m_pressedKeys[Key::LALT] || m_pressedKeys[Key::RALT]);

}

bool SDLKeyboardManager::shiftPressed()
{
    return (m_pressedKeys[Key::LSHIFT] || m_pressedKeys[Key::RSHIFT]);

}

Key orFromScanCode(int scan)
{
    switch(scan)
    {
        case SDL_SCANCODE_A:           return Key::A;
        case SDL_SCANCODE_B:           return Key::B;
        case SDL_SCANCODE_C:           return Key::C;
        case SDL_SCANCODE_D:           return Key::D;
        case SDL_SCANCODE_E:           return Key::E;
        case SDL_SCANCODE_F:           return Key::F;
        case SDL_SCANCODE_G:           return Key::G;
        case SDL_SCANCODE_H:           return Key::H;
        case SDL_SCANCODE_I:           return Key::I;
        case SDL_SCANCODE_J:           return Key::J;
        case SDL_SCANCODE_K:           return Key::K;
        case SDL_SCANCODE_L:           return Key::L;
        case SDL_SCANCODE_M:           return Key::M;
        case SDL_SCANCODE_N:           return Key::N;
        case SDL_SCANCODE_O:           return Key::O;
        case SDL_SCANCODE_P:           return Key::P;
        case SDL_SCANCODE_Q:           return Key::Q;
        case SDL_SCANCODE_R:           return Key::R;
        case SDL_SCANCODE_S:           return Key::S;
        case SDL_SCANCODE_T:           return Key::T;
        case SDL_SCANCODE_U:           return Key::U;
        case SDL_SCANCODE_V:           return Key::V;
        case SDL_SCANCODE_W:           return Key::W;
        case SDL_SCANCODE_X:           return Key::X;
        case SDL_SCANCODE_Y:           return Key::Y;
        case SDL_SCANCODE_Z:           return Key::Z;
        case SDL_SCANCODE_1:           return Key::NUM1;
        case SDL_SCANCODE_2:           return Key::NUM2;
        case SDL_SCANCODE_3:           return Key::NUM3;
        case SDL_SCANCODE_4:           return Key::NUM4;
        case SDL_SCANCODE_5:           return Key::NUM5;
        case SDL_SCANCODE_6:           return Key::NUM6;
        case SDL_SCANCODE_7:           return Key::NUM7;
        case SDL_SCANCODE_8:           return Key::NUM8;
        case SDL_SCANCODE_9:           return Key::NUM9;
        case SDL_SCANCODE_0:           return Key::NUM0;
        case SDL_SCANCODE_ESCAPE:      return Key::ESCAPE;
        case SDL_SCANCODE_LCTRL:       return Key::LCONTROL;
        case SDL_SCANCODE_LSHIFT:      return Key::LSHIFT;
        case SDL_SCANCODE_LALT:        return Key::LALT;
        case SDL_SCANCODE_LGUI:        return Key::LSYSTEM;
        case SDL_SCANCODE_RCTRL:       return Key::RCONTROL;
        case SDL_SCANCODE_RSHIFT:      return Key::RSHIFT;
        case SDL_SCANCODE_RALT:        return Key::RALT;
        case SDL_SCANCODE_RGUI:        return Key::RSYSTEM;
        case SDL_SCANCODE_MENU:        return Key::MENU;
        case SDL_SCANCODE_LEFTBRACKET: return Key::LBRACKET;
        case SDL_SCANCODE_RIGHTBRACKET:return Key::RBRACKET;
        case SDL_SCANCODE_SEMICOLON:   return Key::SEMICOLON;
        case SDL_SCANCODE_COMMA:       return Key::COMMA;
        case SDL_SCANCODE_PERIOD:      return Key::PERIOD;
        case SDL_SCANCODE_APOSTROPHE:  return Key::APOSTROPHE;
        case SDL_SCANCODE_SLASH:       return Key::FORARDSLASH;
        case SDL_SCANCODE_BACKSLASH:   return Key::BACKSLASH;
        case SDL_SCANCODE_GRAVE:       return Key::TILDE;
        case SDL_SCANCODE_SPACE:       return Key::SPACE;
        case SDL_SCANCODE_RETURN:      return Key::RETURN;
        case SDL_SCANCODE_BACKSPACE:   return Key::BACKSPACE;
        case SDL_SCANCODE_TAB:         return Key::TAB;
        case SDL_SCANCODE_PAGEUP:      return Key::PAGEUP;
        case SDL_SCANCODE_PAGEDOWN:    return Key::PAGEDOWN;
        case SDL_SCANCODE_END:         return Key::END;
        case SDL_SCANCODE_HOME:        return Key::HOME;
        case SDL_SCANCODE_INSERT:      return Key::INSERT;
        case SDL_SCANCODE_DELETE:      return Key::DELETE;
        case SDL_SCANCODE_KP_PLUS:     return Key::PLUS;
        case SDL_SCANCODE_KP_MINUS:    return Key::MINUS;
        case SDL_SCANCODE_KP_MULTIPLY: return Key::MULTIPLY;
        case SDL_SCANCODE_KP_DIVIDE:   return Key::DIVIDE;
        case SDL_SCANCODE_LEFT:        return Key::LEFT;
        case SDL_SCANCODE_RIGHT:       return Key::RIGHT;
        case SDL_SCANCODE_UP:          return Key::UP;
        case SDL_SCANCODE_DOWN:        return Key::DOWN;
        case SDL_SCANCODE_KP_0:        return Key::NUMPAD0;
        case SDL_SCANCODE_KP_1:        return Key::NUMPAD1;
        case SDL_SCANCODE_KP_2:        return Key::NUMPAD2;
        case SDL_SCANCODE_KP_3:        return Key::NUMPAD3;
        case SDL_SCANCODE_KP_4:        return Key::NUMPAD4;
        case SDL_SCANCODE_KP_5:        return Key::NUMPAD5;
        case SDL_SCANCODE_KP_6:        return Key::NUMPAD6;
        case SDL_SCANCODE_KP_7:        return Key::NUMPAD7;
        case SDL_SCANCODE_KP_8:        return Key::NUMPAD8;
        case SDL_SCANCODE_KP_9:        return Key::NUMPAD9;
        case SDL_SCANCODE_CAPSLOCK:    return Key::CAPSLOCK;
        case SDL_SCANCODE_PRINTSCREEN: return Key::PRINTSCREEN;
        case SDL_SCANCODE_SCROLLLOCK:  return Key::SCROLLLOCK;
        case SDL_SCANCODE_F1:          return Key::F1;
        case SDL_SCANCODE_F2:          return Key::F2;
        case SDL_SCANCODE_F3:          return Key::F3;
        case SDL_SCANCODE_F4:          return Key::F4;
        case SDL_SCANCODE_F5:          return Key::F5;
        case SDL_SCANCODE_F6:          return Key::F6;
        case SDL_SCANCODE_F7:          return Key::F7;
        case SDL_SCANCODE_F8:          return Key::F8;
        case SDL_SCANCODE_F9:          return Key::F9;
        case SDL_SCANCODE_F10:         return Key::F10;
        case SDL_SCANCODE_F11:         return Key::F11;
        case SDL_SCANCODE_F12:         return Key::F12;
        case SDL_SCANCODE_F13:         return Key::F13;
        case SDL_SCANCODE_F14:         return Key::F14;
        case SDL_SCANCODE_F15:         return Key::F15;
        case SDL_SCANCODE_PAUSE:       return Key::PAUSE;
        default:                       return Key::UNKNOWN;
    }
}

void SDLKeyboardManager::shutdown()
{
    orApplicationPtr->keyboardSignal().disconnect<SDLKeyboardManager, &SDLKeyboardManager::translateEvent>(this);
    orApplicationPtr->updateSignal().disconnect<IKeyboardManager, &IKeyboardManager::onUpdate>(this);
    orConsoleRef.print(orConsoleRef.Info, "KEYBOARDMANAGER: Shutdown\n");
}

void SDLKeyboardManager::translateEvent(const Event& keyEvent)
{
    Key key = keyEvent.eventData.keyboardEvent.scanCode;

    if (keyEvent.type == Event::EV_KEY_PRESSED)
        m_pressedKeys[key] = true;
    else if (keyEvent.type == Event::EV_KEY_RELEASED)
    {
        m_pressedKeys[key] = false;
        m_releasedKeys[key] = true;
    }
}
