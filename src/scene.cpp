#include "scene.h"
#include<iostream>

    std::auto CScene::generate()                                                     //场景生成
{

    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < 9; i++)
    {
        matrix.push_back(std::vector<int>(9, 0));   //创建一个9x9的空矩阵
    }
        return matrix;
}
