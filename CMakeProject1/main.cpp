#include "matrix.h"
#include "parsecfg.h"
#include "simulation.h"

int main()
{
    ParseCfg::getInstance().parseJsonConf();
    Matrix::getInstance().creatAdjacentMatrix();
    Simulation::getInstance().startSimulation();
    Simulation::getInstance().show();
    return 0;
}