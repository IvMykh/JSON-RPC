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

    std::cout << std::endl 
              << "Press <Enter> to exit...";
    std::getchar();
    
    return 0;
}
