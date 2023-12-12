/**@file  : parsejson.h
 *@brief  : descripe the module infomation
 *@author :
 *@date   : 2023.12.04
 *@version: V0.1
 *@note   : note
 *@detailed description:
 */

#ifndef _PARSECFG_H_
#define _PARSECFG_H_

#include "document.h"
#include "istreamwrapper.h"

class ParseCfg
{
private:
    ParseCfg();                                       // external construction prohibited.
    ~ParseCfg();                                      // prohibit external destruction.
    ParseCfg(const ParseCfg& my_simulation) = delete; // prohibit external copy constructs.
    // disable external assignment operations.
    const ParseCfg& operator=(const ParseCfg& my_simulation) = delete;

    /** 
     * @brief     : parse the gain module infomation.
     * @author    : yongping.duan@keliangtek.com
     * @param[in] : const rapidjson::Document& doc
     * @param[out]:
     *            : static Timing m_step.
     *            : static std::map<std::string, Gain> m_gainModule.
     *            : static std::map<std::string, ModuleInfo> m_sumModule.
     *            : static std::map<std::string, ModuleInfo> m_multModule.
     *            : static std::map<std::string, Sine> m_sineModule.
     *            : static std::map<std::string, Cons> m_consModule.
     *            : static std::map<std::string, Disp> m_dispModule.
     *            : static std::vector<std::string> m_moduleList.
     *            : static std::vector<std::vector<std::string>> m_relationship.
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
     * @author    : yongping.duan@keliangtek.com
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
     * @author    : yongping.duan@keliangtek.com
     * @param[in] : const std::string* ioList: io list.
     *              const int listSize: the io list size.
     * @param[out]: none
     * @return    : return true if valid, otherwise, return false.
     * @note      : none
     */
    bool ioCheck(const std::string* io_list, const int& list_size);

    /** 
     * @brief     : parse the step size and final time.
     * @author    : yongping.duan@keliangtek.com
     * @param[in] : const rapidjson::Document& doc
     * @param[out]: Timing step
     * @return    : none
     * @note      : none
     */
    void parseStepSize(const rapidjson::Document& doc);

    /** 
     * @brief     : record the module number.
     * @author    : yongping.duan@keliangtek.com
     * @param[in] : const std::string& name: the name of module.
     * @param[out]: static std::vector<std::string> m_moduleList.
     * @return    : none
     * @note      : none
     */
    void recordModuleNumber(const std::string& name);

    /** 
     * @brief     : record the module MyClass::relationship.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : const std::string& from: the input of module.
     *              const std::string& to: the output of module.
     * @param[out]: static std::vector<std::vector<std::string>> m_relationship.
     * @return    : none
     * @note      : none
     */
    void recordRelationship(const std::string& from, const std::string& to);

    /** 
     * @brief     : get module index in the m_moduleList.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : std::string module_name: the module name.
     * @param[out]: none
     * @return    : return module index in the m_moduleList if success, otherwise,
     *              return -1.
     * @note      : none
     */
    int getModulePosition(std::string module_name);

public:
    static ParseCfg& getInstance()
    {
        static ParseCfg my_simulation;
        return my_simulation;
    }
    /** 
     * @brief     : parse the module infomation.
     * @author    : yongping.duan@keliangtek.
     * @param[in] : modelconfig.json.
     * @param[out]: static Timing m_step.
     *            : static std::map<std::string, Gain> m_gainModule.
     *            : static std::map<std::string, ModuleInfo> m_sumModule.
     *            : static std::map<std::string, ModuleInfo> m_multModule.
     *            : static std::map<std::string, Sine> m_sineModule.
     *            : static std::map<std::string, Cons> m_consModule.
     *            : static std::map<std::string, Disp> m_dispModule.
     *            : static std::vector<std::string> m_moduleList.
     *            : static std::vector<std::vector<std::string>> m_relationship.
     * @return    : return true if parse success, otherwise, return false.
     * @note      : none.
     */
    bool parseJsonConf();

}; // class ParseCfg

#endif // _SIMULATION_H_