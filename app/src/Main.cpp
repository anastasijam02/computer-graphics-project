#include <engine/core/Engine.hpp>
#include "MyApp.h"

/**
 * Start here...
 */
int main(int argc, char **argv) {
    return std::make_unique<MyApp>()->run(argc, argv);
    return 0;
}
