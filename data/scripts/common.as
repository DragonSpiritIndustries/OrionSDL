#ifndef COMMON
#define COMMON
namespace CVar
{
    enum Type
    {
        Boolean,
        Integer,
        Float,
        Literal,
        Color,
        Bind
    };
    enum Flags
    {
        All      = -1,
        System   = (1 << 0),
        Game     = (1 << 1),
        Gui      = (1 << 2),
        Cheat    = (1 << 3),
        ReadOnly = (1 << 4),
        Archive  = (1 << 5),
        Modified = (1 << 6)
    };
}

float interpolate(const float a, const float b, const float bias)
{
	return a + ((b - a) * bias);
}
#endif
