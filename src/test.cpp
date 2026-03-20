#include<iostream>
#include<vector>
#include "utility.inl"
#include "scene.h"

int main()
{

    std::cout<<std::endl;
    message("Test1");
    message("Test2",1);
    message("Test3",true);
    message("Test4",0);
    message("Test5",false);
    std::cout<<std::endl;

    CScene test;
    std::vector<std::vector<int>> Test =test.generate();
    // 遍历矩阵并打印

for(size_t i = 0; i < Test.size(); i++)  // size_t 替代 int
{
    for(size_t j = 0; j < Test[i].size(); j++)  // size_t 替代 int
    {
        std::cout << Test[i][j] << " ";
    }
    std::cout << std::endl;
}
    return 0;
}
