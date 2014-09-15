#include "common.as"
//#ifndef DEBUG
//#define DEBUG
//#endif
import void helloWorld() from "main";
weakref<Texture> textureRef;

enum State
{
    None      = 0,
    Idle      = (1 << 0),
    WalkNorth = (1 << 1),
    WalkSouth = (1 << 2),
    WalkEast  = (1 << 3),
    WalkWest  = (1 << 4)
}

bool isIdle(Object@ self)
{
    return (self.state() & State::Idle != 0);
}

void onCreate(Object@ self)
{
    helloWorld();
#ifdef DEBUG
    orConsole.print(Console::Warning, "DEBUG MODE");
#endif

    CVar@ moveLeft = orCVarManager.cvar("player1.moveLeft");
    CVar@ moveRight = orCVarManager.cvar("player1.moveRight");
    if (moveLeft is null)
        @moveLeft = orCreateBinding("player1.moveLeft", "Moves player1 left", CVar::Archive | CVar::System);
    if (moveRight is null)
        @moveRight = orCreateBinding("player1.moveRight", "Moves player1 right", CVar::Archive | CVar::System);

    orConsole.print(Console::Message, "" + moveLeft.joyValid(0));

    Transformable@ comp = cast<Transformable>(self.addComponent("Transformable", "transformable"));
    if (comp is null)
        orConsole.print(Console::Warning, "Failed to add Transformable");

    Texture @tex = textureRef.get();
    if (tex is null)
    {
        @tex = orLoadTexture("test/test.png");
        if (tex !is null)
            textureRef = weakref<Texture>(@tex);
    }
}
    
void onThink(Object@ self, float delta)
{
    if (isIdle(self))
        return;
}

void onUpdate(Object@ self, float delta)
{
    if (self is null)
        return;

    CVar@ moveLeftJoy    = orCVarManager.cvar("player1.moveLeft");
    CVar@ moveRightJoy   = orCVarManager.cvar("player1.moveRight");
    if (!moveLeftJoy.isBinding() || !moveRightJoy.isBinding())
    {
        orConsole.print(Console::Error, "" + (!moveLeftJoy.isBinding() ? "moveLeft" : "moveRight") + " is not a binding");
        return;
    }

    int joy = 0;
    int axisLeft = moveLeftJoy.joyAxis(joy);
    int axisRight = moveRightJoy.joyAxis(joy);
    float moveLeftValue  = orJoystickManager.axisPosition(joy, axisLeft);
    float moveRightValue = orJoystickManager.axisPosition(joy, axisRight);
    Vector2f move;
    if ((moveLeftJoy.isAxisNegative(joy) && moveLeftValue < 0.0f) || (!moveLeftJoy.isAxisNegative(joy) && moveRightValue < 0.0f))
        move.x = 94.f*moveLeftValue*delta;
    if ((moveRightJoy.isAxisNegative(joy) && moveLeftValue > 0.0f) || (!moveRightJoy.isAxisNegative(joy) && moveRightValue > 0.0f))
        move.x = 94.f*moveRightValue*delta;
        
    if (orKeyboardManager.keyPressed(moveLeftJoy.key()))
       move.x -= 64.f*delta;
    if (orKeyboardManager.keyPressed(moveRightJoy.key()))
       move.x += 64.f*delta;
    move.y = 94.f*orJoystickManager.axisPosition(joy, 1)*delta;
    if (move.x == 0 && move.y == 0)
    {
        self.setState(State::Idle);
        return;
    }
    int state = 0;
    if (move.x > 0)
    {
        state &= ~State::WalkEast;
        state |= State::WalkWest;
    }
    else if (move.x < 0)
    {
        state &= ~State::WalkWest; 
        state |= State::WalkEast;
    }

    if (move.y > 0)
    {
        state &= ~State::WalkNorth;
        state |= State::WalkSouth;
    }
    else if (move.y < 0)
    {
        state &= ~State::WalkSouth;
        state |= State::WalkNorth;
    }
    self.setState(state);
    self.move(move);
}

void onDraw(Object@ self)
{
    Texture @tex = textureRef.get();
    Vector2f origin(tex.width()/2, tex.height());
    Rectanglef subrect(0.0f, 0.0f, tex.width(), tex.height());

    if (tex !is null)
        tex.draw(100.f, 100.f, subrect, origin, false, false, -interpolate(orJoystickManager.axisPosition(0, 2),orJoystickManager.axisPosition(0, 3), 181.f));
}

void onDestroyed(Object@ self)
{
    orConsole.print(Console::Message, "I'm dying, you stupid shit!!!");
    orJoystickManager.motorOff(0);
}

