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

#include "SDLTextureResource.hpp"
#include "physfsrwops.h"
#include <SDL2/SDL_image.h>
#include <ApplicationBase.hpp>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <GL/gl.h>

SDLTextureResource::SDLTextureResource(const std::string& path, SDL_Texture* texture)
    : ITextureResource(path),
      m_texture(texture)
{
    Uint32 format;
    int access, w, h;
    SDL_QueryTexture(m_texture, &format, &access, &w, &h);
    m_size = Vector2i(w, h);
}

SDLTextureResource::~SDLTextureResource()
{
    SDL_DestroyTexture(m_texture);
}

void SDLTextureResource::draw(float x, float y)
{
    draw(Rectanglef(x, y, width(), height()), Rectanglef(0, 0, -1, -1), Vector2f(), false, false, 0.0f);
}

void SDLTextureResource::draw(const Vector2f& position)
{
    draw(position.x, position.y);
}

void SDLTextureResource::draw(float x, float y, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)
{
    draw(Rectanglef(x, y, width(), height()), subrect, origin, flipH, flipV, angle);
}

void SDLTextureResource::draw(const Rectanglef& rect, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)
{
    static SDL_Rect locRect;
    locRect.x = rect.x;
    locRect.y = rect.y;
    locRect.w = (rect.w >= 0.0f ? rect.w : width());
    locRect.h = (rect.h >= 0.0f ? rect.h : height());

    static SDL_Rect partRect;
    partRect.x = subrect.x;
    partRect.y = subrect.y;
    partRect.w = (subrect.w >= 0.0f ? subrect.w : width());
    partRect.h = (subrect.h >= 0.0f ? subrect.h : height());

    static SDL_Point point;
    point.x = origin.x;
    point.y = origin.y;


    int flip = SDL_FLIP_NONE;
    if (flipH)
        flip |= SDL_FLIP_HORIZONTAL;
    if (flipV)
        flip |= SDL_FLIP_VERTICAL;

    SDL_RenderCopyEx(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), m_texture, &partRect, &locRect, angle, &point, (SDL_RendererFlip)flip);
}

Vector2i SDLTextureResource::size() const
{
    return m_size;
}

int SDLTextureResource::width() const
{
    return m_size.x;
}

int SDLTextureResource::height() const
{
    return m_size.y;
}

IResource* SDLTextureResource::loadTexture(const std::string& path)
{
    if (PHYSFS_exists(path.c_str()))
    {
        SDL_Surface* imgSurf = IMG_Load_RW(PHYSFSRWOPS_openRead(path.c_str()), 1);
        if (imgSurf)
        {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), imgSurf);
            SDL_FreeSurface(imgSurf);
            if (tex)
                return new SDLTextureResource(path, tex);
        }
    }

    return nullptr;
}
