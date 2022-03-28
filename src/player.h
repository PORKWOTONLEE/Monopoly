#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>

#include "common.h"

using namespace std;

class player
{
    public:
        // 变量
        string name;
        int coin;
        vector<int> own_Block;
        int position;
        // 变量处理函数
        player(string name);
        void Add_Own_Block(int block_No);
        void Delete_Own_Block(int block_No);
        void Own_Block_Init(void);
};

#endif
