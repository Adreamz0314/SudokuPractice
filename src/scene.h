#pragma once
#include <iostream>
#include <vector>

class CScene
{
public:
    std::vector<std::vector<int>> generate(int* outCycleTime);
    std::vector<int> shuffle_unit();
    std::vector<std::vector<int>> FucRealMatrMake(std::vector<std::vector<int>> matrix);
    std::vector<std::pair<int,int>> FunBlankSpacePairVec(std::vector<std::vector<int>> matrix);
};
