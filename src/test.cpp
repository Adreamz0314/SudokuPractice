#include<iostream>
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

    CScene test;
    std::auto test =test.generate();
    // 遍历矩阵并打印
    for(int i=0; i<test.size(); i++)
    {
        for(int j=0; j<test[i].size(); j++)
        {
            std::cout << test[i][j] << " ";
        }
        std::cout << std::endl; // 每行结束后换行，格式更清晰
    }
    return 0;
}
