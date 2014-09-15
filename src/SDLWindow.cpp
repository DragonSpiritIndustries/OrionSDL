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

#include "SDLWindow.hpp"
#include "Console.hpp"
#include "CVar.hpp"
#include <SDL2/SDL.h>

extern CVar* sys_title;
extern CVar* com_windowWidth;
extern CVar* com_windowHeight;
SDLWindow::SDLWindow()
    : m_window(nullptr)
{
}

SDLWindow::~SDLWindow()
{
    SDL_DestroyWindow(m_window);
}

bool SDLWindow::initialize()
{
    m_window = SDL_CreateWindow(sys_title->toLiteral().c_str(), // title
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // location
                                com_windowWidth->toInteger(), com_windowHeight->toInteger(), // size
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr)
    {
        orConsoleRef.print(orConsoleRef.Fatal, SDL_GetError());
        return false;
    }

    return true;
}

void SDLWindow::setTitle(const std::string& title)
{
    if (title == SDL_GetWindowTitle(m_window))
        return;

    SDL_SetWindowTitle(m_window, title.c_str());
    if (title != sys_title->toLiteral())
    {
        CVarUnlocker unlock(sys_title);
        sys_title->fromLiteral(title);
    }
}

std::string SDLWindow::title() const
{
    return sys_title->toLiteral();
}

void* SDLWindow::handle() const
{
    return reinterpret_cast<void*>(m_window);
}

Vector2i SDLWindow::windowSize()
{
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);

    return Vector2i(w, h);
}

void SDLWindow::setWindowSize(int w, int h)
{
    SDL_SetWindowSize(m_window, w, h);
}

void SDLWindow::setWindowSize(const Vector2i& size)
{
    setWindowSize(size.x, size.y);
}

void SDLWindow::setWindowWidth(int w)
{
    setWindowSize(w, windowHeight());
}

int SDLWindow::windowWidth()
{
    return windowSize().x;
}

void SDLWindow::setWindowHeight(int h)
{
    setWindowSize(windowWidth(), h);
}

int SDLWindow::windowHeight()
{
    return windowSize().y;
}
