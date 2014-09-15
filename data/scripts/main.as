#include "common.as"

void helloWorld()
{
    orConsole.print(Console::Message, "Hello World!");
}

void onInitialized()
{
    orConsole.print(Console::Message, "Application Initialized");
}

void onStart()
{
    orApplication.setTitle("Orion TechDemo INTERNAL PRE-ALPHA");
}

void onUpdate(float delta)
{
    if (orKeyboardManager.keyReleased(Keyboard::Key::ESCAPE))
        orApplication.quit();

    if (orKeyboardManager.keyReleased(Keyboard::Key::F12))
        orScriptEngine.clean();
}

void onShutdown()
{
   orConsole.print(Console::Message, "Application Shutdown");
}

