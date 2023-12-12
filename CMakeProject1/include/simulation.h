/**@file  : simulation.h
 *@brief  : The model is simulated
 *@author : yongping.duan@keliangtek.com
 *@date   : 2023.12.04
 *@version: V0.1
 *@note   : note
 *@detailed description:
 */

#ifndef __SIMULIATION_H__
#define __SIMULIATION_H__

#include <fstream>
#include <iostream>
#include <vector>


class Simulation
{
public:
    static Simulation& getInstance()
    {
        static Simulation simulation;
        return simulation;
    }
    /** 
     * @brief     : the entry function, start the simulation.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : the input of module.
     * @param[out]: the output of module.
     * @return    : none.
     * @note      : none.
     */
    void startSimulation();

    /**
     * debug display.
     */
    void show();

private:
    std::ofstream m_outFile;               // the file that store the simulation result.
    std::vector<std::vector<int>> m_cycle; // the index of cycle in moduleList.
    std::vector<int> m_color;              // the color of modules that has three values.
                                           // 0: the module is not visited.
                                           // 1: the modules that follow it have already been accessed. 
                                           // -1: the module has visited, is repeat.

    /** 
     * @brief     : calculate the result of simulation. starting from the output,
     *              a recursive algorithm is used to iterate forward  
     * @author    : yongping.duan@keliangtek  
     * @param[in] : std::string lastModel: the input module.
     *              double sinValue: the sine value.
     *              bool getPreValue: whether to get the value of the previous step.
     * @param[out]: the module infomation.
     * @return    : Simulation results of the current step.
     * @note      : none
     */
    double calculateSimulationResult(const std::string& last_model, 
                                     const double& sin_value,
                                     const bool& get_pre_value);

    /** 
     * @brief     : depth-first algorithm.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : static std::vector<std::string> m_moduleList.
     * @param[out]: std::vector<std::vector<int>> m_cycle.
     *              std::vector<int> m_color.
     * @return    : none.
     * @note      : none.
     */
    void startDfs();

    /** 
     * @brief     : search the cycle.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : static std::vector<std::string> m_moduleList.
     *              std::vector<std::vector<int>> m_adjacentMatrix.
     * @param[out]: std::vector<std::vector<int>> m_cycle.
     *              std::vector<int> m_color.
     * @return    : none.
     * @note      : none.
     */
    void dfs(int i);

    Simulation();
    ~Simulation();
    Simulation(const Simulation& simulation) = delete; // prohibit external copy constructs.
    // disable external assignment operations.
    const Simulation& operator=(const Simulation& simulation) = delete;
}; // class Simulation

#endif // __SIMULIATION_H__
