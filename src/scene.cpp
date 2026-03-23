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

        matrix=real_number(matrix);

        return matrix;
}

    std::vector<std::vector<int>> CScene::real_number(std::vector<std::vector<int>> matrix)                                                   //返回最终填入矩阵
    {
        std::map<std::pair<int,int>,std::vector<int>> avail_num_map;



        std::vector<std::vector<int>> First_cycle_matrix;               //第一次循环校验

        for (int i = 0; i < 9; i++)
    {
        First_cycle_matrix.push_back(std::vector<int>(9, 1));
    }

        int GotoCheck=1;                                                //goto校验
        int GotoCheckColumn;
        Restart_Aim:

        for(int aim_column=0;aim_column<9;aim_column++)
        {
            if(GotoCheck==0)
            {
                aim_column=GotoCheckColumn;

            }

            for(int aim_row=0;aim_row<9;aim_row++)
        {

            if(GotoCheck==0)
            {
                aim_row=8;
                GotoCheck=1;
                matrix[aim_column][aim_row]=0;
            }

            if (matrix[aim_column][aim_row]==0)
                {
                    std::vector<int> avail_num={1,2,3,4,5,6,7,8,9};


                    for(int i=0;i<9;i++)                                //检查当前行所有格子
                    {
                        if(i==aim_column)
                        {
                            continue;
                        }
                        if(matrix[i][aim_row]!=0)
                        {
                            avail_num.erase(std::remove(avail_num.begin(), avail_num.end(), matrix[i][aim_row]), avail_num.end());
                        }

                    }

                    for(int i=0;i<9;i++)                                //检查当前列所有格子
                    {
                        if(i==aim_row)
                        {
                            continue;
                        }
                        if(matrix[aim_column][i]!=0)
                        {
                            avail_num.erase(std::remove(avail_num.begin(), avail_num.end(), matrix[aim_column][i]), avail_num.end());
                        }

                    }

                    for(int i=aim_column/3*3;i<aim_column/3*3+3;i++)   //检查当前小矩阵内所有格子
                    {
                        for(int j=aim_row/3*3;j<aim_row/3*3+3;j++)
                        {
                            if(i==aim_column&&j==aim_row)
                            {
                                continue;
                            }

                            if(matrix[i][j]!=0)
                            {
                                avail_num.erase(std::remove(avail_num.begin(), avail_num.end(), matrix[i][j]), avail_num.end());
                            }
                        }
                    }

                    if(avail_num.size()==0)                         //检查可用数字，如果为空则返回上一次循环
                    {
                        if(aim_row!=0)
                        {
                            aim_row--;
                            continue;
                        }
                        else
                        {
                            aim_row=8;
                            GotoCheckColumn=aim_column--;
                            GotoCheck=0;
                            goto Restart_Aim;
                        }
                    }

                    if(First_cycle_matrix[aim_column][aim_row]==1)                                  //若为第一次循环，则将可用数字存入map
                    {
                    avail_num_map[std::make_pair(aim_column, aim_row)] = avail_num;
                    First_cycle_matrix[aim_column][aim_row]=0;

                    }

                    auto it = avail_num_map.find(std::pair<int,int>(aim_column,aim_row));
                    if (it != avail_num_map.end())          // 检查key是否存在
                    {
                    std::vector<int>& vec = it->second;          // 取vector的引用（修改原数据
                    matrix[aim_column][aim_row] = vec[0];
                    vec.erase(vec.begin());                                                         //可用数字不为空，为当前方块赋值并删去已用值
                    }



                }
        }
        }

        return matrix;
    }


    std::vector<int> CScene::shuffle_unit()
{
    std::vector<int> source={1,2,3,4,5,6,7,8,9};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(source.begin(), source.end(), g);
    return source;
}
