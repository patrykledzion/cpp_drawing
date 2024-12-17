#include <iostream> 
#include "Program.h"


int main()
{
    Program* program = new Program();

    while (true)
    {
        program->Update();
        program->Draw();   
    }
}
 