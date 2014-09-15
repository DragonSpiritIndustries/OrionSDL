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

#include "SDLRenderer.hpp"
#include "SDLWindow.hpp"
#include "Console.hpp"
#include <SDL2/SDL_render.h>
#include "SDL2/SDL_opengl.h"
#include "CVar.hpp"
#include <limits.h>

extern CVar* com_clear;

float toFloat(int val)
{
    return (float)(val*(1.f/255.f));
}

SDLRenderer::SDLRenderer()
    : m_context(nullptr)
{
}

SDLRenderer::~SDLRenderer()
{
    SDL_GL_DeleteContext(m_context);
}

void SDLRenderer::setClearColor(const Colorf& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void SDLRenderer::clear()
{
    if (!com_clear->toBoolean())
        return;

    //Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SDLRenderer::present()
{
    SDL_GL_SwapWindow(m_windowHandle);
}

void SDLRenderer::drawRect(int w, int h, int x, int y, bool fill, Colorb col)
{
    static SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_BlendMode oldMode;
    SDL_GetRenderDrawBlendMode(m_renderer, &oldMode);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_renderer, (int)(col.r), (int)(col.g), (int)(col.b), (int)(col.a));
    if (fill)
        SDL_RenderFillRect(m_renderer, &rect);
    else
        SDL_RenderDrawRect(m_renderer, &rect);
    SDL_SetRenderDrawBlendMode(m_renderer, oldMode);
}

void* SDLRenderer::handle()
{
    return reinterpret_cast<void*>(m_renderer);
}

void SDLRenderer::setVSync(bool enable)
{
    //Use Vsync
    if( SDL_GL_SetSwapInterval(enable) < 0 )
    {
        orConsoleRef.print(orConsoleRef.Warning, "Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }
}

bool SDLRenderer::initialize(IWindow* window)
{
    m_windowHandle =reinterpret_cast<SDL_Window*>(window->handle());
    m_context = SDL_GL_CreateContext(m_windowHandle);
    if (m_context == nullptr)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "%s", SDL_GetError());
        return false;
    }
    m_renderer = SDL_CreateRenderer(m_windowHandle, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "%s", SDL_GetError());
        return false;
    }
    return true;
}

