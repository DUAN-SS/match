/**@file  : matrix.h
 *@brief  : Create a zero-order matrix
 *@author : yongping.duan@keliangtek.com
 *@date   : 2023.12.04
 *@version: V0.1
 *@note   : note
 *@detailed description:
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <string>
#include <vector>

class Matrix
{
public:
    static Matrix& getInstance()
    {
        static Matrix matrix;
        return matrix;
    }

    /** 
     * @brief     : create the adjacent matrix.
     * @author    : yongping.duan@keliangtek.com
     * @param[in] : std::string MyClass::relationship[MAXNUM][2].
     *              int relationshipNum.
     * @param[out]: int adjacentMatrix[MAXNUM][MAXNUM].
     * @return    : none
     * @note      : none
     */
    void creatAdjacentMatrix();
    inline const std::vector<std::vector<int>>& getAdjacentMatrix() { return m_adjacentMatrix; }

private:
    Matrix();
    ~Matrix();
    std::vector<std::vector<int>> m_adjacentMatrix; // Zero-order matrix, indicates the
                                                    // MyClass::relationship between modules, set the
                                                    // value is 1 if two modules are related.
                                                    // eg: the output of module1 is the input of module2.
                                                    // |       | module1  | module2  |
                                                    // |module1|   0      |    1     |
                                                    // |module2|   0      |    0     |

    /** 
     * @brief     : get the module location in the moduleList.
     * @author    : yongping.duan@keliangtek.com
     * @param[in] : std::string moduleName: the name of module.
     *              int relationshipNum.
     * @param[out]: none
     * @return    : the module index.
     * @note      : none
     */
    int getModulePosition(const std::string &module_name);
    Matrix(const Matrix& matrix) = delete; // prohibit external copy constructs.
    // disable external assignment operations.
    const Matrix& operator=(const Matrix& matrix) = delete;
};
#endif
