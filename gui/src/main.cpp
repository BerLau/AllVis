#include "imgui_window.h"
#include <iostream>
#include <fstream>
int main()
{
    GUI::Window window;
    window.show(true);
    std::cout << "QUIT" << std::endl;
    Rendering::shader_program_factory.destroy();
    return 0;
}