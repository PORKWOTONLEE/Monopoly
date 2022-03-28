#include "player.h"

player::player(string name)
{
    player::name = name;
    player::coin = INIT_COIN;
    player::Own_Block_Init();
    player::position = 0;
}

void player::Add_Own_Block(int block_No)
{
    // 检查传入地图块编号是否存在
    for (auto it=player::own_Block.begin(); it!=player::own_Block.end(); ++it) 
    {
        if (block_No == *it) 
        {
            return;
        }
    }

    player::own_Block.push_back(block_No);
}

void player::Delete_Own_Block(int block_No)
{
    // 检查传入地图块编号是否存在
    for (auto it=player::own_Block.begin(); it!=player::own_Block.end(); ++it) 
    {
        if (block_No == *it)
        {
            player::own_Block.erase(it);
        }
    }

    return;
}

void player::Own_Block_Init(void)
{
    // 检查传入地图块编号是否存在
    for (;player::own_Block.size()>0;) 
    {
        player::own_Block.pop_back();
    }

    return;
}

