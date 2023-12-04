#include "parsejson.h"
#include <cmath>
#include <ctime>

const std::string MODEL_DATA_JSON_FILE_PATH = "../modelconfig.json";
const double pi = atan(1.0) * 4;

MySimulation::MySimulation()
{
    mVexNum = -1;
    cycleNum = 0;
    relationshipNum = 0;
    memset(cycle, 0, sizeof(cycle));
    memset(color, 0,sizeof(color));
    memset(adjacentMatrix, 0, sizeof(adjacentMatrix));

    for (auto it : gainModel) {
        it.second.preValue = 0.0;
    }

    for (auto it : sumModel) {
        it.second.preValue = 0.0;
    }

    for (auto it : multModel) {
        it.second.preValue = 0;
        it.second.outputValue = 0;
    }

    for (auto it : sineModel) {
        it.second.preValue = 0.0;
    }
    outFile.open("data.csv");
}

MySimulation::~MySimulation()
{
    outFile.close();
}

void MySimulation::dfs( int i)
{
    color[i] = -1;
    for (int j = 0; j <= mVexNum; j++) {
        if (adjacentMatrix[i][j] != 0) {
            if (color[j] == -1) {   //探索到回边,存在环
                is_DAG = false;     //是有向无环图
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
    for (i = 0; i <= mVexNum; i++) {
        //如果这个顶点未被访问过，则从i顶点出发进行深度优先遍历  
        if (color[i] == 0) {
            dfs(i);
        }
    }
}

void MySimulation::recordVexs(const std::string& name)
{
    mVexNum++;
    moduleList[mVexNum] = name;
}

void MySimulation::recordRelationship(const std::string& from, const std::string& to) 
{
    relationship[relationshipNum][0] = from;
    relationship[relationshipNum][1] = to;
    relationshipNum++;
}


void MySimulation::parseGainModel(const rapidjson::Document& doc)
{
    if (doc.HasMember("gain") && doc["gain"].IsArray()) {
        auto gain_config_arrays = doc["gain"].GetArray();
        for (unsigned int i = 0; i < gain_config_arrays.Size(); i++) {
            std::string name;
            Gain gainInfo;
            if (gain_config_arrays[i].HasMember("name") && gain_config_arrays[i]["name"].IsString()) {
                name = gain_config_arrays[i]["name"].GetString();
                recordVexs(name);
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
            gainModel[name] = gainInfo;
        }
    }
}

void MySimulation::parseSumModel(const rapidjson::Document& doc)
{
    if (doc.HasMember("sum") && doc["sum"].IsArray()) {
        auto sum_config_arrays = doc["sum"].GetArray();
        for (unsigned int i = 0; i < sum_config_arrays.Size(); i++) {
            std::string name;
            ModuleInfo sumInfo;
            if (sum_config_arrays[i].HasMember("name") && sum_config_arrays[i]["name"].IsString()) {
                name = sum_config_arrays[i]["name"].GetString();
                recordVexs(name);
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
            sumModel[name] = sumInfo;
        }
    }
}

void MySimulation::parseMultModel(const rapidjson::Document& doc)
{
    if (doc.HasMember("mult") && doc["mult"].IsArray()) {
        auto mult_config_arrays = doc["mult"].GetArray();
        for (unsigned int i = 0; i < mult_config_arrays.Size(); i++) {
            std::string name;
            ModuleInfo multInfo;
            if (mult_config_arrays[i].HasMember("name") && mult_config_arrays[i]["name"].IsString()) {
                name = mult_config_arrays[i]["name"].GetString();
                recordVexs(name);
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
            multModel[name] = multInfo;
        }
    }
}

void MySimulation::parseConsModel(const rapidjson::Document& doc)
{
    if (doc.HasMember("cons") && doc["cons"].IsArray()) {
        auto cons_config_arrays = doc["cons"].GetArray();
        for (unsigned int i = 0; i < cons_config_arrays.Size(); i++) {
            std::string name;
            Cons consInfo;
            if (cons_config_arrays[i].HasMember("name") && cons_config_arrays[i]["name"].IsString()) {
                name = cons_config_arrays[i]["name"].GetString();
                recordVexs(name);
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
            consModel[name] = consInfo;
        }
    }
}

void MySimulation::parseSineModel(const rapidjson::Document& doc)
{
    if (doc.HasMember("sine") && doc["sine"].IsArray()) {
        auto sine_config_arrays = doc["sine"].GetArray();
        for (unsigned int i = 0; i < sine_config_arrays.Size(); i++) {
            std::string name;
            Sine sineInfo;
            if (sine_config_arrays[i].HasMember("name") && sine_config_arrays[i]["name"].IsString()) {
                name = sine_config_arrays[i]["name"].GetString();
                recordVexs(name);
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
            sineModel[name] = sineInfo;
        }
    }
}

void MySimulation::parseDspModel(const rapidjson::Document& doc)
{
    if (doc.HasMember("disp") && doc["disp"].IsArray()) {
        auto disp_config_arrays = doc["disp"].GetArray();
        for (unsigned int i = 0; i < disp_config_arrays.Size(); i++) {
            std::string name;
            Disp dispInfo;
            if (disp_config_arrays[i].HasMember("name") && disp_config_arrays[i]["name"].IsString()) {
                name = disp_config_arrays[i]["name"].GetString();
                recordVexs(name);
            }

            if (disp_config_arrays[i].HasMember("input") && disp_config_arrays[i]["input"].IsArray()) {
                auto outputArray = disp_config_arrays[i]["input"].GetArray();
                for (unsigned int j = 0; j < outputArray.Size(); j++) {
                    dispInfo.input[outputArray[j].GetString()] = 0;
                }
            }
            dispModel[name] = dispInfo;
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

bool MySimulation::initJsonConf()
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
    parseSineModel(doc);
    parseConsModel(doc);
    parseSumModel(doc);
    parseGainModel(doc);
    parseMultModel(doc);
    parseDspModel(doc);
    parseStepSize(doc);
    creatAdjacentMatrix();
    return true;
}

int MySimulation::getPosition(std::string moduleName)
{
    for (int i = 0; i <= mVexNum; i++) {
        if (moduleList[i] == moduleName) {
            return i;
        }
    }
}

void MySimulation::creatAdjacentMatrix()
{
    for (int i = 0; i < relationshipNum; i++) {
        int from = getPosition(relationship[i][0]);
        int to = getPosition(relationship[i][1]);
        adjacentMatrix[from][to] = 1;
    }
}

double MySimulation::startSimulation(std::string lastModel, double sinValue, bool getPreValue)
{
    if (lastModel.find("cons") != std::string::npos) {
        consModel[lastModel].outputValue = consModel[lastModel].consValue;
        std::cout << lastModel << ":" << consModel[lastModel].outputValue << std::endl;
        return consModel[lastModel].outputValue;
    }

    if (lastModel.find("sum") != std::string::npos) {
        if (getPreValue) {
            return sumModel[lastModel].outputValue;
        }
        int j = 0;
        for (j; j < 2; j++) {
            bool isCycle = false;

            for (int i = 0; i < cycleNum; i++) {
                if (moduleList[cycle[i][1]] == lastModel && sumModel[lastModel].input[j] == moduleList[cycle[i][0]]) {
                    isCycle = true;
                    break;
                }
            }
            if (isCycle) {
                break;
            } 

        }

        if (j == 2) {
            sumModel[lastModel].outputValue = startSimulation(sumModel[lastModel].input[0], sinValue, false) +
                startSimulation(sumModel[lastModel].input[1], sinValue, false);
        }
        else if (j == 0) {
            sumModel[lastModel].outputValue = startSimulation(sumModel[lastModel].input[0], sinValue, true) +
                startSimulation(sumModel[lastModel].input[1], sinValue, false);
        }
        else {
            sumModel[lastModel].outputValue = startSimulation(sumModel[lastModel].input[0], sinValue, false) +
                startSimulation(sumModel[lastModel].input[1], sinValue, true);
        }
        std::cout << lastModel << ":" << sumModel[lastModel].outputValue << std::endl;
        return sumModel[lastModel].outputValue;
    }

    if (lastModel.find("gain") != std::string::npos) {
        if (getPreValue) {
            return gainModel[lastModel].outputValue;
        }
        gainModel[lastModel].outputValue = gainModel[lastModel].gainValue * startSimulation(gainModel[lastModel].info.input[0], sinValue, false);
        std::cout << lastModel << ":" << gainModel[lastModel].outputValue << std::endl;
        return gainModel[lastModel].outputValue;
    }

    if (lastModel.find("sine") != std::string::npos) {
        //sineModel[lastModel].outputValue = sineModel[lastModel].sineValue * sin(sinValue * pi / 180);
        sineModel[lastModel].outputValue = sineModel[lastModel].sineValue * sin(sinValue);
        std::cout << lastModel<< " : " << sinValue << " : " << sineModel[lastModel].outputValue << std::endl;
        return sineModel[lastModel].outputValue;
    }

    if (lastModel.find("mult") != std::string::npos) {
        if (getPreValue) {
            std::cout << lastModel << ":" << multModel[lastModel].outputValue << std::endl;
            return multModel[lastModel].outputValue;
        }

        int j = 0;

        for (j; j < 2; j++) {
            bool isCycle = false;
            for (int i = 0; i < cycleNum; i++) {
                if (moduleList[cycle[i][1]] == lastModel && multModel[lastModel].input[j] == moduleList[cycle[i][0]]) {
                    isCycle = true;
                    break;
                }
            }
            if (isCycle) {
                break;
            }
        }
        if (j == 2) {
            multModel[lastModel].outputValue = startSimulation(multModel[lastModel].input[0], sinValue, false) *
                startSimulation(multModel[lastModel].input[1], sinValue, false);
        } else if (j == 0) {
            multModel[lastModel].outputValue = startSimulation(multModel[lastModel].input[0], sinValue, true) *
                startSimulation(multModel[lastModel].input[1], sinValue, false);
        } else {
            multModel[lastModel].outputValue = startSimulation(multModel[lastModel].input[0], sinValue, false) *
                startSimulation(multModel[lastModel].input[1], sinValue, true);
        }
        std::cout << lastModel << ":" << multModel[lastModel].outputValue << std::endl;
        return multModel[lastModel].outputValue;
    }
}

void MySimulation::setPreValue()
{
    for (auto it : gainModel) {
        it.second.preValue = it.second.outputValue;
    }

    for (auto it : sumModel) {
        std::cout << it.second.preValue << " : " << it.second.outputValue << std::endl;
        it.second.preValue = it.second.outputValue;
        std::cout << it.second.preValue << " : " << it.second.outputValue << std::endl;
    }

    for (auto it : multModel) {
        it.second.preValue = it.second.outputValue;
    }

    for (auto it : sineModel) {
        it.second.preValue = it.second.outputValue;
    }
}

void MySimulation::displayResult()
{
    DFS();
    long long stepCount = 0;
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    std::cout << "当前时间: " << asctime(timeinfo) << "  finalTime: " << step.finalTime << "  stepSize: " << step.stepSize << std::endl;
    long long stepNum = step.finalTime / step.stepSize;
    while(stepCount <= stepNum) {
        setPreValue();
        std::cout << "第" << stepCount << "步" << std::endl;
        for (auto it : dispModel) {
            for (auto input : it.second.input) {
                outFile << startSimulation(input.first, stepCount * step.stepSize, false) << std::endl;
            }
        }
        stepCount++;
    }

}

void MySimulation::showData()
{
    for (auto it : gainModel) {
        std::cout << "gain name: " << it.first << std::endl;
        for (int i = 0; i < size(it.second.info.input); i++) {
            std::cout << "    input: " << it.second.info.input[i] << std::endl;
        }

        for (auto output : it.second.info.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
    }

    for (auto it : sumModel) {
        std::cout << "sum name: " << it.first << std::endl;
        for (int i = 0; i < size(it.second.input); i++) {
            std::cout << "    input: " << it.second.input[i] << std::endl;
        }
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
    }


    for (auto it : multModel) {
        std::cout << "mult name: " << it.first << std::endl;
        for (int i = 0; i < size(it.second.input); i++) {
            std::cout << "    input: " << it.second.input[i] << std::endl;
        }
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
    }


    for (auto it : sineModel) {
        std::cout << "sine name: " << it.first << std::endl;
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
        std::cout << "    sineValue: " << it.second.sineValue << std::endl;
    }


    for (auto it : consModel) {
        std::cout << "cons name: " << it.first << std::endl;
        for (auto output : it.second.output) {
            std::cout << "    output: " << output.first << " " << output.second << std::endl;
        }
        std::cout << "    consValue: " << it.second.consValue << std::endl;
    }


    for (auto it : dispModel) {
        std::cout << "disp name: " << it.first << std::endl;
        for (auto input : it.second.input) {
            std::cout << "    input: " << input.first << " " << input.second << std::endl;
        }
    }
    std::cout << "step: " << step.stepCout << std::endl;

    std::cout << "*********************\n\n" << std::endl;
    for (int i = 0; i <= mVexNum; i++) {
        std::cout << moduleList[i] << std::endl;
    }
    std::cout << "*********************\n\n" << std::endl;
    for (int i = 0; i < relationshipNum; i++) {
        std::cout << relationship[i][0] << ", "<< relationship[i][1] << std::endl;
    }

    std::cout << "***********moduleList**********\n\n     adjacentMatrix:" << std::endl;
    for (int k = 0; k <= mVexNum; k++) {
        std::cout << moduleList[k] << "  ";
    }
    std::cout << std::endl;;
    for (int i = 0; i <= mVexNum; i++) {
        std::cout << moduleList[i] << ": ";
        for (int j = 0; j <= mVexNum; j++) {
            std::cout << adjacentMatrix[i][j] << "  ";
        }
        std::cout << "          " << std::endl;
    }

    std::cout << "***********cycle**********\n\n" << std::endl;
    for (int i = 0; i < cycleNum; i++) {
        std::cout << "环在" << "from: [" << moduleList[cycle[i][0]] << "]  to: [" << moduleList[cycle[i][1]] << "]" << std::endl;
    }

    
}
