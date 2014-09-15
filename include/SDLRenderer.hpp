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

#ifndef SDLRENDERER_HPP
#define SDLRENDERER_HPP

#include "IRenderer.hpp"
#include "SDLWindow.hpp"
#include "SDL2/SDL.h"

class SDLRenderer : public IRenderer
{
public:
    SDLRenderer();
    ~SDLRenderer();

    void setClearColor(const Colorf& color = Colorf::black);
    bool initialize(IWindow* window);
    void clear();
    void present();
    void drawRect(int w, int h, int x, int y, bool fill = false, Colorb col=Colorb::white);
    void* handle();
    void setVSync(bool enable);
private:
    SDL_Renderer*  m_renderer;
    SDL_GLContext  m_context;
    SDL_Window*    m_windowHandle;
};

#endif // SDLRENDERER_HPP
