#include "matrix.h"
#include "modeldata.h"

Matrix::Matrix()
{
    for (int i = 0; i < ModuleData::m_moduleList.size(); i++) {
        std::vector<int> zero;
        for (int j = 0; j < ModuleData::m_moduleList.size(); j++) {
            zero.push_back(0);
        }
        m_adjacentMatrix.push_back(zero);
    }
}

Matrix::~Matrix() { }

int Matrix::getModulePosition(const std::string &moduleName)
{
    for (int i = 0; i < ModuleData::m_moduleList.size(); i++) {
        if (ModuleData::m_moduleList.at(i) == moduleName) {
            return i;
        }
    }
    std::cout << "\033[31mmoduleName[ " << moduleName << "] is -1\033[0m" << std::endl;
    return -1;
}

void Matrix::creatAdjacentMatrix()
{
    for (int i = 0; i < ModuleData::m_relationship.size(); i++) {
        int from = getModulePosition(ModuleData::m_relationship.at(i).at(0));
        int to = getModulePosition(ModuleData::m_relationship.at(i).at(1));
        if ((from != -1) && (to != -1)) {
            m_adjacentMatrix.at(from).at(to) = 1;
        }
    }
}