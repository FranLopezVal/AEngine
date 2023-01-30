
#include <iostream>

#include "AEWindow.h"


std::unique_ptr<AEWindow> window;

int main(int argc, char* args[]) {

    window = std::make_unique<AEWindow>();
   
    window.get()->Run();

    return 0;
}