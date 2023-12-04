#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_
#include <iostream>
#include <map>


#pragma pack(1)
typedef struct ModuleInfo {
    std::string input[2];
    std::map<std::string, double> output;
    double outputValue = 0.0;
    double preValue = 0.0;
}ModuleInfo;

typedef struct Gain {
    ModuleInfo info;
    double outputValue;
    double gainValue;
    double preValue;
}Gain;

typedef struct Sine
{
    std::map<std::string, double> output;
    double sineValue;
    double outputValue;
    double preValue;
}Sine;

typedef struct Cons
{
    std::map<std::string, double> output;
    double consValue;
    double outputValue;
    double preValue;
}Cons;

typedef struct Disp
{
    std::map<std::string, double> input;   // disp 模块输入
    double preValue;                       // disp模块的上一步的值
}Disp;


typedef struct Timing 
{
    int finalTime;   // 仿真时长
    double stepSize; // 仿真步长
    int stepCout;    // 仿真次数
}Timing;

#pragma pack()

#endif // _MODEL_DATA_H_