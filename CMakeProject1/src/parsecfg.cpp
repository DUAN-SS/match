#include "parsecfg.h"
#include "modeldata.h"
#include <fstream>


const std::string g_model_data_json_file = "../example3.json";

ParseCfg::ParseCfg() { }

ParseCfg::~ParseCfg() { }

void ParseCfg::recordModuleNumber(const std::string& name)
{
    ModuleData::m_moduleList.push_back(name);
}

void ParseCfg::recordRelationship(const std::string& from, const std::string& to)
{
    std::vector<std::string> a;
    a.push_back(from);
    a.push_back(to);
    ModuleData::m_relationship.push_back(a);
}

void ParseCfg::parseGainModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("gain") && doc["gain"].IsArray()) {
        auto gain_config_arrays = doc["gain"].GetArray();
        for (unsigned int i = 0; i < gain_config_arrays.Size(); i++) {
            std::string name;
            Gain gainInfo;
            // parse name of gain module.
            if (gain_config_arrays[i].HasMember("name") && gain_config_arrays[i]["name"].IsString()) {
                name = gain_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }
            // parse value of gain module.
            if (gain_config_arrays[i].HasMember("value") && gain_config_arrays[i]["value"].IsDouble()) {
                gainInfo.gain_value = gain_config_arrays[i]["value"].GetDouble();
            }
            // parse input of gain module.
            if (gain_config_arrays[i].HasMember("input") && gain_config_arrays[i]["input"].IsArray()) {
                auto inputArray = gain_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < inputArray.Size(); j++) {
                    gainInfo.info.input[j] = inputArray[j].GetString();
                }
            }
            // parse output of gain module.
            if (gain_config_arrays[i].HasMember("output") && gain_config_arrays[i]["output"].IsArray()) {
                auto outputArray = gain_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    gainInfo.info.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            // parse init value of gain module.
            if (gain_config_arrays[i].HasMember("initvalue") && gain_config_arrays[i]["initvalue"].IsDouble()) {
                gainInfo.output_value = gain_config_arrays[i]["initvalue"].GetDouble();
            }

            ModuleData::m_gainModule[name] = gainInfo;
        }
    }
}

void ParseCfg::parseSumModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("sum") && doc["sum"].IsArray()) {
        auto sum_config_arrays = doc["sum"].GetArray();
        for (unsigned int i = 0; i < sum_config_arrays.Size(); i++) {
            std::string name;
            ModuleInfo sumInfo;
            // parse name of sum module.
            if (sum_config_arrays[i].HasMember("name") && sum_config_arrays[i]["name"].IsString()) {
                name = sum_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }
            // parse input of sum module.
            if (sum_config_arrays[i].HasMember("input") && sum_config_arrays[i]["input"].IsArray()) {
                auto inputArray = sum_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < inputArray.Size(); j++) {
                    sumInfo.input[j] = inputArray[j].GetString();
                }
            }
            // parse output of sum module.
            if (sum_config_arrays[i].HasMember("output") && sum_config_arrays[i]["output"].IsArray()) {
                auto outputArray = sum_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    sumInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            // parse init value of sum module.
            if (sum_config_arrays[i].HasMember("initvalue") && sum_config_arrays[i]["initvalue"].IsDouble()) {
                sumInfo.output_value = sum_config_arrays[i]["initvalue"].GetDouble();
            }

            ModuleData::m_sumModule[name] = sumInfo;
        }
    }
}

void ParseCfg::parseMultModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("mult") && doc["mult"].IsArray()) {
        auto mult_config_arrays = doc["mult"].GetArray();
        for (unsigned int i = 0; i < mult_config_arrays.Size(); i++) {
            std::string name;
            ModuleInfo multInfo;
            // parse name of mult module.
            if (mult_config_arrays[i].HasMember("name") && mult_config_arrays[i]["name"].IsString()) {
                name = mult_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }
            // parse input of mult module.
            if (mult_config_arrays[i].HasMember("input") && mult_config_arrays[i]["input"].IsArray()) {
                auto inputArray = mult_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < inputArray.Size(); j++) {
                    multInfo.input[j] = inputArray[j].GetString();
                }
            }
            // parse output of mult module.
            if (mult_config_arrays[i].HasMember("output") && mult_config_arrays[i]["output"].IsArray()) {
                auto outputArray = mult_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    multInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            // parse init value of mult module.
            if (mult_config_arrays[i].HasMember("initvalue") && mult_config_arrays[i]["initvalue"].IsDouble()) {
                multInfo.output_value = mult_config_arrays[i]["initvalue"].GetDouble();
            }

            ModuleData::m_multModule[name] = multInfo;
        }
    }
}

void ParseCfg::parseConsModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("cons") && doc["cons"].IsArray()) {
        auto cons_config_arrays = doc["cons"].GetArray();
        for (unsigned int i = 0; i < cons_config_arrays.Size(); i++) {
            std::string name;
            Cons consInfo;
            // parse name of cons module.
            if (cons_config_arrays[i].HasMember("name") && cons_config_arrays[i]["name"].IsString()) {
                name = cons_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }
            // parse value of cons module.
            if (cons_config_arrays[i].HasMember("value") && cons_config_arrays[i]["value"].IsDouble()) {
                consInfo.cons_value = cons_config_arrays[i]["value"].GetDouble();
            }
            // parse output of cons module.
            if (cons_config_arrays[i].HasMember("output") && cons_config_arrays[i]["output"].IsArray()) {
                auto outputArray = cons_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    consInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            ModuleData::m_consModule[name] = consInfo;
        }
    }
}

void ParseCfg::parseSineModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("sine") && doc["sine"].IsArray()) {
        auto sine_config_arrays = doc["sine"].GetArray();
        for (unsigned int i = 0; i < sine_config_arrays.Size(); i++) {
            std::string name;
            Sine sineInfo;
            // parse name of sine module.
            if (sine_config_arrays[i].HasMember("name") && sine_config_arrays[i]["name"].IsString()) {
                name = sine_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }
            // parse value of sine module.
            if (sine_config_arrays[i].HasMember("value") && sine_config_arrays[i]["value"].IsDouble()) {
                sineInfo.amplitude = sine_config_arrays[i]["value"].GetDouble();
            }
            // parse output of sine module.
            if (sine_config_arrays[i].HasMember("output") && sine_config_arrays[i]["output"].IsArray()) {
                auto outputArray = sine_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    sineInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            ModuleData::m_sineModule[name] = sineInfo;
        }
    }
}

void ParseCfg::parseDspModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("disp") && doc["disp"].IsArray()) {
        auto disp_config_arrays = doc["disp"].GetArray();
        for (unsigned int i = 0; i < disp_config_arrays.Size(); i++) {
            std::string name;
            Disp dispInfo;
            // parse name of disp module.
            if (disp_config_arrays[i].HasMember("name") && disp_config_arrays[i]["name"].IsString()) {
                name = disp_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }
            // parse input of disp module.
            if (disp_config_arrays[i].HasMember("input") && disp_config_arrays[i]["input"].IsArray()) {
                auto outputArray = disp_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    dispInfo.input[outputArray[j].GetString()] = 0;
                }
            }
            ModuleData::m_dispModule[name] = dispInfo;
        }
    }
}

void ParseCfg::parseStepSize(const rapidjson::Document& doc)
{
    if (doc.HasMember("step") && doc["step"].IsArray()) {
        auto step_config_arrays = doc["step"].GetArray();
        for (unsigned int i = 0; i < step_config_arrays.Size(); i++) {
            // parse the final time of simulation.
            if (step_config_arrays[i].HasMember("finaltime") && step_config_arrays[i]["finaltime"].IsDouble()) {
                ModuleData::m_step.final_time = step_config_arrays[i]["finaltime"].GetDouble();
            }
            // parse the step size of simulation.
            if (step_config_arrays[i].HasMember("stepsize") && step_config_arrays[i]["stepsize"].IsDouble()) {
                ModuleData::m_step.step_size = step_config_arrays[i]["stepsize"].GetDouble();
            }
        }
    }
}

bool ParseCfg::parseJsonConf()
{
    std::ifstream ifs(g_model_data_json_file.c_str());
    // open config file
    if (!ifs.is_open()) {
        std::cout << "open error" << std::endl;
        return false;
    }
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    if (doc.HasParseError()) {
        std::cout << "parse error: " << doc.GetParseError() << std::endl;
        return false;
    }
    // parse the config file.
    parseSineModule(doc);
    parseConsModule(doc);
    parseSumModule(doc);
    parseGainModule(doc);
    parseMultModule(doc);
    parseDspModule(doc);
    parseStepSize(doc);
    // close the config file.
    ifs.close();
    // check the module whether is valid
    if (!moduleValidityCheck()) {
        std::cout << "\033[31m: module check failed, please check the modelconfig.json!!!\033[0m" << std::endl;
        return false;
    }

    return true;
}

int ParseCfg::getModulePosition(std::string moduleName)
{
    for (int i = 0; i < ModuleData::m_moduleList.size(); i++) {
        if (ModuleData::m_moduleList[i] == moduleName) {
            return i;
        }
    }
    return -1;
}

bool ParseCfg::ioCheck(const std::string* ioList, const int& listSize)
{
    int ret = true;
    if (ioList == nullptr) {
        return ret;
    }

    for (int i = 0; i < listSize; i++) {
        if (getModulePosition(ioList[i]) == -1) {
            ret = false;
        }
    }
    return ret;
}

bool ParseCfg::checkGain()
{
    const double eps = 1e-6;
    bool ret = true;
    for (auto it : ModuleData::m_gainModule) {
        if (sizeof(it.second.info.input) == 0) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (it.second.info.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (fabs(it.second.gain_value - 0.0) < eps) {
            std::cout << it.first << "\033[31m: the value is invalid!!!\033[0m" << std::endl;
            ret = false;
        }

        if (!ioCheck(it.second.info.input, 1)) {
            ret = false;
            std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
        }

        for (auto out : it.second.info.output) {
            if (getModulePosition(out.first) == -1) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool ParseCfg::checkSum()
{
    bool ret = true;
    for (auto it : ModuleData::m_sumModule) {
        if (sizeof(it.second.input) == 0) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (it.second.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (!ioCheck(it.second.input, size(it.second.input))) {
            ret = false;
            std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
        }

        for (auto out : it.second.output) {
            if (getModulePosition(out.first) == -1) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool ParseCfg::checkMult()
{
    bool ret = true;
    for (auto it : ModuleData::m_multModule) {
        if (sizeof(it.second.input) == 0) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (it.second.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (!ioCheck(it.second.input, size(it.second.input))) {
            ret = false;
            std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
        }

        for (auto out : it.second.output) {
            if (getModulePosition(out.first) == -1) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool ParseCfg::checkSine()
{
    bool ret = true;
    const double eps = 1e-6;

    for (auto it : ModuleData::m_sineModule) {
        if (it.second.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }
        
        if (fabs(it.second.amplitude - 0.0) < eps)
        {
            std::cout << it.first << "\033[31m: the value is invalid!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.output) {
            if (getModulePosition(out.first) == -1) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool ParseCfg::checkCons()
{
    bool ret = true;
    const double eps = 1e-6;
    for (auto it : ModuleData::m_consModule) {
        if (it.second.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (fabs(it.second.cons_value - 0.0) < eps) {
            std::cout << it.first << "\033[31m: the value is invalid!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.output) {
            if (getModulePosition(out.first) == -1) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool ParseCfg::checkDisp()
{
    bool ret = true;
    for (auto it : ModuleData::m_dispModule) {
        if (it.second.input.empty()) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.input) {
            if (getModulePosition(out.first) == -1) {
                ret = false;
                std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool ParseCfg::checkStep()
{
    const double eps = 1e-6;
    if (fabs(ModuleData::m_step.step_size - 0.0) < eps) {
        std::cout << "\033[31m: the step_size is invalid!!!\033[0m" << std::endl;
        return false;
    }
    return true;
}

bool ParseCfg::moduleValidityCheck()
{
    return checkGain() && checkSum() && checkMult() && checkDisp() && checkSine() && checkStep() && checkCons();
}
