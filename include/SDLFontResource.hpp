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

#ifndef SDLFONTRESOURCE_HPP
#define SDLFONTRESOURCE_HPP

#include <IFontResource.hpp>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

class SDLFontResource : public IFontResource
{
public:
    SDLFontResource(const std::string& path);
    ~SDLFontResource();
    Vector2i measureString(const std::string &str, int size=16);
    int glyphAdvance(atUint16 ch, int size=16);
    void draw(float x, float y, const std::string& text, int size=16, const Colorb& color=Colorb::white);
    static IResource* loadFont(const std::string& path);
private:
    void cacheFont(int size);
    std::unordered_map<int, TTF_Font*> m_cachedFonts;
    Uint32 m_fontLength;
    void* m_fontData;
};

#endif // SDLFONTRESOURCE_HPP
