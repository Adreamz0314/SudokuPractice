#include "scene.h"
#include<iostream>
#include<vector>
#include<random>
#include <algorithm>
#include<map>
#include<utility>

    std::vector<std::vector<int>> CScene::generate()                            //场景生成
{

    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < 9; i++)                                                //创建一个9x9的空矩阵
    {
        matrix.push_back(std::vector<int>(9, 0));
    }

    for(int i=0;i<3;i++)                                                       //每次循环填入单个种子矩阵
    {
        std::vector<int> unit = shuffle_unit();
        int start_index=i*3;
        for(int column=start_index;column<start_index+3;column++)
        {
            for(int row=start_index;row<start_index+3;row++)
            {
                matrix[row][column]=unit.back();
                unit.pop_back();
            }
        }
    }

    for(int aim_column=0;aim_column<9;aim_column++)                         //填写程序
        {
            for(int aim_row=0;aim_row<9;aim_row++)
        {
            if (matrix[aim_column][aim_row]==0)
                {
                   matrix[aim_column][aim_row]=real_number();           //为空格赋值
                }
        }
        }

        return matrix;
}

    int real_number()                                                   //为每个空格返回最终填入数字
    {
        std::map<std::pair<int,int>,std::vector<int>> avail_num;


        for(int aim_column=0;aim_column<9;aim_column++)
        {
            for(int aim_row=0;aim_row<9;aim_row++)
        {
            if (matrix[aim_column][aim_row]==0)
                {
                    avail_num.emplace(std::pair<int,int>(aim_column,aim_row),{1,2,3,4,5,6,7,8,9});

                    for(int i=0;i<9;i++)                                //检查当前行所有格子
                    {
                        if(i==aim_column)
                        {
                            continue;
                        }
                        if(matrix[i][aim_row]!=0)
                        {
                            auto it=std::find(avail_num.find(std::pair(aim_column,aim_row)).begin(), std::find(avail_num.find(std::pair(aim_column,aim_row)).end(), matrix[i][aim_row]);
                            avail_num.erase(it);
                        }

                    }
                }
        }
        }

        return real_number;
    }


    std::vector<int> CScene::shuffle_unit()
{
    std::vector<int> source={1,2,3,4,5,6,7,8,9};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(source.begin(), source.end(), g);
    return source;
}
