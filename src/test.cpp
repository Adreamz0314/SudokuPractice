#include<iostream>
#include<vector>
#include "utility.inl"
#include "scene.h"

int main()
{

    CScene test;

    std::cout<<"接下来是随机数测试\n";
    std::vector<int> test_shuffle_unit=test.shuffle_unit();
    for(int i=0;i<9;i++)    std::cout<<test_shuffle_unit[i]<<" ";
    std::cout<<std::endl;

std::vector<std::vector<int>> Test =test.generate();

// 遍历矩阵并打印
    message("开始打印矩阵");
for(size_t i = 0; i < Test.size(); i++)  // size_t 替代 int
{
    for(size_t j = 0; j < Test[i].size(); j++)  // size_t 替代 int
    {
        std::cout << Test[i][j] << " ";
    }
    std::cout << std::endl;
}
    message("矩阵打印完成");




    return 0;
}
