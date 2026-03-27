#include "scene.h"
#include "utility.inl"
#include<iostream>
#include<vector>
#include<random>
#include <algorithm>
#include<map>
#include<utility>

    std::vector<std::vector<int>> CScene::generate(int* outCycleTime)           //场景生成
{


    std::vector<std::vector<int>> matrix;
    int main_cycle_time=0;



    while(true)                                                                 //暂行的可用完整矩阵生成方法
    {
        matrix.clear();
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

    std::vector<std::pair<int,int>> FullCheck=FunBlankSpacePairVec(matrix);

    if(FullCheck.empty())
    {
        break;
    }

    main_cycle_time++;
    if(main_cycle_time>10000)
    {
        break;
    }

    }
    *outCycleTime = main_cycle_time;

    std::vector<std::pair<int,int>> erase_unit_coord=erase_unit_coord_fuc(difficulty_choose());
    matrix=erase_unit_make(erase_unit_coord,matrix);

    return matrix;
}

//-------------------------------------------------------------------------------------------------------------------------------


    std::vector<std::vector<int>> CScene::FucRealMatrMake(std::vector<std::vector<int>> matrix)
                                                                                //生成并返回完整矩阵
    {
        std::map<std::pair<int,int>,std::vector<int>> avail_num_map;
        int cycle_time_check=0;
        std::vector<std::pair<int,int>> VarBlankSpacePairVec=FunBlankSpacePairVec(matrix);
                                                                                //统计空格坐标，以pair形式存入vector中

        int NumsFilledCell=0;
        std::vector<std::vector<int>> PerFillMap;
        for (int i = 0; i < 9; i++)                                             //创建一个9x9的矩阵,用于循环校验map填充
        {
            PerFillMap.push_back(std::vector<int>(9, 1));
        }



        while(true)
        {
                    std::vector<int> avail_num={1,2,3,4,5,6,7,8,9};
                    int aim_row=VarBlankSpacePairVec[NumsFilledCell].first;
                    int aim_column=VarBlankSpacePairVec[NumsFilledCell].second;


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
                        PerFillMap[aim_row][aim_column]=1;
                        avail_num_map.erase(std::make_pair(aim_row, aim_column));
                        continue;
                    }

                    if(PerFillMap[aim_row][aim_column]==1)
                                                                                //若为第一次循环或已经回溯，则将可用数字存入map
                    {
                    avail_num_map[std::make_pair(aim_row, aim_column)] = avail_num;
                    PerFillMap[aim_row][aim_column]=0;
                    }

                    auto it = avail_num_map.find(std::pair<int,int>(aim_row,aim_column));
                    if (it != avail_num_map.end())                              // 检查key是否存在
                    {
                    std::vector<int>& vec = it->second;                         // 取vector的引用（修改原数据
                    matrix[aim_row][aim_column] = vec[0];
                    vec.erase(vec.begin());                                     //可用数字不为空，为当前方块赋值并删去已用值
                    NumsFilledCell++;
                    }

                    if(NumsFilledCell==VarBlankSpacePairVec.size())
                    {
                        break;
                    }

                    cycle_time_check++;
                    if(cycle_time_check>1000)
                    {
                        break;
                    }

        }

        return matrix;
    }


    std::vector<int> CScene::shuffle_unit()                                     //返回一个随机数列,用于种子矩阵生成
{
    std::vector<int> source={1,2,3,4,5,6,7,8,9};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(source.begin(), source.end(), g);
    return source;
}


    std::vector<std::pair<int,int>> CScene::erase_unit_coord_fuc(int erase_unit_num)
                                                                                //输入应擦除格子数量，返回一个vector,包含应擦除矩阵坐标
{
    std::vector<int> erase_source;
    for(int i=0;i<81;i++)
    {
        erase_source.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(erase_source.begin(), erase_source.end(), g);
    erase_source.resize(erase_unit_num);
    int row=0;
    int column=0;
    std::vector<std::pair<int,int>> erase_unit_coord;
    for(int i=0;i<erase_unit_num;i++)
    {
        row=erase_source[i]/9;
        column=erase_source[i]%9;
        erase_unit_coord.push_back(std::pair<int,int>(row,column));
    }
    return erase_unit_coord;
}

    int CScene::difficulty_choose()                                             //进行难度选择，返回应清除格子数量
    {
        message("选择难度");
        message("1 2 3");
        int erase_unit_num;
        int difficulty_level;
        std::cin>>difficulty_level;
        switch(difficulty_level)
        {
            case 1:
                erase_unit_num=15;
                break;

            case 2:
                erase_unit_num=25;
                break;

            case 3:
                erase_unit_num=35;
                break;
            default:
                message("输入错误 默认选择难度1");
                erase_unit_num=15;
        }
        return erase_unit_num;
    }

    std::vector<std::vector<int>> CScene::erase_unit_make(std::vector<std::pair<int,int>>erase_unit_coord,std::vector<std::vector<int>> matrix)
                                                                                //输入完整矩阵和应擦除格子数量，输出残缺矩阵
    {
        int blank_num=erase_unit_coord.size();
        for(int i=0;i<blank_num;i++)
        {
            int erase_aim_row=erase_unit_coord[i].first;
            int erase_aim_column=erase_unit_coord[i].second;
            matrix[erase_aim_row][erase_aim_column]=0;
        }
        return matrix;
    }

    std::vector<std::pair<int,int>> CScene::FunBlankSpacePairVec(std::vector<std::vector<int>> matrix)
                                                                                //统计空格坐标，以pair形式存入vector中
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

    void CScene::play(std::vector<std::vector<int>> matrix)                     //接受一个残缺矩阵，负责用户交互
    {
        char play_input;
        int point_x=0;
        int point_y=0;

        std::cout << "\033[s";
        std::cout.flush();

        while(true)
        {

        for(int i=0;i<9;i++)                                                    //打印矩阵
        {
            for(int j=0;j<9;j++)
            {
                message(matrix[i][j],0);
                message(" ",0);
            }
            message();
        }

        std::cout << "\033[" << point_y << ";" << point_x << "H";               //移动光标至计算位置
        std::cout.flush();

        std::cin>>play_input;
        switch(play_input)
        {
            case 'a':
                point_x--;
                break;

            case 'd':
                point_x++;
                break;

            case 'w':
                point_y++;
                break;

            case 's':
                point_y--;
                break;

            case 'q':
                goto play_end;

        }

        std::cout << "\033[u";
        std::cout.flush();                                                      //移动光标到初始位置
        }

        play_end:
    }

    bool CScene::game_over(std::vector<std::vector<int>> matrix)               //接受矩阵，判断是否符合结束游戏的条件
    {
        bool game_over_check=1;

        for(int i=0;i<9;i++)                                                    //检查行
        {
            std::vector<int> row_check=matrix[i];
            sort(row_check.begin(),row_check.end());
            if(!(row_check[0]==1&&row_check[1]==2&&row_check[2]==3&&row_check[3]==4&&row_check[4]==5&&row_check[5]==6&&row_check[6]==7&&row_check[7]==8&&row_check[8]==9))
            {
                game_over_check=0;
            }
            row_check.clear();
        }

        std::vector<int> column_check;
        for(int i=0;i<9;i++)                                                    //检查列
        {
            for(int j=0;j<9;j++)
            {
            column_check.push_back(matrix[j][i]);
            }
            sort(column_check.begin(),column_check.end());
            if(!(column_check[0]==1&&column_check[1]==2&&column_check[2]==3&&column_check[3]==4&&column_check[4]==5&&column_check[5]==6&&column_check[6]==7&&column_check[7]==8&&column_check[8]==9))
            {
                game_over_check=0;
            }
            column_check.clear();
        }

        std::vector<int> mini_matrix_check;                                     //检查小矩阵
        for(int i=0;i<9;i+=3)
        {
            for(int j=0;j<9;j+=3)
            {

        for(int x=i;x<i+3;x++)
        {
            for(int y=j;y<j+3;y++)
            {
                mini_matrix_check.push_back(matrix[x][y]);
            }
        }
        sort(mini_matrix_check.begin(),mini_matrix_check.end());
        if(!(mini_matrix_check[0]==1&&mini_matrix_check[1]==2&&mini_matrix_check[2]==3&&mini_matrix_check[3]==4&&mini_matrix_check[4]==5&&mini_matrix_check[5]==6&&mini_matrix_check[6]==7&&mini_matrix_check[7]==8&&mini_matrix_check[8]==9))
        {
            game_over_check=0;
        }
        mini_matrix_check.clear();
            }
        }

            return game_over_check;

    }








