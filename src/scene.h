#pragma once
#include <iostream>
#include <vector>

class CScene
{
public:
    std::vector<std::vector<int>> generate();

private:
    std::vector<int> shuffle_unit();
};
