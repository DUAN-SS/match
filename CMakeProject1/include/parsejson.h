#ifndef _PARSEJSON_H_
#define _PARSEJSON_H_

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "modeldata.h"
#include "document.h"
#include "istreamwrapper.h"

class MySimulation
{
private:
    Timing step;                                   // the infomation about step (step size and final time).
    std::map<std::string, Gain> gainModule;        // the gain module infomation.
    std::map<std::string, ModuleInfo> sumModule;   // the sum module infomation.
    std::map<std::string, ModuleInfo> multModule;  // the mult module infomation.
    std::map<std::string, Sine> sineModule;        // the sine module infomation.
    std::map<std::string, Cons> consModule;        // the cons module infomation.
    std::map<std::string, Disp> dispModule;        // the disp module infomation.
    int cycleNum;                        // the cycle number.
    int cycle[MAXNUM][2];                // the index of cycle in moduleList.
    int color[MAXNUM];                   // the color of modules that has three values.
                                         // 0: the module is not visited.
                                         // 1: the modules that follow it have already been accessed.
                                         // -1: the module has visited, is repeat.

    int adjacentMatrix[MAXNUM][MAXNUM];  // Zero-order matrix, indicates the relationship between modules, 
                                         // set the value is 1 if two modules are related.
                                         // eg: the output of module1 is the input of module2.
                                         // |       | module1  |  module2 |
                                         // |module1|   0      |    1     |
                                         // |module2|   0      |    0     |

    int moduleNumber;                         // the module numbers.
    std::string moduleList[MAXNUM];      // the modules array.
    int relationshipNum;                 // the relationshipe number of modules.
    std::string relationship[MAXNUM][2]; // the input/output relationship of the modules.
    std::ofstream outFile;               // the file that store the simulation result.

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



    bool checkGain();
    bool checkSum();
    bool checkMult();
    bool checkDisp();
    bool checkSine();

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
     * @brief     : calculate the result of simulation. starting from the output, a recursive algorithm is used to iterate forward
     * @author    : yongping.duan@keliangtek
     * @param[in] : std::string lastModel: the input module.
     *              double sinValue: the sine value.
     *              bool getPreValue: whether to get the value of the previous step.
     * @param[out]: the module infomation.
     * @return    : Simulation results of the current step.
     * @note      : none
     */
    double calculateSimulationResult(std::string lastModel, double sinValue, bool getPreValue);


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
    int getModulePosition(std::string moduleName);

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
    void DFS();

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

    bool moduleValidityCheck();
    bool ioCheck(const std::string* ioList, const int listSize);
public:
    MySimulation();
    ~MySimulation();


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

#endif //__PARSECONFIG_H__