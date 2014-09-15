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

#include "SDLFontResource.hpp"
#include "ApplicationBase.hpp"
#include "SDL2/SDL_ttf.h"
#include <physfs.h>
#include "Rectangle.hpp"

SDLFontResource::SDLFontResource(const std::string& path)
    : IFontResource(path)
{
    PHYSFS_File* handle = PHYSFS_openRead(path.c_str());
    m_fontLength = PHYSFS_fileLength(handle);
    m_fontData = (void*)(new atUint8[m_fontLength]);
    PHYSFS_read(handle, m_fontData, 1, m_fontLength);
    PHYSFS_close(handle);

    if (!TTF_WasInit())
        TTF_Init();
}

SDLFontResource::~SDLFontResource()
{
    orForeach(std::pair<int, TTF_Font*> font _in_ m_cachedFonts)
    {
        TTF_CloseFont(font.second);
    }
    delete[] m_fontData;
    m_cachedFonts.clear();
}

Vector2i SDLFontResource::measureString(const std::string& str, int size)
{
    if (m_cachedFonts.find(size) == m_cachedFonts.end())
        cacheFont(size);

    Vector2i fontSize;
    TTF_SizeText(m_cachedFonts[size], str.c_str(), &fontSize.x, &fontSize.y);

    return fontSize;
}

int SDLFontResource::glyphAdvance(atUint16 ch, int size)
{
    if (m_cachedFonts.find(size) == m_cachedFonts.end())
        cacheFont(size);

    int ret;
    TTF_GlyphMetrics(m_cachedFonts[size], ch, nullptr, nullptr, nullptr, nullptr, &ret);
    return ret;
}

void SDLFontResource::draw(float x, float y, const std::string& text, int size, const Colorb& color)
{
    if (m_cachedFonts.find(size) == m_cachedFonts.end())
        cacheFont(size);
    Vector2i fontSize = measureString(text, size);
    SDL_Rect rect;
    static SDL_Surface* fontSurf;
    static SDL_Texture* texture;
    rect.x = x;
    rect.y = y;
    rect.w = fontSize.x;
    rect.h = fontSize.y;
    fontSurf = TTF_RenderText_Blended(m_cachedFonts[size], text.c_str(), SDL_Color{color.r, color.g, color.b, color.a});
    texture = SDL_CreateTextureFromSurface(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), fontSurf);
    SDL_FreeSurface(fontSurf);
    SDL_RenderCopy(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
}

void SDLFontResource::cacheFont(int size)
{
    if (m_cachedFonts.find(size) != m_cachedFonts.end())
        return;
    SDL_RWops* ops = SDL_RWFromConstMem(m_fontData, m_fontLength);
    if (ops)
    {
        TTF_Font* font = TTF_OpenFontRW(ops, 0, size);
        if (font)
            m_cachedFonts[size] = font;
    }
}

IResource* SDLFontResource::loadFont(const std::string& path)
{
    if (PHYSFS_exists(path.c_str()))
        return new SDLFontResource(path);

    return nullptr;
}
