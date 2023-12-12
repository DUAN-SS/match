/**@file  : modeldata.h
 *@brief  : public class, descripe the module infomation
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
#include <string>
#include <vector>

#pragma pack(1)
typedef struct ModuleInfo {
    std::string input[2];
    std::map<std::string, double> output;
    double output_value = 0.0;
} ModuleInfo;

typedef struct Gain {
    ModuleInfo info;
    double output_value = 0.0;
    double gain_value = 0.0;
} Gain;

typedef struct Sine {
    std::map<std::string, double> output;
    double amplitude = 0.0;
    double output_value = 0.0;
} Sine;

typedef struct Cons {
    std::map<std::string, double> output;
    double cons_value = 0.0;
    double output_value = 0.0;
} Cons;

typedef struct Disp {
    std::map<std::string, double> input; // the input of disp module
} Disp;

typedef struct Timing {
    double final_time = -1.0; // the final time1
    double step_size = 0.0;   // the step size
} Timing;

#pragma pack()

class ModuleData
{
public:
    static Timing m_step;                                  // the infomation about step (step size and final time).
    static std::map<std::string, Gain> m_gainModule;       // the gain module infomation.
    static std::map<std::string, ModuleInfo> m_sumModule;  // the sum module infomation.
    static std::map<std::string, ModuleInfo> m_multModule; // the mult module infomation.
    static std::map<std::string, Sine> m_sineModule;       // the sine module infomation.
    static std::map<std::string, Cons> m_consModule;       // the cons module infomation.
    static std::map<std::string, Disp> m_dispModule;       // the disp module infomation.
    static std::vector<std::string> m_moduleList;          // the modules array.
    static std::vector<std::vector<std::string>>
            m_relationship; // the input/output MyClass::relationship of the modules.
};

#endif // _MODEL_DATA_H_