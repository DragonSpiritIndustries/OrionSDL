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

#ifndef SDLMOUSEMANAGER_HPP
#define SDLMOUSEMANAGER_HPP

#include "IMouseManager.hpp"
#include <unordered_map>

class Event;
class SDLMouseManager : public IMouseManager
{
public:
    SDLMouseManager();
    void shutdown();
    bool buttonPressed(MouseButton button);
    bool buttonReleased(MouseButton button);

    Vector2i position();
    Vector2i wheelDelta();

    void setMousePosition(int x, int y);
    void setMousePosition(Vector2i position);
protected:
    void onMouseButton(const Event& e);
    void onMouseWheel(const Event& e);
};

#endif // SDLMOUSEMANAGER_HPP
