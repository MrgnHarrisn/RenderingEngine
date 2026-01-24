#include <iostream>

#include "Engine.h"

int main()
{
    Engine engine(800, 900, "BoxEngine Window");
    // engine.setClearColor(0.1, 0.3, 0.6);
    engine.init();
    engine.run();
    return 0;
}