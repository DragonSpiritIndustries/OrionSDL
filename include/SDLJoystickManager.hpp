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

#ifndef SDLJOYSTICKMANAGER_HPP
#define SDLJOYSTICKMANAGER_HPP

#include "IJoystickManager.hpp"
#include "unordered_map"
#include "vector"
#include "Event.hpp"
#include "SDL2/SDL_joystick.h"
#include "SDL2/SDL_haptic.h"

struct SDLJoystickDevice
{
    int id;
    SDL_Joystick* device;
    int buttonCount;
    int axisCount;
    bool hasRumble;
    std::string name;
    SDL_Haptic* forceFeedback;
    SDL_JoystickGUID guid;
    int currentEffect;
};

class SDLJoystickManager : public IJoystickManager
{
public:
    SDLJoystickManager();
    ~SDLJoystickManager();
    void onJoystickAdded(int which);
    void onJoystickRemoved(int which);

    float axisPosition(int which, int axis);
    bool buttonPressed(int which, int button);
    bool buttonReleased(int which, int button);

    std::string name(int which);

    int maxButtons(int which);
    int maxAxes(int which);

    void motorOn(int which);
    void motorOff(int which);
    void shutdown();
    bool isPluggedIn(int which);

    void update(float);
private:
    SDLJoystickDevice* joystick(int which);
    void onButton(const Event& joy);
    std::vector<SDLJoystickDevice*> m_devices;
};

#endif // SDLJOYSTICKMANAGER_HPP
