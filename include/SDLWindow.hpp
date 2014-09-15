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

#ifndef SDLWINDOW_HPP
#define SDLWINDOW_HPP

#include "Global.hpp"
#include <string>
#include <IWindow.hpp>

struct SDL_Window;
class SDLWindow : public IWindow
{
public:
    SDLWindow();
    ~SDLWindow();

    bool initialize();

    void setTitle(const std::string& title);
    std::string title() const;

    void* handle() const;

    Vector2i windowSize();
    void setWindowSize(int w, int h);
    void setWindowSize(const Vector2i& size);
    void setWindowWidth(int w);
    int windowWidth();
    void setWindowHeight(int h);
    int windowHeight();
private:
    SDL_Window* m_window;
};

#endif // SDLWINDOW_HPP
