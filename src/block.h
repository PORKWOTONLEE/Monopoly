#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <iostream>
#include <vector>

#include "common.h"

class block
{
    public:
        // 变量
        int no;
        std::string name;
        block_Type type;  
        std::string owner;
        std::vector<int> price;
        std::vector<int> rent;
        int level;
        // 构造函数
        block(std::vector<std::string> map_Info_Vector);
        // 变量处理函数
        void Stand_On_Event(void);
        void Pass_By_Event(void);
    protected:
        block_Type Convert_String_To_Block_Type(std::string map_Info_Vector);

};

#endif
