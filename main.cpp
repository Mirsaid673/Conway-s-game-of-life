#include <iostream>

#include "life/game.h"

Application *app;

int main()
{
    Game mp;
    app = &mp;
    return app->run();
}