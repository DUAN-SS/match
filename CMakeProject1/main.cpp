#include "simulation.h"

int main() {
  MySimulation::getInstance().startSimulation();
  MySimulation::getInstance().showData();
  return 0;
}