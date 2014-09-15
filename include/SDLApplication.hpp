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

#ifndef SDLAPPLICATION_HPP
#define SDLAPPLICATION_HPP

#include "ApplicationBase.hpp"
#include "SDLWindow.hpp"
#include "SDLRenderer.hpp"
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_ttf.h>

class SDLApplication : public ApplicationBase
{
public:
    SDLApplication();
    ~SDLApplication();

    int exec();
    bool init(int argc, char* argv[]);
    void close();
    void onDraw();
    void onExit();
    void* rendererHandle();
    void drawDebugText(const std::string& text, float x, float y, Colorb col=Colorb::white);
    void drawDebugText(const std::string& text, const Vector2f& position, Colorb col=Colorb::white);
    void drawRectangle(int w, int h, int x, int y, bool fill = false, Colorb col=Colorb::white);
    void setTitle(const std::string &title);
    std::string title() const;

    Vector2i windowSize();
    int windowWidth();
    int windowHeight();
    virtual void setClearColor(const Colorf& color = Colorf::black);

protected:
    void pollEvents();
    void applyViewport();
private:
    IFontResource* m_debugFont;
    void updateFPS();
    enum {MaxFrameValues=32};
    bool          m_running;

    unsigned      m_lastFrame;
    unsigned      m_frameCount;
    unsigned      m_frameValues[MaxFrameValues];
};

#endif // SDLAPPLICATION_HPP
