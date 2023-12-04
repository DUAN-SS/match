#include <iostream>
#include "parsejson.h"

int main()
{
    MySimulation simuJson;
    simuJson.initJsonConf();
    simuJson.displayResult();
    simuJson.showData();
    return 0;
}