#include "modeldata.h"

Timing ModuleData::ModuleData::m_step;                      // the infomation about step (step size and final time).
std::map<std::string, Gain> ModuleData::m_gainModule;       // the gain module infomation.
std::map<std::string, ModuleInfo> ModuleData::m_sumModule;  // the sum module infomation.
std::map<std::string, ModuleInfo> ModuleData::m_multModule; // the mult module infomation.
std::map<std::string, Sine> ModuleData::m_sineModule;       // the sine module infomation.
std::map<std::string, Cons> ModuleData::m_consModule;       // the cons module infomation.
std::map<std::string, Disp> ModuleData::m_dispModule;       // the disp module infomation.
std::vector<std::string> ModuleData::m_moduleList;          // the modules array.
// the input/output MyClass::relationship of the modules.
std::vector<std::vector<std::string>> ModuleData::m_relationship;