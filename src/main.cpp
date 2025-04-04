#include <iostream>
#include <v6turbo/engine/Engine.h>

int main(int argc, char const **argv)
{
    if (argc < 2)
    {
        std::cerr << "No source files provided." << std::endl;
        return 1;
    }

    v6::Engine engine{};

    engine.Run(argv[1]);

    return 0;
}
