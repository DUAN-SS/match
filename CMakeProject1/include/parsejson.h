#ifndef _PARSEJSON_H_
#define _PARSEJSON_H_

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "modeldata.h"
#include "document.h"
#include "istreamwrapper.h"

const int MAXNUM = 100;
class MySimulation
{
private:
    Timing step;
    std::map<std::string, Gain> gainModel;
    std::map<std::string, ModuleInfo> sumModel;
    std::map<std::string, ModuleInfo> multModel;
    std::map<std::string, Sine> sineModel;
    std::map<std::string, Cons> consModel;
    std::map<std::string, Disp> dispModel;
    void parseGainModel(const rapidjson::Document& doc);
    void parseSumModel(const rapidjson::Document& doc);
    void parseMultModel(const rapidjson::Document& doc);
    void parseConsModel(const rapidjson::Document& doc);
    void parseSineModel(const rapidjson::Document& doc);
    void parseDspModel(const rapidjson::Document& doc);
    void parseStepSize(const rapidjson::Document& doc);
    double startSimulation(std::string lastModel, double sinValue, bool getPreValue);

private:
    void creatAdjacentMatrix();
    void recordVexs(const std::string& name);
    void recordRelationship(const std::string& from, const std::string& to);
    int getPosition(std::string moduleName);
    void DFS();
    void dfs(int i);
    bool dfs1(int cur);
    void MatchModule(int i);
    void setPreValue();

private:
    int cycleNum;
    int cycle[MAXNUM][2];
    int color[MAXNUM];//顶点颜色表 color[u] 
    int adjacentMatrix[MAXNUM][MAXNUM];  // 零阶矩阵
    bool is_DAG = true;//标识位，表示有向无环图  
    int mVexNum;                         // 顶点数
    std::string moduleList[MAXNUM];      // 顶点集合

    int relationshipNum;                 // 边数
    std::string relationship[MAXNUM][2]; // 边集合

private:
    std::ofstream outFile;
public:
    MySimulation();
    ~MySimulation();
    bool initJsonConf();
    void showData();
    void displayResult();
}; // class Simulation

#endif //__PARSECONFIG_H__