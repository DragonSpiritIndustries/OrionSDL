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

#include "SDLApplication.hpp"
#include "SDLKeyboardManager.hpp"
#include "SDLJoystickManager.hpp"
#include "SDLMouseManager.hpp"
#include "IFontResource.hpp"
#include "ScriptEngine.hpp"
#include "ObjectManager.hpp"
#include "Console.hpp"
#include "Global.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <Athena/Utility.hpp>
#include "angelscript/addons.h"
#include "CVarManager.hpp"

extern CVar* sys_title;

SDLApplication::SDLApplication()
    : m_running(false)
{
}

SDLApplication::~SDLApplication()
{
}

int SDLApplication::exec()
{
    ApplicationBase::onStart();
    while (m_running)
    {
        updateFPS();
        onUpdate();
        pollEvents();
        onDraw();
    }
    onExit();
    return 0;
}

bool SDLApplication::init(int argc, char* argv[])
{
    int code= 0;
    if ((code = SDL_Init(SDL_INIT_EVERYTHING)) < 0)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "SDL Failed to initalize: %s(%i)", SDL_GetError(), code);
        return false;
    }

    if (!ApplicationBase::init(argc, argv))
        return false;
    orConsoleRef.print(orConsoleRef.Info, "Orion " orVERSION_STR " " orRELEASE_NAME " SDL Application");
    parseCommandLine(argc, argv);

    memset(m_frameValues, 0, sizeof(m_frameValues));
    m_frameCount = 0;
    m_fps = 0.f;
    m_lastFrame = SDL_GetTicks();

    if (TTF_Init() == -1)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "%s", TTF_GetError());
        return false;
    }

    // Attempt to load debug font
    m_debugFont = orResourceManagerRef.loadResource<IFontResource>("fonts/debug.ttf");

    if (!m_debugFont)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "Unable to obtain debug font: %s", TTF_GetError());
        return false;
    }

    m_running = true;
    return true;
}

void SDLApplication::close()
{
    m_running = false;
}

void SDLApplication::pollEvents()
{
    SDL_Event sdlEvent;
    // TODO: Create Key,Mouse, and Joystick mapping enumerations
    // TODO: Add a more complete event list
    while (SDL_PollEvent(&sdlEvent))
    {
        Event oEvent;
        if (sdlEvent.type == SDL_QUIT)
        {
            oEvent.type = Event::EV_QUIT;
            m_eventSignal(oEvent);
            m_running = false;
        }
        else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
        {
            oEvent.type = (sdlEvent.type == SDL_KEYDOWN ? Event::EV_KEY_PRESSED
                                                        : Event::EV_KEY_RELEASED);
            oEvent.eventData.keyboardEvent.pressed  = (oEvent.type == Event::EV_KEY_PRESSED);
            oEvent.eventData.keyboardEvent.keyCode  = sdlEvent.key.keysym.sym;
            oEvent.eventData.keyboardEvent.scanCode = orFromScanCode(sdlEvent.key.keysym.scancode);
            oEvent.eventData.keyboardEvent.modifier = 0;
            short mod = sdlEvent.key.keysym.mod;
            if (mod & KMOD_LSHIFT)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::LSHIFT;
            if (mod & KMOD_RSHIFT)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::RSHIFT;
            if (mod & KMOD_LCTRL)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::LCONTROL;
            if (mod & KMOD_RCTRL)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::RCONTROL;
            if (mod & KMOD_LALT)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::LALT;
            if (mod & KMOD_RALT)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::RALT;
            if (mod & KMOD_LGUI)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::LGUI;
            if (mod & KMOD_RGUI)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::RGUI;
            if (mod & KMOD_NUM)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::NUM;
            if (mod & KMOD_CAPS)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::CAPS;
            if (mod & KMOD_MODE)
                oEvent.eventData.keyboardEvent.modifier |= (short)KeyModifier::MODE;

            m_eventSignal(oEvent);
            m_keyboardSignal(oEvent);
        }
        // TODO: Should text editing be handled separately? could be useful for a GUI implementation
        else if (sdlEvent.type == SDL_TEXTINPUT || sdlEvent.type == SDL_TEXTEDITING)
        {
            oEvent.type = Event::EV_TEXT_ENTERED;
            oEvent.eventData.textEvent.string = sdlEvent.text.text;
            m_eventSignal(oEvent);
            m_textSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_JOYDEVICEADDED)
        {
            m_joystickAddedSignal(sdlEvent.cdevice.which);
        }
        else if (sdlEvent.type == SDL_JOYDEVICEREMOVED)
        {
            m_joystickRemovedSignal(sdlEvent.cdevice.which);
        }
        else if (sdlEvent.type == SDL_JOYAXISMOTION)
        {
            oEvent.type = Event::EV_JOY_AXIS;
            oEvent.eventData.joystickEvent.id = sdlEvent.caxis.which;
            oEvent.eventData.joystickEvent.axis = sdlEvent.caxis.axis;
            oEvent.eventData.joystickEvent.position = sdlEvent.caxis.value/32768.f;
            m_eventSignal(oEvent);
            m_joystickSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_JOYBUTTONDOWN || sdlEvent.type == SDL_JOYBUTTONUP)
        {
            oEvent.type = (sdlEvent.type == SDL_JOYBUTTONDOWN ? Event::EV_JOY_BTN_PRESSED
                                                              : Event::EV_JOY_BTN_RELEASED);
            oEvent.eventData.joystickEvent.id = sdlEvent.cbutton.which;
            oEvent.eventData.joystickEvent.button = sdlEvent.cbutton.button;
            m_eventSignal(oEvent);
            m_joystickSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP)
        {
            oEvent.type = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? Event::EV_MOUSE_BTN_PRESSED
                                                                : Event::EV_MOUSE_BTN_RELEASED);
            oEvent.eventData.mouseButtonEvent.button = sdlEvent.button.button;
            oEvent.eventData.mouseButtonEvent.x      = sdlEvent.button.x;
            oEvent.eventData.mouseButtonEvent.y      = sdlEvent.button.y;
            m_eventSignal(oEvent);
            m_mouseButtonSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_MOUSEWHEEL)
        {
            oEvent.type = Event::EV_MOUSE_WHEEL;
            int x,y;
            SDL_GetMouseState(&x, &y);
            oEvent.eventData.mouseWheelEvent.x = x;
            oEvent.eventData.mouseWheelEvent.y = y;
            oEvent.eventData.mouseWheelEvent.vDelta = sdlEvent.wheel.y;
            oEvent.eventData.mouseWheelEvent.hDelta = sdlEvent.wheel.x;
            m_eventSignal(oEvent);
            m_mouseWheelSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_MOUSEMOTION)
        {
            oEvent.type = Event::EV_MOUSE_MOVED;
            oEvent.eventData.mouseMoveEvent.x = sdlEvent.motion.x;
            oEvent.eventData.mouseMoveEvent.y = sdlEvent.motion.y;
            m_eventSignal(oEvent);
            m_mouseMoveSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_WINDOWEVENT)
        {
            switch (sdlEvent.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    oEvent.type = Event::EV_WINDOW_RESIZED;
                    oEvent.eventData.resizeEvent.width = sdlEvent.window.data1;
                    oEvent.eventData.resizeEvent.height = sdlEvent.window.data2;
                    // Compare the addresses of the viewports to see if they're the same
                    // We can't just compare the two incase the user has a viewport that happens to
                    // Be the same size as the default (bad things would happen if we overrode their vp)
                    Viewport* defaultVP  = &m_defaultViewport;
                    bool resetVP = (m_currentViewport == defaultVP);

                    m_defaultViewport = Viewport(0, 0, sdlEvent.window.data1, sdlEvent.window.data2);
                    if (resetVP)
                        restoreDefaultViewport();
                    else
                        // if we're not resetting the viewport
                        // reapply it, for some reason SDL Updates the viewport
                        // when the window is resized
                        applyViewport();

                    m_eventSignal(oEvent);
                    m_resizeSignal(oEvent);
                }
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    oEvent.type = Event::EV_FOCUS_GAINED;
                    oEvent.eventData.focusEvent.focus = true;
                    m_eventSignal(oEvent);
                    m_focusSignal(oEvent);
                }
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                {
                    oEvent.type = Event::EV_FOCUS_LOST;
                    oEvent.eventData.focusEvent.focus = false;
                    m_eventSignal(oEvent);
                    m_focusSignal(oEvent);
                }
                    break;
            }
        }
    }
}

void SDLApplication::applyViewport()
{
    SDL_Rect viewport;
    viewport.x = std::fabs(m_currentViewport->x());
    viewport.y = std::fabs(m_currentViewport->y());
    viewport.w = m_currentViewport->width();
    viewport.h = m_currentViewport->height();
    SDL_RenderSetViewport(static_cast<SDL_Renderer*>(m_renderer.get()->handle()), &viewport);
}

void SDLApplication::updateFPS()
{
    unsigned frameIndex = m_frameCount % MaxFrameValues;

    unsigned ticks = SDL_GetTicks();

    m_frameValues[frameIndex] = ticks - m_lastFrame;
    m_frameTime = m_frameValues[frameIndex] / 1000.0f;
    m_lastFrame = ticks;

    m_frameCount++;

    unsigned count = 0;
    if (m_frameCount < MaxFrameValues)
        count = m_frameCount;
    else
        count = MaxFrameValues;

    // Should never happen buuut.
    if (count == 0)
        count = 1;

    m_fps = 0;
    for (Uint32 i = 0; i < count; i++)
        m_fps += m_frameValues[i];

    m_fps /= count;
    m_fps  = 1000.f / m_fps;
}

void SDLApplication::onDraw()
{
    m_renderer.get()->clear();
    ApplicationBase::onDraw();
    m_renderer.get()->present();
}

void SDLApplication::onExit()
{
    ApplicationBase::onExit();
    SDL_Quit();
}

void* SDLApplication::rendererHandle()
{
    return (void*)m_renderer.get()->handle();
}

void SDLApplication::drawDebugText(const std::string& text, float x, float y, Colorb col)
{
    m_debugFont->draw(x, y, text, 16, col);
}

void SDLApplication::drawDebugText(const std::string& text, const Vector2f& position, Colorb col)
{
    drawDebugText(text, position.x, position.y, col);
}

void SDLApplication::drawRectangle(int w, int h, int x, int y, bool fill, Colorb col)
{
    m_renderer.get()->drawRect(w, h, x, y, fill, col);
}

void SDLApplication::setTitle(const std::string& title)
{
    m_window.get()->setTitle(title);
}

std::string SDLApplication::title() const
{
    return m_window.get()->title();
}

Vector2i SDLApplication::windowSize()
{
    return m_window.get()->windowSize();
}

int SDLApplication::windowWidth()
{
    return m_window.get()->windowWidth();
}

int SDLApplication::windowHeight()
{
    return m_window.get()->windowHeight();
}

void SDLApplication::setClearColor(const Colorf& color)
{
    m_renderer.get()->setClearColor(color);
}
