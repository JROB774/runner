/// WIN32 /////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    while (true)
    {
        J_System::initialise();
        Container::initialise();

        J_System::getFpsTimer()->start();

        J_Window::show();

        while (J_System::getState() == J_System::STATE_ACTIVE)
        {
            J_System::getCapTimer()->start();

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

        Container::terminate();
        J_System::terminate();

        if (J_System::getState() == J_System::STATE_RESETTING) { continue; }
        else { break; }
    }

    return 0;
}

/// WIN32 /////////////////////////////////////////////////////////////////////
