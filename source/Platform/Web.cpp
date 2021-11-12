/// WEB ///////////////////////////////////////////////////////////////////////

#include <emscripten.h>

void main_loop ()
{
    while (J_System::pollEvent() != 0)
    {
        J_System::handle();
        Container::handle();
    }

    J_System::stepBegin();
    Container::step();
    Container::render();
    J_System::stepEnd();
}

int main (int argc, char* argv[])
{
    J_System::initialise();
    Container::initialise();

    J_Window::show();

    emscripten_set_main_loop(main_loop, -1, 1);

    return 0;
}

/// WEB ///////////////////////////////////////////////////////////////////////
