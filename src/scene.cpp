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
    for (int i = 0; i < 9; i++)                                                 //创建一个9x9的空矩阵
    {
        matrix.push_back(std::vector<int>(9, 0));
    }

    for(int i=0;i<3;i++)                                                        //每次循环填入单个种子矩阵
    {
        std::vector<int> unit = shuffle_unit();
        int start_index=i*3;
        for(int row=start_index;row<start_index+3;row++)
        {
            for(int column=start_index;column<start_index+3;column++)
            {
                matrix[row][column]=unit.back();
                unit.pop_back();                                              //标准库 删去最后一个数字
            }
        }
    }

    matrix=FucRealMatrMake(matrix);

    return matrix;
}

//-------------------------------------------------------------------------------------------------------------------------------


    std::vector<std::vector<int>> CScene::FucRealMatrMake(std::vector<std::vector<int>> matrix)
                                                                                //生成并返回可用矩阵
    {
        std::map<std::pair<int,int>,std::vector<int>> avail_num_map;

        std::vector<std::pair<int,int>> VarBlankSpacePairVec=FunBlankSpacePairVec(matrix);
                                                                                //统计空格坐标，以pair形式存入vector中
        std::vector<std::vector<int>> BacVerMapCheck;
        int NumsFilledCell=0;
        int BacVerCheck=1;
        std::vector<std::vector<int>> First_cycle_matrix;
        for (int i = 0; i < 9; i++)                                             //创建一个9x9的矩阵,用于第一次循环校验map填充
        {
            First_cycle_matrix.push_back(std::vector<int>(9, 1));
        }

        for (int i = 0; i < 9; i++)                                             //创建一个9x9的矩阵,用于回溯校验map填充
        {
            BacVerMapCheck.push_back(std::vector<int>(9, 1));
        }

        while(true)
        {
                    std::vector<int> avail_num={1,2,3,4,5,6,7,8,9};
                    int aim_row=VarBlankSpacePairVec[NumsFilledCell].first;
                    int aim_column=VarBlankSpacePairVec[NumsFilledCell].second;

                    if(BacVerCheck==0)
                    {
                        matrix[aim_row][aim_column]=0;
                        BacVerCheck=1;
                    }

                    for(int i=0;i<9;i++)                                        //检查当前列所有格子
                    {
                        if(i==aim_row)
                        {
                            continue;
                        }
                        if(matrix[i][aim_column]!=0)
                        {
                            avail_num.erase(std::remove(avail_num.begin(), avail_num.end(), matrix[i][aim_column]), avail_num.end());
                        }

                    }

                    for(int i=0;i<9;i++)                                        //检查当前行所有格子
                    {
                        if(i==aim_column)
                        {
                            continue;
                        }
                        if(matrix[aim_row][i]!=0)
                        {
                            avail_num.erase(std::remove(avail_num.begin(), avail_num.end(), matrix[aim_row][i]), avail_num.end());
                        }

                    }

                    for(int i=aim_row/3*3;i<aim_row/3*3+3;i++)                  //检查当前小矩阵内所有格子
                    {
                        for(int j=aim_column/3*3;j<aim_column/3*3+3;j++)
                        {
                            if(i==aim_row&&j==aim_column)
                            {
                                continue;
                            }

                            if(matrix[i][j]!=0)
                            {
                                avail_num.erase(std::remove(avail_num.begin(), avail_num.end(), matrix[i][j]), avail_num.end());
                            }
                        }
                    }

                    if(avail_num.size()==0)                                     //检查可用数字，如果为空则返回上一次循环
                    {
                        matrix[aim_row][aim_column]=0;
                        NumsFilledCell--;
                        BacVerCheck=0;
                        BacVerMapCheck[aim_row][aim_column]=0;
                        continue;
                    }

                    if(First_cycle_matrix[aim_row][aim_column]==1||BacVerMapCheck[aim_row][aim_column]==0)
                                                                                //若为第一次循环或已经回溯，则将可用数字存入map
                    {
                    avail_num_map[std::make_pair(aim_row, aim_column)] = avail_num;
                    First_cycle_matrix[aim_row][aim_column]=0;
                    BacVerMapCheck[aim_row][aim_column]=1;
                    }

                    auto it = avail_num_map.find(std::pair<int,int>(aim_row,aim_column));
                    if (it != avail_num_map.end())                              // 检查key是否存在
                    {
                    std::vector<int>& vec = it->second;                         // 取vector的引用（修改原数据
                    matrix[aim_row][aim_column] = vec[0];
                    vec.erase(vec.begin());                                     //可用数字不为空，为当前方块赋值并删去已用值
                    NumsFilledCell++;
                    }

                    if(NumsFilledCell==54)
                    {
                        break;
                    }

        }

        return matrix;
    }


    std::vector<int> CScene::shuffle_unit()                                    //返回一个随机数列
{
    std::vector<int> source={1,2,3,4,5,6,7,8,9};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(source.begin(), source.end(), g);
    return source;
}

    std::vector<std::pair<int,int>> CScene::FunBlankSpacePairVec(std::vector<std::vector<int>> matrix)     //统计空格坐标，以pair形式存入vector中
    {
        std::vector<std::pair<int,int>> VarBlankSpacePairVec;

        for(int aim_row=0;aim_row<9;aim_row++)
        {
            for(int aim_column=0;aim_column<9;aim_column++)
            {
                if(matrix[aim_row][aim_column]==0)
                {
                    VarBlankSpacePairVec.push_back(std::make_pair(aim_row,aim_column));
                }
            }
        }

        return VarBlankSpacePairVec;
    }












