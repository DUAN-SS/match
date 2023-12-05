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

const int g_MaxNumber = 100;

#pragma pack(1)
typedef struct ModuleInfo {
    std::string input[2];
    std::map<std::string, double> output;
    double output_value = 0.0;
}ModuleInfo;

typedef struct Gain {
    ModuleInfo info;
    double output_value = 0.0;
    double gain_value = 0.0;
}Gain;

typedef struct Sine
{
    std::map<std::string, double> output;
    double sine_value = 0.0;
    double output_value = 0.0;
}Sine;

typedef struct Cons
{
    std::map<std::string, double> output;
    double cons_value = 0.0;
    double output_value = 0.0;
}Cons;

typedef struct Disp
{
    std::map<std::string, double> input;   // the input of disp module
}Disp;

typedef struct Timing 
{
    int final_time = -1;   // the final time
    double step_size = 0.0; // the step size
}Timing;

#pragma pack()

#endif // _MODEL_DATA_H_