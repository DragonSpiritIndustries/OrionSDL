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

#include "SDLJoystickManager.hpp"
#include "ApplicationBase.hpp"
#include <algorithm>
#include <cstring>

SDLJoystickManager::SDLJoystickManager()
{
    orApplicationPtr->joystickAddedSignal().connect<SDLJoystickManager, &SDLJoystickManager::onJoystickAdded>(this);
    orApplicationPtr->joystickRemovedSignal().connect<SDLJoystickManager, &SDLJoystickManager::onJoystickRemoved>(this);
    orApplicationPtr->joystickSignal().connect<SDLJoystickManager, &SDLJoystickManager::onButton>(this);
    orApplicationPtr->updateSignal().connect<SDLJoystickManager, &SDLJoystickManager::update>(this);
}

SDLJoystickManager::~SDLJoystickManager()
{
}

void SDLJoystickManager::onJoystickAdded(int which)
{
    if (m_devices.size() >= MaxJoysticks)
    {
        orConsoleRef.print(orConsoleRef.Info, "Joystick limit %i reached, ignoring new controller", MaxJoysticks);
        return;
    }

    SDLJoystickDevice* device = new SDLJoystickDevice;
    device->device = SDL_JoystickOpen(which);
    if (!device->device)
    {
        delete device;
        orConsoleRef.print(orConsoleRef.Error, "Error obtaining controller %i\n", which);
    }
    else
    {
        orConsoleRef.print(orConsoleRef.Info, "Obtained controller:\n");
        device->name = std::string(SDL_JoystickNameForIndex(which));
        // Load metadata
        loadMetaData(device->name);
        orConsoleRef.print(orConsoleRef.Info, "Name: %s\n", device->name.c_str());
        device->id = SDL_JoystickInstanceID(device->device);
        orConsoleRef.print(orConsoleRef.Info, "Device slot: %i\n", device->id);
        device->axisCount = SDL_JoystickNumAxes(device->device);
        orConsoleRef.print(orConsoleRef.Info, "Axis count: %i\n", device->axisCount);
        device->buttonCount = SDL_JoystickNumButtons(device->device);
        orConsoleRef.print(orConsoleRef.Info, "Button Count: %i\n", device->buttonCount);
        device->forceFeedback = SDL_HapticOpenFromJoystick(device->device);
        device->hasRumble = (device->forceFeedback != nullptr);
        if (device->hasRumble)
            SDL_HapticRumbleInit(device->forceFeedback);

        orConsoleRef.print(orConsoleRef.Info, "Force feedback: %s\n", (device->hasRumble ? "yes" : "no"));
        device->guid = SDL_JoystickGetGUID(device->device);
        char guidString[33];
        SDL_JoystickGetGUIDString(device->guid, guidString, sizeof(guidString));
        orConsoleRef.print(orConsoleRef.Info, "GUID: %s\n", guidString);
        device->currentEffect = -1;
        m_devices.push_back(device);
    }
}

void SDLJoystickManager::onJoystickRemoved(int which)
{
    SDLJoystickDevice* device = joystick(which);
    if (!device)
        return;

    std::vector<SDLJoystickDevice*>::iterator iter = std::find(m_devices.begin(), m_devices.end(), device);

    if (device->hasRumble)
        SDL_HapticClose(device->forceFeedback);

    SDL_JoystickClose(device->device);

    orConsoleRef.print(orConsoleRef.Info, "Controller removed: %s\n", device->name.c_str());
    m_devices.erase(iter);
    delete device;
    device = nullptr;
}

float SDLJoystickManager::axisPosition(int which, int axis)
{
    if (which < 0 || which >= (int)m_devices.size())
        return 0.0f;

    // SDL Stores the axis position as a signed int, we need a float value
    // fortunately it gets stored the the correct range for a simple division
    // Normally you should divide by 32768.f in order to get the correct value, however it seems to cap it
    // to between -0.9 and +0.9
    SDLJoystickDevice* device = m_devices[which];
    return SDL_JoystickGetAxis(device->device, axis)/32767.0f;
}

bool SDLJoystickManager::buttonPressed(int which, int button)
{
    if (which < 0 || which >= m_devices.size())
        return false;

    return SDL_JoystickGetButton(m_devices[which]->device, button);
}

bool SDLJoystickManager::buttonReleased(int which, int button)
{
    if (which < 0 || which >= m_devices.size())
        return false;
    SDLJoystickDevice* device = m_devices[which];
    return m_releasedButtons[device->id][button];
}

std::string SDLJoystickManager::name(int which)
{
    if (which < 0 || which >= m_devices.size())
        return std::string();

    return m_devices[which]->name;
}

int SDLJoystickManager::maxButtons(int which)
{
    if (which < 0 || which >= m_devices.size())
        return 0;

    return m_devices[which]->buttonCount;
}

int SDLJoystickManager::maxAxes(int which)
{
    if (!joystick(which))
        return 0;

    return m_devices[which]->buttonCount;
}

void SDLJoystickManager::motorOn(int which)
{
    if (which < 0 || which >= m_devices.size())
        return;

    SDLJoystickDevice* device = m_devices[which];

    if (device->hasRumble)
        SDL_HapticRumblePlay(device->forceFeedback, 5.f, 5000*10000);
}

void SDLJoystickManager::motorOff(int which)
{
    if (which < 0 || which >= m_devices.size())
        return;

    SDLJoystickDevice* device = m_devices[which];

    if (device->hasRumble && device->currentEffect != -1)
        SDL_HapticRumbleStop(device->forceFeedback);
}

void SDLJoystickManager::shutdown()
{
    orApplicationPtr->joystickAddedSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::onJoystickAdded>(this);
    orApplicationPtr->joystickRemovedSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::onJoystickRemoved>(this);
    orApplicationPtr->joystickSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::onButton>(this);
    orApplicationPtr->updateSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::update>(this);
    for (SDLJoystickDevice* device : m_devices)
    {
        if (device->hasRumble)
            SDL_HapticClose(device->forceFeedback);

        SDL_JoystickClose(device->device);
        delete device;
        device = nullptr;
    }
    m_devices.clear();
}

bool SDLJoystickManager::isPluggedIn(int which)
{
    if (joystick(which))
        return true;

    return false;
}

void SDLJoystickManager::update(float)
{
    // ensure the joysticks are getting updated properly
    SDL_JoystickUpdate();
}

SDLJoystickDevice* SDLJoystickManager::joystick(int which)
{
    std::vector<SDLJoystickDevice*>::iterator iter = std::find_if(m_devices.begin(), m_devices.end(),
                                                                  [&which](SDLJoystickDevice* d)->bool{ return d->id == which;});
    if (iter == m_devices.end())
        return nullptr;

    return *iter;
}

void SDLJoystickManager::onButton(const Event& joy)
{
    if (joy.type == Event::EV_JOY_BTN_RELEASED)
        m_releasedButtons[joy.eventData.joystickEvent.id][joy.eventData.joystickEvent.button] = true;
    else if (joy.type == Event::EV_JOY_BTN_PRESSED)
        m_releasedButtons[joy.eventData.joystickEvent.id][joy.eventData.joystickEvent.button] = false;
}
