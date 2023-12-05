#include "parsejson.h"
#include <cmath>
#include <ctime>

const std::string MODEL_DATA_JSON_FILE_PATH = "../modelconfig.json";

MySimulation::MySimulation()
{
    moduleNumber = -1;
    cycleNum = 0;
    relationshipNum = 0;
    memset(cycle, 0, sizeof(cycle));
    memset(color, 0,sizeof(color));
    memset(adjacentMatrix, 0, sizeof(adjacentMatrix));
    outFile.open("data.csv");
}

MySimulation::~MySimulation()
{
    outFile.close();
}

void MySimulation::dfs( int i)
{
    color[i] = -1;
    for (int j = 0; j <= moduleNumber; j++) {
        if (adjacentMatrix[i][j] != 0) {
            if (color[j] == -1) {   //探索到回边,存在环
                cycle[cycleNum][0] = i;
                cycle[cycleNum][1] = j;
                cycleNum++;
            } else if (color[j] == 0) {
                dfs(j);
            }
        }
    }
    color[i] = 1;//表示i的后裔节点都被访问过  
}

// 深度优先搜索
void MySimulation::DFS()
{
    int i;
    for (i = 0; i <= moduleNumber; i++) {
        //如果这个顶点未被访问过，则从i顶点出发进行深度优先遍历  
        if (color[i] == 0) {
            dfs(i);
        }
    }
}

void MySimulation::recordModuleNumber(const std::string& name)
{
    moduleNumber++;
    moduleList[moduleNumber] = name;
}

void MySimulation::recordRelationship(const std::string& from, const std::string& to) 
{
    relationship[relationshipNum][0] = from;
    relationship[relationshipNum][1] = to;
    relationshipNum++;
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
                gainInfo.gainValue = gain_config_arrays[i]["value"].GetDouble();
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
            gainModule[name] = gainInfo;
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
            sumModule[name] = sumInfo;
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
            multModule[name] = multInfo;
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
                consInfo.consValue = cons_config_arrays[i]["value"].GetDouble();
            }

            if (cons_config_arrays[i].HasMember("output") && cons_config_arrays[i]["output"].IsArray()) {
                auto outputArray = cons_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    consInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            consModule[name] = consInfo;
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
                sineInfo.sineValue = sine_config_arrays[i]["value"].GetDouble();
            }

            if (sine_config_arrays[i].HasMember("output") && sine_config_arrays[i]["output"].IsArray()) {
                auto outputArray = sine_config_arrays[i]["output"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    std::string outputName = outputArray[j].GetString();
                    sineInfo.output[outputName] = 0;
                    recordRelationship(name, outputName);
                }
            }
            sineModule[name] = sineInfo;
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
            dispModule[name] = dispInfo;
        }
    }
}

void MySimulation::parseStepSize(const rapidjson::Document& doc)
{
    if (doc.HasMember("step") && doc["step"].IsArray()) {
        auto step_config_arrays = doc["step"].GetArray();
        std::cout << "Size: " << step_config_arrays.Size() << std::endl;
        for (unsigned int i = 0; i < step_config_arrays.Size(); i++) {
            if (step_config_arrays[i].HasMember("finaltime") && step_config_arrays[i]["finaltime"].IsInt()) {
                step.finalTime = step_config_arrays[i]["finaltime"].GetInt();
                std::cout << "finalTime: " << step.finalTime << std::endl;
            }

            if (step_config_arrays[i].HasMember("stepsize") && step_config_arrays[i]["stepsize"].IsDouble()) {
                step.stepSize = step_config_arrays[i]["stepsize"].GetDouble();
                std::cout << "stepSize: " << step.stepSize << std::endl;
            }
        }
    }
}

bool MySimulation::parseJsonConf()
{
    std::ifstream ifs(MODEL_DATA_JSON_FILE_PATH.c_str());
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
    for (int i = 0; i <= moduleNumber; i++) {
        if (moduleList[i] == moduleName) {
            return i;
        }
    }
}

void MySimulation::creatAdjacentMatrix()
{
    for (int i = 0; i < relationshipNum; i++) {
        int from = getModulePosition(relationship[i][0]);
        int to = getModulePosition(relationship[i][1]);
        adjacentMatrix[from][to] = 1;
    }
}

double MySimulation::calculateSimulationResult(std::string lastModel, double sinValue, bool getPreValue)
{
    if (lastModel.find("cons") != std::string::npos) {
        consModule[lastModel].outputValue = consModule[lastModel].consValue;
        std::cout << lastModel << ":" << consModule[lastModel].outputValue << std::endl;
        return consModule[lastModel].outputValue;
    }

    if (lastModel.find("sum") != std::string::npos) {
        if (getPreValue) {
            return sumModule[lastModel].outputValue;
        }
        int j = 0;
        for (j; j < 2; j++) {
            bool isCycle = false;

            for (int i = 0; i < cycleNum; i++) {
                if (moduleList[cycle[i][1]] == lastModel && sumModule[lastModel].input[j] == moduleList[cycle[i][0]]) {
                    isCycle = true;
                    break;
                }
            }
            if (isCycle) {
                break;
            } 

        }

        if (j == 2) {
            sumModule[lastModel].outputValue = calculateSimulationResult(sumModule[lastModel].input[0], sinValue, false) +
                calculateSimulationResult(sumModule[lastModel].input[1], sinValue, false);
        } else if (j == 0) {
            sumModule[lastModel].outputValue = calculateSimulationResult(sumModule[lastModel].input[0], sinValue, true) +
                calculateSimulationResult(sumModule[lastModel].input[1], sinValue, false);
        } else {
            sumModule[lastModel].outputValue = calculateSimulationResult(sumModule[lastModel].input[0], sinValue, false) +
                calculateSimulationResult(sumModule[lastModel].input[1], sinValue, true);
        }
        std::cout << lastModel << ":" << sumModule[lastModel].outputValue << std::endl;
        return sumModule[lastModel].outputValue;
    }

    if (lastModel.find("gain") != std::string::npos) {
        if (getPreValue) {
            return gainModule[lastModel].outputValue;
        }
        gainModule[lastModel].outputValue = gainModule[lastModel].gainValue * calculateSimulationResult(gainModule[lastModel].info.input[0], sinValue, false);
        std::cout << lastModel << ":" << gainModule[lastModel].outputValue << std::endl;
        return gainModule[lastModel].outputValue;
    }

    if (lastModel.find("sine") != std::string::npos) {
        // using the rad
        //const double pi = atan(1.0) * 4;
        //sineModel[lastModel].outputValue = sineModel[lastModel].sineValue * sin(sinValue * pi / 180);
        sineModule[lastModel].outputValue = sineModule[lastModel].sineValue * sin(sinValue);
        std::cout << lastModel<< " : " << sinValue << " : " << sineModule[lastModel].outputValue << std::endl;
        return sineModule[lastModel].outputValue;
    }

    if (lastModel.find("mult") != std::string::npos) {
        if (getPreValue) {
            std::cout << lastModel << ":" << multModule[lastModel].outputValue << std::endl;
            return multModule[lastModel].outputValue;
        }

        int j = 0;

        for (j; j < 2; j++) {
            bool isCycle = false;
            for (int i = 0; i < cycleNum; i++) {
                if (moduleList[cycle[i][1]] == lastModel && multModule[lastModel].input[j] == moduleList[cycle[i][0]]) {
                    isCycle = true;
                    break;
                }
            }
            if (isCycle) {
                break;
            }
        }
        if (j == 2) {
            multModule[lastModel].outputValue = calculateSimulationResult(multModule[lastModel].input[0], sinValue, false) *
                calculateSimulationResult(multModule[lastModel].input[1], sinValue, false);
        } else if (j == 0) {
            multModule[lastModel].outputValue = calculateSimulationResult(multModule[lastModel].input[0], sinValue, true) *
                calculateSimulationResult(multModule[lastModel].input[1], sinValue, false);
        } else {
            multModule[lastModel].outputValue = calculateSimulationResult(multModule[lastModel].input[0], sinValue, false) *
                calculateSimulationResult(multModule[lastModel].input[1], sinValue, true);
        }
        std::cout << lastModel << ":" << multModule[lastModel].outputValue << std::endl;
        return multModule[lastModel].outputValue;
    }
}

void MySimulation::startSimulation()
{
    if (!parseJsonConf()) {
        return;
    }

    DFS();
    long long stepCount = 0;
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::cout << "当前时间: " << asctime(timeinfo) << "  finalTime: " << step.finalTime << "  stepSize: " << step.stepSize << std::endl;
    long long stepNum = step.finalTime / step.stepSize;
    while(stepCount <= stepNum) {
        std::cout << "第" << stepCount << "步" << std::endl;
        for (auto it : dispModule) {
            for (auto input : it.second.input) {
                outFile << calculateSimulationResult(input.first, stepCount * step.stepSize, false) << std::endl;
            }
        }
        stepCount++;
    }

}


bool MySimulation::ioCheck(const std::string* ioList, const int listSize)
{
    for (int i = 0; i < listSize; i++) {
        if (getModulePosition(ioList[i]) <= moduleNumber) {
            return true;
        }
    }
    return false;
}


bool MySimulation::checkGain()
{
    bool ret = true;
    for (auto it : gainModule) {
        if (sizeof(it.second.info.input) == 0) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (it.second.info.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        if (!ioCheck(it.second.info.input, size(it.second.info.input))) {
            ret = false;
            std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
        }

        for (auto out : it.second.info.output) {
            if (getModulePosition(out.first) > moduleNumber) {
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
    for (auto it : sumModule) {
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
            if (getModulePosition(out.first) > moduleNumber) {
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
    for (auto it : multModule) {
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
            if (getModulePosition(out.first) > moduleNumber) {
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
    for (auto it : sineModule) {
        if (it.second.output.empty()) {
            std::cout << it.first << "\033[31m: the output not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.output) {
            if (getModulePosition(out.first) > moduleNumber) {
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
    for (auto it : dispModule) {
        if (it.second.input.empty()) {
            std::cout << it.first << "\033[31m: the input not connect!!!\033[0m" << std::endl;
            ret = false;
        }

        for (auto out : it.second.input) {
            if (getModulePosition(out.first) > moduleNumber) {
                ret = false;
                std::cout << it.first << "\033[31m: the input error!!!\033[0m" << std::endl;
            }
        }
    }
    return ret;
}


bool MySimulation::moduleValidityCheck()
{
    return  checkGain() && checkSum() && checkMult() && checkDisp() && checkSine();
}

void MySimulation::showData()
{
    for (auto it : gainModule) {
        std::cout << "gain name: " << it.first << std::endl;
        for (int i = 0; i < size(it.second.info.input); i++) {
            std::cout << "    input: " << it.second.info.input[i] << std::endl;
        }

        for (auto output : it.second.info.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
    }

    for (auto it : sumModule) {
        std::cout << "sum name: " << it.first << std::endl;
        for (int i = 0; i < size(it.second.input); i++) {
            std::cout << "    input: " << it.second.input[i] << std::endl;
        }
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
    }


    for (auto it : multModule) {
        std::cout << "mult name: " << it.first << std::endl;
        for (int i = 0; i < size(it.second.input); i++) {
            std::cout << "    input: " << it.second.input[i] << std::endl;
        }
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
    }


    for (auto it : sineModule) {
        std::cout << "sine name: " << it.first << std::endl;
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
        std::cout << "    sineValue: " << it.second.sineValue << std::endl;
    }


    for (auto it : consModule) {
        std::cout << "cons name: " << it.first << std::endl;
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
        std::cout << "    consValue: " << it.second.consValue << std::endl;
    }


    for (auto it : dispModule) {
        std::cout << "disp name: " << it.first << std::endl;
        for (auto input : it.second.input) {
            std::cout << "    input: " << input.first << " " << input.second << std::endl;
        }
    }
    std::cout << "step: " << step.stepCout << std::endl;

    std::cout << "*********************\n\n" << std::endl;
    for (int i = 0; i <= moduleNumber; i++) {
        std::cout << moduleList[i] << std::endl;
    }
    std::cout << "*********************\n\n" << std::endl;
    for (int i = 0; i < relationshipNum; i++) {
        std::cout << relationship[i][0] << ", "<< relationship[i][1] << std::endl;
    }

    std::cout << "***********moduleList**********\n\n     adjacentMatrix:" << std::endl;
    for (int k = 0; k <= moduleNumber; k++) {
        std::cout << moduleList[k] << "  ";
    }
    std::cout << std::endl;;
    for (int i = 0; i <= moduleNumber; i++) {
        std::cout << moduleList[i] << ": ";
        for (int j = 0; j <= moduleNumber; j++) {
            std::cout << adjacentMatrix[i][j] << "  ";
        }
        std::cout << "          " << std::endl;
    }

    std::cout << "***********cycle**********\n\n" << std::endl;
    for (int i = 0; i < cycleNum; i++) {
        std::cout << "环在" << "from: [" << moduleList[cycle[i][0]] << "]  to: [" << moduleList[cycle[i][1]] << "]" << std::endl;
    }

    
}
