#include "simulation.h"
#include "matrix.h"
#include "modeldata.h"
#include <cmath>
#include <iomanip>

Simulation::Simulation()
{
    m_outFile.open("data.csv");
    for (int i = 0; i < ModuleData::m_moduleList.size(); i++) {
        m_color.push_back(0);
    }
    m_cycle = std::vector<std::vector<int>>(0, std::vector<int>(0));
}

Simulation::~Simulation()
{
    m_outFile.close();
}

double Simulation::calculateSimulationResult(const std::string& lastModel, 
                                             const double& sinValue, 
                                             const bool& getPreValue)
{
    // if the module is cons.
    if (lastModel.find("cons") != std::string::npos) { // the cons is not be cycle. return the value.
        ModuleData::m_consModule.at(lastModel).output_value = ModuleData::m_consModule.at(lastModel).cons_value;
        return ModuleData::m_consModule[lastModel].output_value;
    }
    // if the module is sum.
    if (lastModel.find("sum") != std::string::npos) {
        if (getPreValue) { // if it is the star of cycle.
            return ModuleData::m_sumModule.at(lastModel).output_value;
        }
        int j = 0;
        for (j; j < 2; j++) { // search the input of sum.
            bool isCycle = false;

            for (int i = 0; i < m_cycle.size(); i++) {
                if (ModuleData::m_moduleList.at(m_cycle.at(i).at(1)) == lastModel
                    && ModuleData::m_sumModule.at(lastModel).input[j]
                            == ModuleData::m_moduleList.at(m_cycle.at(i).at(0))) {
                    isCycle = true; // it is cycle.
                    break;
                }
            }
            if (isCycle) { // has cycle, break.
                break;
            }
        }

        if (j == 2) { // the sum is not the cycle.
            ModuleData::m_sumModule[lastModel].output_value =
                    calculateSimulationResult(ModuleData::m_sumModule[lastModel].input[0], sinValue, false)
                    + calculateSimulationResult(ModuleData::m_sumModule[lastModel].input[1], sinValue, false);
        } else if (j == 0) { // the sum cycle with input 0
            ModuleData::m_sumModule[lastModel].output_value =
                    calculateSimulationResult(ModuleData::m_sumModule[lastModel].input[0], sinValue, true)
                    + calculateSimulationResult(ModuleData::m_sumModule[lastModel].input[1], sinValue, false);
        } else { // the sum cycle with input 1
            ModuleData::m_sumModule[lastModel].output_value =
                    calculateSimulationResult(ModuleData::m_sumModule[lastModel].input[0], sinValue, false)
                    + calculateSimulationResult(ModuleData::m_sumModule[lastModel].input[1], sinValue, true);
        }
        return ModuleData::m_sumModule[lastModel].output_value;
    }
    // if the module is gain.
    if (lastModel.find("gain") != std::string::npos) {
        if (getPreValue) { // if the gain is the start of cycle.
            std::cout << lastModel << ": pre: " << ModuleData::m_gainModule[lastModel].output_value << std::endl;

            return ModuleData::m_gainModule[lastModel].output_value;
        }
        // the gain output = the input * gain value;
        ModuleData::m_gainModule[lastModel].output_value = ModuleData::m_gainModule[lastModel].gain_value
                * calculateSimulationResult(ModuleData::m_gainModule[lastModel].info.input[0], sinValue, false);
        return ModuleData::m_gainModule[lastModel].output_value;
    }
    // is the module is sine.
    if (lastModel.find("sine") != std::string::npos) {
        // using the rad. if need use.
        // const double pi = atan(1.0) * 4;
        // sineModel[lastModel].outputValue = sineModel[lastModel].sineValue * sin(sinValue * pi / 180);
        // the sine value is the count of step * simulation step.
        ModuleData::m_sineModule[lastModel].output_value =
                ModuleData::m_sineModule[lastModel].amplitude * sin(sinValue);
        return ModuleData::m_sineModule[lastModel].output_value;
    }
    // if the module is mult.
    if (lastModel.find("mult") != std::string::npos) {
        if (getPreValue) { // if it is the star of cycle.
            return ModuleData::m_multModule[lastModel].output_value;
        }

        int j = 0;
        for (j; j < 2; j++) { // search the input of mult.
            bool isCycle = false;
            for (int i = 0; i < m_cycle.size(); i++) {
                if (ModuleData::m_moduleList.at(m_cycle.at(i).at(1)) == lastModel
                    && ModuleData::m_multModule[lastModel].input[j]
                            == ModuleData::m_moduleList.at(m_cycle.at(i).at(0))) {
                    isCycle = true;
                    break;
                }
            }

            if (isCycle) {
                break;
            }
        }

        if (j == 2) {
            ModuleData::m_multModule[lastModel].output_value =
                    calculateSimulationResult(ModuleData::m_multModule[lastModel].input[0], sinValue, false)
                    * calculateSimulationResult(ModuleData::m_multModule[lastModel].input[1], sinValue, false);
        } else if (j == 0) {
            ModuleData::m_multModule[lastModel].output_value =
                    calculateSimulationResult(ModuleData::m_multModule[lastModel].input[0], sinValue, true)
                    * calculateSimulationResult(ModuleData::m_multModule[lastModel].input[1], sinValue, false);
        } else {
            ModuleData::m_multModule[lastModel].output_value =
                    calculateSimulationResult(ModuleData::m_multModule[lastModel].input[0], sinValue, false)
                    * calculateSimulationResult(ModuleData::m_multModule[lastModel].input[1], sinValue, true);
        }
        return ModuleData::m_multModule[lastModel].output_value;
    }
}

void Simulation::dfs(int i)
{
    // let's label it -1, if -1 is found in recursion, there is a cycle
    m_color.at(i) = -1;
    std::vector<std::vector<int>> adjacent_matrix = Matrix::getInstance().getAdjacentMatrix();
    for (int j = 0; j < ModuleData::m_moduleList.size(); j++) {
        if (adjacent_matrix.at(i).at(j) != 0) {
            if (m_color.at(j) == -1) { // explore back to the edge, there is a cycle.
                std::vector<int> cycle_index;
                // record the cycle infomation.
                cycle_index.push_back(i);
                cycle_index.push_back(j);
                m_cycle.push_back(cycle_index);
            } else if (m_color.at(j) == 0) {
                dfs(j);// continuing recursion
            }
        }
    }
    m_color.at(i) = 1; // indicates that all descendant nodes of i have been visited.
}

// deep first search.
void Simulation::startDfs()
{
    int i;
    for (i = 0; i < ModuleData::m_moduleList.size(); i++) {
        // if this vertex has not been visited, depth-first traversal is performed from vertex i.
        if (m_color.at(i) == 0) {
            dfs(i);
        }
    }
}

void Simulation::startSimulation()
{
    // search and record the cycle.
    startDfs();
    long long stepCount = 0;
    // calculate the number of steps to be simulated,
    long long stepNum = ModuleData::m_step.final_time / ModuleData::m_step.step_size;
    m_outFile << "stepsize      ";
    for (auto it : ModuleData::m_dispModule) {
        m_outFile << it.first << "                               ";
    }
    m_outFile << std::endl;
    while (stepCount - stepNum <= 0) {
        m_outFile << stepCount << "           ";
        for (auto it : ModuleData::m_dispModule) {
            for (auto input : it.second.input) {
                double result= calculateSimulationResult(input.first, stepCount * ModuleData::m_step.step_size, false);
                // keep 19 decimal places. add the result to file.
                m_outFile << std::fixed << std::setprecision(19) << result << "      ";
                std::cout << "********************************" << std::endl;
                std::cout << "*********    " << it.first << "    **********" << std::endl;
                std::cout << "********************************" << std::endl;
                std::cout << "**** " << std::fixed << std::setprecision(19) 
                          << result << " *****" << std::endl;
                std::cout << "********************************\n\n" << std::endl;
            }
            
        }
        m_outFile << std::endl;
        stepCount++;
    }
}

void Simulation::show()
{
    const std::vector<std::vector<int>> adjacent_matrix = Matrix::getInstance().getAdjacentMatrix();
    std::cout << "***************         step_size: " << ModuleData::m_step.step_size << "     *************"
              << std::endl;
    std::cout << "***************         final_time: " << ModuleData::m_step.final_time << "     *************"
              << std::endl;

    std::cout << "\033[36m***********************module list***********************\033[0m" << std::endl;
    int maxLen = 0;
    for (int i = 0; i < ModuleData::m_moduleList.size(); i++) {
        if (ModuleData::m_moduleList[i].length() > maxLen) {
            maxLen = ModuleData::m_moduleList[i].length();
        }
        std::cout << "\033[36m*****                \033[0m" << ModuleData::m_moduleList[i] << std::endl;
    }
    std::cout << "\033[36m***********************module list***********************\033[0m" << std::endl << std::endl;

    std::cout << "\033[34m***********************module MyClass::relationship***********************\033[0m"
              << std::endl;
    for (int i = 0; i < ModuleData::m_relationship.size(); i++) {
        std::cout << "\033[34m*****                \033[0m" << ModuleData::m_relationship.at(i).at(0) << ", "
                  << ModuleData::m_relationship.at(i).at(1) << std::endl;
    }
    std::cout << "\033[34m***********************module MyClass::relationship***********************\033[0m"
              << std::endl
              << std::endl;

    std::cout << "\033[35m***********************module parameter***********************\033[0m" << std::endl;
    for (auto it : ModuleData::m_gainModule) {
        std::cout << "\033[35m*****                \033[0m" << it.first << ": " << it.second.gain_value << std::endl;
    }

    for (auto it : ModuleData::m_consModule) {
        std::cout << "\033[35m*****                \033[0m" << it.first << ": " << it.second.cons_value << std::endl;
    }

    for (auto it : ModuleData::m_sineModule) {
        std::cout << "\033[35m*****                \033[0m" << it.first << ": " << it.second.amplitude << std::endl;
    }
    std::cout << "\033[35m***********************module parameter***********************\033[0m" << std::endl
              << std::endl;

    std::cout << "\033[32m***********************adjacentMatrix***********************\033[0m" << std::endl;
    std::cout << "\033[32m*****       \033[0m";
    for (int i = 0; i < maxLen + 2; i++) {
        std::cout << " ";
    }

    for (int k = 0; k < ModuleData::m_moduleList.size(); k++) {
        if (ModuleData::m_moduleList[k].length() > maxLen) {
            maxLen = ModuleData::m_moduleList[k].length();
        }
        std::cout << ModuleData::m_moduleList[k] << "  ";
    }
    std::cout << "\033[32m   *****\033[0m" << std::endl;
    ;

    for (int i = 0; i < ModuleData::m_moduleList.size(); i++) {
        std::cout << "\033[32m*****       \033[0m" << ModuleData::m_moduleList[i];
        for (int x = 0; x < (9 - ModuleData::m_moduleList[i].length()); x++) {
            std::cout << " ";
        }
        for (int j = 0; j < ModuleData::m_moduleList.size(); j++) {
            std::cout << adjacent_matrix.at(i).at(j);
            for (int k = 0; k < 5; k++) {
                std::cout << " ";
            }
        }

        std::cout << "\033[32m    *****\033[0m" << std::endl;
    }
    std::cout << "\033[32m***********************adjacentMatrix***********************\033[0m" << std::endl
              << std::endl;
    for (int i = 0; i < m_cycle.size(); i++) {
        std::cout << "\033[33m环在"
                  << "from: [" << ModuleData::m_moduleList[m_cycle.at(i).at(0)] << "]  to: ["
                  << ModuleData::m_moduleList[m_cycle.at(i).at(1)] << "]\033[0m" << std::endl;
    }
}
