#include "scene.h"
#include<iostream>
#include<vector>

    std::vector<std::vector<int>> CScene::generate()                                                     //场景生成
{

    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < 9; i++)
    {
        matrix.push_back(std::vector<int>(9, 0));               //创建一个9x9的空矩阵
    }

    for(int i=0;i<0;i++)                                                       //每次循环填入单个种子矩阵;依靠列column判断
    {

    }
        return matrix;
}

    std::vector<int> shuffle_unit();
{
    std::vector<int> source={1,2,3,4,5,6,7,8,9}
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(source.begin(), source.end(), g);
    return source;
}
