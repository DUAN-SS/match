#include "simulation.h"
#include <cmath>
#include <iomanip>

const std::string g_model_data_json_file = "../modelconfig.json";

MySimulation::MySimulation()
{
    m_step.final_time = -1;  // the simulation is not stop
    m_step.step_size = 1;    // init the step size is 1.
    m_gainModule.clear();
    m_sumModule.clear();
    m_multModule.clear();
    m_sineModule.clear();
    m_consModule.clear();
    m_dispModule.clear();

    m_cycleNum = 0;         // has not cycle.
    memset(m_cycle, 0, sizeof(m_cycle));

    memset(m_color, 0, sizeof(m_color));

    memset(m_adjacentMatrix, 0, sizeof(m_adjacentMatrix));

    m_moduleNumber = -1;    // has no module.
    m_moduleList->clear();

    m_relationshipNum = 0;  // has no relationship.
    m_relationship[0]->clear();
    m_relationship[1]->clear();

    m_outFile.open("data.csv");
}

MySimulation::~MySimulation()
{
    m_outFile.close();
}

void MySimulation::dfs(int i)
{
    m_color[i] = -1;
    for (int j = 0; j <= m_moduleNumber; j++) {
        if (m_adjacentMatrix[i][j] != 0) {
            if (m_color[j] == -1) { // 探索到回边,存在环
                m_cycle[m_cycleNum][0] = i;
                m_cycle[m_cycleNum][1] = j;
                m_cycleNum++;
            } else if (m_color[j] == 0) {
                dfs(j);
            }
        }
    }
    m_color[i] = 1; // 表示i的后裔节点都被访问过
}

// 深度优先搜索
void MySimulation::startDfs()
{
    int i;
    for (i = 0; i <= m_moduleNumber; i++) {
        // 如果这个顶点未被访问过，则从i顶点出发进行深度优先遍历
        if (m_color[i] == 0) {
            dfs(i);
        }
    }
}

void MySimulation::recordModuleNumber(const std::string& name)
{
    m_moduleNumber++;
    m_moduleList[m_moduleNumber] = name;
}

void MySimulation::recordRelationship(const std::string& from, const std::string& to)
{
    m_relationship[m_relationshipNum][0] = from;
    m_relationship[m_relationshipNum][1] = to;
    m_relationshipNum++;
}

void MySimulation::parseGainModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("gain") && doc["gain"].IsArray()) {
        auto gain_config_arrays = doc["gain"].GetArray();
        for (unsigned int i = 0; i < gain_config_arrays.Size(); i++) {
            std::string name;
            Gain gainInfo;
            if (gain_config_arrays[i].HasMember("name") && gain_config_arrays[i]["name"].IsString()) {
                name = gain_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }

            if (gain_config_arrays[i].HasMember("value") && gain_config_arrays[i]["value"].IsDouble()) {
                gainInfo.gain_value = gain_config_arrays[i]["value"].GetDouble();
            }

            if (gain_config_arrays[i].HasMember("input") && gain_config_arrays[i]["input"].IsArray()) {
                auto inputArray = gain_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < inputArray.Size(); j++) {
                    gainInfo.info.input[j] = inputArray[j].GetString();
                }
            }

            if (gain_config_arrays[i].HasMember("output") && gain_config_arrays[i]["output"].IsArray()) {
                auto outputArray = gain_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    gainInfo.info.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            m_gainModule[name] = gainInfo;
        }
    }
}

void MySimulation::parseSumModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("sum") && doc["sum"].IsArray()) {
        auto sum_config_arrays = doc["sum"].GetArray();
        for (unsigned int i = 0; i < sum_config_arrays.Size(); i++) {
            std::string name;
            ModuleInfo sumInfo;
            if (sum_config_arrays[i].HasMember("name") && sum_config_arrays[i]["name"].IsString()) {
                name = sum_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }

            if (sum_config_arrays[i].HasMember("input") && sum_config_arrays[i]["input"].IsArray()) {
                auto inputArray = sum_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < inputArray.Size(); j++) {
                    sumInfo.input[j] = inputArray[j].GetString();
                }
            }

            if (sum_config_arrays[i].HasMember("output") && sum_config_arrays[i]["output"].IsArray()) {
                auto outputArray = sum_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    sumInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            m_sumModule[name] = sumInfo;
        }
    }
}

void MySimulation::parseMultModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("mult") && doc["mult"].IsArray()) {
        auto mult_config_arrays = doc["mult"].GetArray();
        for (unsigned int i = 0; i < mult_config_arrays.Size(); i++) {
            std::string name;
            ModuleInfo multInfo;
            if (mult_config_arrays[i].HasMember("name") && mult_config_arrays[i]["name"].IsString()) {
                name = mult_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }

            if (mult_config_arrays[i].HasMember("input") && mult_config_arrays[i]["input"].IsArray()) {
                auto inputArray = mult_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < inputArray.Size(); j++) {
                    multInfo.input[j] = inputArray[j].GetString();
                }
            }

            if (mult_config_arrays[i].HasMember("output") && mult_config_arrays[i]["output"].IsArray()) {
                auto outputArray = mult_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    multInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            m_multModule[name] = multInfo;
        }
    }
}

void MySimulation::parseConsModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("cons") && doc["cons"].IsArray()) {
        auto cons_config_arrays = doc["cons"].GetArray();
        for (unsigned int i = 0; i < cons_config_arrays.Size(); i++) {
            std::string name;
            Cons consInfo;
            if (cons_config_arrays[i].HasMember("name") && cons_config_arrays[i]["name"].IsString()) {
                name = cons_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }

            if (cons_config_arrays[i].HasMember("value") && cons_config_arrays[i]["value"].IsDouble()) {
                consInfo.cons_value = cons_config_arrays[i]["value"].GetDouble();
            }

            if (cons_config_arrays[i].HasMember("output") && cons_config_arrays[i]["output"].IsArray()) {
                auto outputArray = cons_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    consInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            m_consModule[name] = consInfo;
        }
    }
}

void MySimulation::parseSineModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("sine") && doc["sine"].IsArray()) {
        auto sine_config_arrays = doc["sine"].GetArray();
        for (unsigned int i = 0; i < sine_config_arrays.Size(); i++) {
            std::string name;
            Sine sineInfo;
            if (sine_config_arrays[i].HasMember("name") && sine_config_arrays[i]["name"].IsString()) {
                name = sine_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }

            if (sine_config_arrays[i].HasMember("value") && sine_config_arrays[i]["value"].IsDouble()) {
                sineInfo.sine_value = sine_config_arrays[i]["value"].GetDouble();
            }

            if (sine_config_arrays[i].HasMember("output") && sine_config_arrays[i]["output"].IsArray()) {
                auto outputArray = sine_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    sineInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            m_sineModule[name] = sineInfo;
        }
    }
}

void MySimulation::parseDspModule(const rapidjson::Document& doc)
{
    if (doc.HasMember("disp") && doc["disp"].IsArray()) {
        auto disp_config_arrays = doc["disp"].GetArray();
        for (unsigned int i = 0; i < disp_config_arrays.Size(); i++) {
            std::string name;
            Disp dispInfo;
            if (disp_config_arrays[i].HasMember("name") && disp_config_arrays[i]["name"].IsString()) {
                name = disp_config_arrays[i]["name"].GetString();
                recordModuleNumber(name);
            }

            if (disp_config_arrays[i].HasMember("input") && disp_config_arrays[i]["input"].IsArray()) {
                auto outputArray = disp_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    dispInfo.input[outputArray[j].GetString()] = 0;
                }
            }
            m_dispModule[name] = dispInfo;
        }
    }
}

void MySimulation::parseStepSize(const rapidjson::Document& doc)
{
    if (doc.HasMember("step") && doc["step"].IsArray()) {
        auto step_config_arrays = doc["step"].GetArray();
        for (unsigned int i = 0; i < step_config_arrays.Size(); i++) {
            if (step_config_arrays[i].HasMember("finaltime") && step_config_arrays[i]["finaltime"].IsInt()) {
                m_step.final_time = step_config_arrays[i]["finaltime"].GetInt();
            }

            if (step_config_arrays[i].HasMember("stepsize") && step_config_arrays[i]["stepsize"].IsDouble()) {
                m_step.step_size = step_config_arrays[i]["stepsize"].GetDouble();
            }
        }
    }
}

bool MySimulation::parseJsonConf()
{
    std::ifstream ifs(g_model_data_json_file.c_str());
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

    parseSineModule(doc);
    parseConsModule(doc);
    parseSumModule(doc);
    parseGainModule(doc);
    parseMultModule(doc);
    parseDspModule(doc);
    parseStepSize(doc);

    ifs.close();
    if (!moduleValidityCheck()) {
        std::cout << "\033[31m: module check failed, please check the modelconfig.json!!!\033[0m" << std::endl;
        return false;
    }
    creatAdjacentMatrix();
    return true;
}

int MySimulation::getModulePosition(std::string moduleName)
{
    for (int i = 0; i <= m_moduleNumber; i++) {
        if (m_moduleList[i] == moduleName) {
            return i;
        }
    }
    return g_MaxNumber;
}

void MySimulation::creatAdjacentMatrix()
{
    for (int i = 0; i < m_relationshipNum; i++) {
        int from = getModulePosition(m_relationship[i][0]);
        int to = getModulePosition(m_relationship[i][1]);
        if ((from != g_MaxNumber) && (to != g_MaxNumber)) {
            m_adjacentMatrix[from][to] = 1;
        }
    }
}

double MySimulation::calculateSimulationResult(std::string lastModel, double sinValue, bool getPreValue)
{
    if (lastModel.find("cons") != std::string::npos) { // the cons is not be cycle. return the value.
        m_consModule[lastModel].output_value = m_consModule[lastModel].cons_value;
        return m_consModule[lastModel].output_value;
    }

    if (lastModel.find("sum") != std::string::npos) {
        if (getPreValue) { // if it is the star of cycle.
            return m_sumModule[lastModel].output_value;
        }
        int j = 0;
        for (j; j < 2; j++) {  // search the input of sum.
            bool isCycle = false;

            for (int i = 0; i < m_cycleNum; i++) {
                if (m_moduleList[m_cycle[i][1]] == lastModel
                    && m_sumModule[lastModel].input[j] == m_moduleList[m_cycle[i][0]]) {
                    isCycle = true;  // it is cycle.
                    break;
                }
            }
            if (isCycle) { // has cycle, break.
                break;
            }
        }

        if (j == 2) { // the sum is not the cycle.
            m_sumModule[lastModel].output_value =
                    calculateSimulationResult(m_sumModule[lastModel].input[0], sinValue, false)
                    + calculateSimulationResult(m_sumModule[lastModel].input[1], sinValue, false);
        } else if (j == 0) { // the sum cycle with input 0
            m_sumModule[lastModel].output_value =
                    calculateSimulationResult(m_sumModule[lastModel].input[0], sinValue, true)
                    + calculateSimulationResult(m_sumModule[lastModel].input[1], sinValue, false);
        } else { // the sum cycle with input 1
            m_sumModule[lastModel].output_value =
                    calculateSimulationResult(m_sumModule[lastModel].input[0], sinValue, false)
                    + calculateSimulationResult(m_sumModule[lastModel].input[1], sinValue, true);
        }
        return m_sumModule[lastModel].output_value;
    }

    if (lastModel.find("gain") != std::string::npos) {
        if (getPreValue) { // if the gain is the start of cycle.
            return m_gainModule[lastModel].output_value;
        }
        // the gain output = the input * gain value;
        m_gainModule[lastModel].output_value = m_gainModule[lastModel].gain_value
                * calculateSimulationResult(m_gainModule[lastModel].info.input[0], sinValue, false);
        return m_gainModule[lastModel].output_value;
    }

    if (lastModel.find("sine") != std::string::npos) {
        // using the rad. if need use.
        // const double pi = atan(1.0) * 4;
        // sineModel[lastModel].outputValue = sineModel[lastModel].sineValue * sin(sinValue * pi / 180);
        // the sine value is the count of step * simulation step.
        m_sineModule[lastModel].output_value = m_sineModule[lastModel].sine_value * sin(sinValue);
        return m_sineModule[lastModel].output_value;
    }

    if (lastModel.find("mult") != std::string::npos) {
        if (getPreValue) {
            return m_multModule[lastModel].output_value;
        }

        int j = 0;

        for (j; j < 2; j++) {
            bool isCycle = false;
            for (int i = 0; i < m_cycleNum; i++) {
                if (m_moduleList[m_cycle[i][1]] == lastModel
                    && m_multModule[lastModel].input[j] == m_moduleList[m_cycle[i][0]]) {
                    isCycle = true;
                    break;
                }
            }
            if (isCycle) {
                break;
            }
        }
        if (j == 2) {
            m_multModule[lastModel].output_value =
                    calculateSimulationResult(m_multModule[lastModel].input[0], sinValue, false)
                    * calculateSimulationResult(m_multModule[lastModel].input[1], sinValue, false);
        } else if (j == 0) {
            m_multModule[lastModel].output_value =
                    calculateSimulationResult(m_multModule[lastModel].input[0], sinValue, true)
                    * calculateSimulationResult(m_multModule[lastModel].input[1], sinValue, false);
        } else {
            m_multModule[lastModel].output_value =
                    calculateSimulationResult(m_multModule[lastModel].input[0], sinValue, false)
                    * calculateSimulationResult(m_multModule[lastModel].input[1], sinValue, true);
        }
        return m_multModule[lastModel].output_value;
    }
}

void MySimulation::startSimulation()
{
    if (!parseJsonConf()) {
        return;
    }

    startDfs();
    long long stepCount = 0;
    long long stepNum = m_step.final_time / m_step.step_size;
    while (stepCount - stepNum <= 0) {
        for (auto it : m_dispModule) {
            for (auto input : it.second.input) {
                m_outFile << std::fixed << std::setprecision(19) <<calculateSimulationResult(input.first, stepCount * m_step.step_size, false) << std::endl;
            }
        }
        stepCount++;
    }
}

bool MySimulation::ioCheck(const std::string* ioList, const int listSize)
{
    int ret = true;
    if (ioList == nullptr) {
        return ret;
    }

    for (int i = 0; i < listSize; i++) {
        if (getModulePosition(ioList[i]) > m_moduleNumber) {
            ret = false;
        }
    }
    return ret;
}

bool MySimulation::checkGain()
{
    bool ret = true;
    for (auto it : m_gainModule) {
        if (sizeof(it.second.info.input) == 0) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (it.second.info.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (!ioCheck(it.second.info.input, 1)) {
            ret = false;
            std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
        }

        for (auto out : it.second.info.output) {
            if (getModulePosition(out.first) > m_moduleNumber) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool MySimulation::checkSum()
{
    bool ret = true;
    for (auto it : m_sumModule) {
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
            if (getModulePosition(out.first) > m_moduleNumber) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool MySimulation::checkMult()
{
    bool ret = true;
    for (auto it : m_multModule) {
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
            if (getModulePosition(out.first) > m_moduleNumber) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool MySimulation::checkSine()
{
    bool ret = true;
    for (auto it : m_sineModule) {
        if (it.second.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.output) {
            if (getModulePosition(out.first) > m_moduleNumber) {
                ret = false;
                std::cout << it.first << "\033[31m: the out error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool MySimulation::checkDisp()
{
    bool ret = true;
    for (auto it : m_dispModule) {
        if (it.second.input.empty()) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.input) {
            if (getModulePosition(out.first) > m_moduleNumber) {
                ret = false;
                std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}

bool MySimulation::checkStep() 
{
    const double eps = 1e-6;
    if (fabs(m_step.step_size - 0.0) < eps) {
        std::cout << "\033[31m: the step_size is invalid!!!\033[0m" << std::endl;
        return false;
    }
    return true;
}

bool MySimulation::moduleValidityCheck()
{
    return checkGain() && checkSum() && checkMult() && checkDisp() && checkSine() && checkStep();
}

void MySimulation::showData()
{
    std::cout << "***************         step: " << m_step.step_size << "     *************" << std::endl;
    std::cout << "***************         step: " << m_step.final_time << "     *************" << std::endl;

    std::cout << "\033[36m***********************module list***********************\033[0m" << std::endl;
    int maxLen = 0;
    for (int i = 0; i <= m_moduleNumber; i++) {
        if (m_moduleList[i].length() > maxLen) {
            maxLen = m_moduleList[i].length();
        }
        std::cout << "\033[36m*****                \033[0m" << m_moduleList[i] << std::endl;
    }
    std::cout << "\033[36m***********************module list***********************\033[0m" << std::endl << std::endl;

    std::cout << "\033[34m***********************module relationship***********************\033[0m" << std::endl;
    for (int i = 0; i < m_relationshipNum; i++) {
        std::cout << "\033[34m*****                \033[0m" << m_relationship[i][0] << ", " << m_relationship[i][1]
                  << std::endl;
    }
    std::cout << "\033[34m***********************module relationship***********************\033[0m" << std::endl
              << std::endl;

    std::cout << "\033[35m***********************module parameter***********************\033[0m" << std::endl;
    for (auto it : m_gainModule) {
        std::cout << "\033[35m*****                \033[0m" << it.first << ": " << it.second.gain_value << std::endl;
    }

    for (auto it : m_consModule) {
        std::cout << "\033[35m*****                \033[0m" << it.first << ": " << it.second.cons_value << std::endl;
    }

    for (auto it : m_sineModule) {
        std::cout << "\033[35m*****                \033[0m" << it.first << ": " << it.second.sine_value << std::endl;
    }
    std::cout << "\033[35m***********************module parameter***********************\033[0m" << std::endl
              << std::endl;

    std::cout << "\033[32m***********************adjacentMatrix***********************\033[0m" << std::endl;
    std::cout << "\033[32m*****       \033[0m";
    for (int i = 0; i < maxLen + 2; i++) {
        std::cout << " ";
    }

    for (int k = 0; k <= m_moduleNumber; k++) {
        if (m_moduleList[k].length() > maxLen) {
            maxLen = m_moduleList[k].length();
        }
        std::cout << m_moduleList[k] << "  ";
    }
    std::cout << "\033[32m   *****\033[0m" << std::endl;
    ;

    for (int i = 0; i <= m_moduleNumber; i++) {
        std::cout << "\033[32m*****       \033[0m" << m_moduleList[i];
        for (int x = 0; x < (9 - m_moduleList[i].length()); x++) {
            std::cout << " ";
        }
        for (int j = 0; j <= m_moduleNumber; j++) {
            std::cout << m_adjacentMatrix[i][j];
            for (int k = 0; k < 5; k++) {
                std::cout << " ";
            }
        }

        std::cout << "\033[32m    *****\033[0m" << std::endl;
    }
    std::cout << "\033[32m***********************adjacentMatrix***********************\033[0m" << std::endl
              << std::endl;
    for (int i = 0; i < m_cycleNum; i++) {
        std::cout << "\033[33m环在"
                  << "from: [" << m_moduleList[m_cycle[i][0]] << "]  to: [" << m_moduleList[m_cycle[i][1]] << "]\033[0m"
                  << std::endl;
    }
}
