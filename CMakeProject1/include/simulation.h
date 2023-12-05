/**@file  : parsejson.h
 *@brief  : descripe the module infomation
 *@author :
 *@date   : 2023.12.04
 *@version: V0.1
 *@note   : note
 *@detailed description:
 */

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <iostream>
#include <map>
#include <string>
#include <fstream>

#include "document.h"
#include "istreamwrapper.h"

#include "modeldata.h"

class MySimulation
{
private:
    Timing m_step;                                  // the infomation about step (step size and final time).
    std::map<std::string, Gain> m_gainModule;       // the gain module infomation.
    std::map<std::string, ModuleInfo> m_sumModule;  // the sum module infomation.
    std::map<std::string, ModuleInfo> m_multModule; // the mult module infomation.
    std::map<std::string, Sine> m_sineModule;       // the sine module infomation.
    std::map<std::string, Cons> m_consModule;       // the cons module infomation.
    std::map<std::string, Disp> m_dispModule;       // the disp module infomation.
    int m_cycleNum;                                 // the cycle number.
    int m_cycle[g_MaxNumber][2];                    // the index of cycle in moduleList.
    int m_color[g_MaxNumber];                       // the color of modules that has three values.
                                                    // 0: the module is not visited.
                                                    // 1: the modules that follow it have already been
                                                    // accessed. -1: the module has visited, is repeat.

    int m_adjacentMatrix[g_MaxNumber][g_MaxNumber]; // Zero-order matrix, indicates the
                                                    // relationship between modules, set the
                                                    // value is 1 if two modules are related.
                                                    // eg: the output of module1 is the input
                                                    // of module2. |       | module1  | module2
                                                    // | |module1|   0      |    1     |
                                                    // |module2|   0      |    0     |

    int m_moduleNumber;                         // the module numbers.
    std::string m_moduleList[g_MaxNumber];      // the modules array.
    int m_relationshipNum;                      // the relationshipe number of modules.
    std::string m_relationship[g_MaxNumber][2]; // the input/output relationship
                                                // of the modules.
    std::ofstream m_outFile;                    // the file that store the simulation result.

    MySimulation();                                           // external construction prohibited.
    ~MySimulation();                                          // prohibit external destruction.
    MySimulation(const MySimulation& my_simulation) = delete; // prohibit external copy constructs.
     // disable external assignment operations.
    const MySimulation& operator=(const MySimulation& my_simulation) = delete; 

    /** 
     * @brief     : parse the module infomation.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : modelconfig.json.
     * @param[out]: module infomation.
     * @return    : return true if parse success, otherwise, return false.
     * @note      : none.
     */
    bool parseJsonConf();

    /** 
     * @brief     : parse the gain module infomation.
     * @author    : yongping.duan@keliangtek
     * @param[in] : const rapidjson::Document& doc
     * @param[out]: moduleNumber, moduleList, relationshipNum, relationship.
     * @return    : none
     * @note      : none
     */
    void parseGainModule(const rapidjson::Document& doc);
    void parseSumModule(const rapidjson::Document& doc);
    void parseMultModule(const rapidjson::Document& doc);
    void parseConsModule(const rapidjson::Document& doc);
    void parseSineModule(const rapidjson::Document& doc);
    void parseDspModule(const rapidjson::Document& doc);

    /** 
     * @brief     : check the validity of the model.
     * @author    : yongping.duan@keliangtek
     * @param[in] : the module infomation.
     * @param[out]: none
     * @return    : return true if valid, otherwise, return false.
     * @note      : none
     */
    bool moduleValidityCheck();
    bool checkGain();
    bool checkSum();
    bool checkMult();
    bool checkDisp();
    bool checkSine();
    bool checkStep();

    /** 
     * @brief     : check the input/output validity of the model.
     * @author    : yongping.duan@keliangtek
     * @param[in] : const std::string* ioList: io list.
     *              const int listSize: the io list size.
     * @param[out]: none
     * @return    : return true if valid, otherwise, return false.
     * @note      : none
     */
    bool ioCheck(const std::string* io_list, const int list_size);

    /** 
     * @brief     : parse the step size and final time.
     * @author    : yongping.duan@keliangtek
     * @param[in] : const rapidjson::Document& doc
     * @param[out]: Timing step
     * @return    : none
     * @note      : none
     */
    void parseStepSize(const rapidjson::Document& doc);

    /** 
     * @brief     : calculate the result of simulation. starting from the output,
     *a recursive algorithm is used to iterate forward  @author    :
     *yongping.duan@keliangtek  @param[in] : std::string lastModel: the input
     *module. double sinValue: the sine value. bool getPreValue: whether to get
     *the value of the previous step.
     * @param[out]: the module infomation.
     * @return    : Simulation results of the current step.
     * @note      : none
     */
    double calculateSimulationResult(std::string last_model, double sin_value, bool get_pre_value);

    /** 
     * @brief     : create the adjacent matrix.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : std::string relationship[MAXNUM][2].
     *              int relationshipNum.
     * @param[out]: int adjacentMatrix[MAXNUM][MAXNUM].
     * @return    : none
     * @note      : none
     */
    void creatAdjacentMatrix();

    /** 
     * @brief     : record the module number.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : const std::string& name: the name of module.
     * @param[out]: int moduleNumber.
     *              std::string moduleList[MAXNUM].
     * @return    : none
     * @note      : none
     */
    void recordModuleNumber(const std::string& name);

    /** 
     * @brief     : record the module relationship.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : const std::string& from: the input of module.
     *              const std::string& to: the output of module.
     * @param[out]: int relationshipNum.
     *              std::string relationship[MAXNUM][2].
     * @return    : none
     * @note      : none
     */
    void recordRelationship(const std::string& from, const std::string& to);

    /** 
     * @brief     : get the module location in the moduleList.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : std::string moduleName: the name of module.
     *              int relationshipNum.
     * @param[out]: none
     * @return    : the module index.
     * @note      : none
     */
    int getModulePosition(std::string module_name);

    /** 
     * @brief     : depth-first algorithm.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : int moduleNumber: the number of module.
     *              int relationshipNum.
     *              int adjacentMatrix[MAXNUM][MAXNUM].
     * @param[out]: int cycleNum.
     *              int cycle[MAXNUM][2].
     *              int color[MAXNUM].
     * @return    : none.
     * @note      : none.
     */
    void startDfs();

    /** 
     * @brief     : search the cycle.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : int moduleNumber: the number of module.
     *              int relationshipNum.
     *              int adjacentMatrix[MAXNUM][MAXNUM].
     * @param[out]: int cycleNum.
     *              int cycle[MAXNUM][2].
     *              int color[MAXNUM].
     * @return    : none.
     * @note      : none.
     */
    void dfs(int i);

public:
    static MySimulation& getInstance()
    {
        static MySimulation my_simulation;
        return my_simulation;
    }

    /* debug ,show the data*/
    void showData();

    /** 
     * @brief     : the entry function, start the simulation..
     * @author    : yongping.duan@keliangtek.
     * @param[in] : the input of module.
     * @param[out]: the output of module.
     * @return    : none.
     * @note      : none.
     */
    void startSimulation();

}; // class Simulation

#endif // _SIMULATION_H_