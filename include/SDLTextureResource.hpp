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

#ifndef SDLTEXTURE_HPP
#define SDLTEXTURE_HPP

#include <ITextureResource.hpp>
#include <SDL2/SDL.h>

class SDLTextureResource : public ITextureResource
{
public:
    SDLTextureResource(const std::string& path, SDL_Texture* texture);
    ~SDLTextureResource();

    virtual void draw(float x, float y);
    virtual void draw(const Vector2f& position);
    virtual void draw(float x, float y, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle);
    virtual void draw(const Rectanglef& rect, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle);
    Vector2i size() const;
    int width()     const;
    int height()    const;
    static IResource* loadTexture(const std::string& path);
private:
    SDL_Texture* m_texture;
};

#endif // SDLTEXTURE_HPP
