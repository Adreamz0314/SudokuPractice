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

    message("随机数测试完成");

    // 输出【红色加粗】的 角点符号 CORNER


    message("开始调用generate部分");
    int count;
    std::vector<std::vector<int>> matrix =test.generate(&count);

    bool game_over_check=test.game_over(matrix);
    message("game_over_check的值为",0);
    message(game_over_check);

    message("开始调用play部分");
    test.play(matrix);
    message("本次主循环的次数为",0);
    message(count);


    return 0;
}
