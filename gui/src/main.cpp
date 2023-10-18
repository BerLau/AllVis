#include "imgui_window.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

int main()
{
    srand(time(NULL));
    // srand(114514);
    GUI::Window window;
    window.show(true);
    std::cout << "QUIT" << std::endl;
    Rendering::shader_program_factory.destroy();
    return 0;
}