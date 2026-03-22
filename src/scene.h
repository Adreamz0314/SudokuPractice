#pragma once
#include <iostream>
#include <vector>

class CScene
{
public:
    std::vector<std::vector<int>> generate();
    std::vector<int> shuffle_unit();
    int avail_num_check();
};
