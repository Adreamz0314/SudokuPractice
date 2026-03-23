#pragma once
#include <iostream>
#include <vector>

class CScene
{
public:
    std::vector<std::vector<int>> generate();
    std::vector<int> shuffle_unit();
    std::vector<std::vector<int>> real_number(std::vector<std::vector<int>> matrix);
};
