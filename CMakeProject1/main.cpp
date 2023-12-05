#include <iostream>
#include "parsejson.h"

int main()
{
    MySimulation::getInstance().startSimulation();
    MySimulation::getInstance().showData();
    return 0;
}