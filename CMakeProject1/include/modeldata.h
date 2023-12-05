/**@file  : modeldata.h
 *@brief  : descripe the module infomation
 *@author :
 *@date   : 2023.12.04
 *@version: V0.1
 *@note   : note
 *@detailed description:
 */

#ifndef _MODEL_DATA_H_
#define _MODEL_DATA_H_
#include <iostream>
#include <map>

const int MAXNUM = 100;

#pragma pack(1)
typedef struct ModuleInfo {
    std::string input[2];
    std::map<std::string, double> output;
    double outputValue = 0.0;
}ModuleInfo;

typedef struct Gain {
    ModuleInfo info;
    double outputValue;
    double gainValue;
}Gain;

typedef struct Sine
{
    std::map<std::string, double> output;
    double sineValue;
    double outputValue;
}Sine;

typedef struct Cons
{
    std::map<std::string, double> output;
    double consValue;
    double outputValue;
}Cons;

typedef struct Disp
{
    std::map<std::string, double> input;   // the input of disp module
}Disp;

typedef struct Timing 
{
    int finalTime;   // the final time
    double stepSize; // the step size
}Timing;

#pragma pack()

#endif // _MODEL_DATA_H_