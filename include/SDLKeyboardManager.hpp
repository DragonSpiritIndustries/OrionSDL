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

#ifndef KEYBOARDMANAGER_HPP
#define KEYBOARDMANAGER_HPP

#include "IKeyboardManager.hpp"
#include <unordered_map>

class SDLKeyboardManager : public IKeyboardManager
{
public:
    SDLKeyboardManager();
    ~SDLKeyboardManager();

    bool keyPressed(Key key);
    bool keyReleased(Key key);
    bool controlPressed();
    bool altPressed();
    bool shiftPressed();

    void shutdown();
protected:
    void translateEvent(const Event& ev);
private:
};

#endif // KEYBOARDMANAGER_HPP
