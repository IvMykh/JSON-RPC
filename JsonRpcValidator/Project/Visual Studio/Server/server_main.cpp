#include <iostream>
#include "server_app.h"



int main()
{
    try
    {
        RunApp();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl
                  << std::endl;
    }

    system("pause");
    return 0;
}
