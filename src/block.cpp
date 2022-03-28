#include "block.h"

block_Type block::Convert_String_To_Block_Type(std::string map_Info_Vector)
{
    if ("S" ==map_Info_Vector)
    {
        return START;
    }
    else if ("C" == map_Info_Vector)
    {
        return CHANCE;
    }
    else if ("D" == map_Info_Vector)
    {
        return DESTINY;
    }
    else if ("H" == map_Info_Vector)
    {
        return HOUSE;
    }

    return START;
}

block::block(std::vector<std::string> map_Info_Vector)
{
    block::no = atoi(map_Info_Vector[0].c_str()); 
    block::name = map_Info_Vector[1];
    block::type = Convert_String_To_Block_Type(map_Info_Vector[2]);

    if (map_Info_Vector.size() > 3)     
    {
        switch((int)block::type)
        {
            case START:
            case CHANCE:
            case DESTINY:
                break;
            case HOUSE:
                // 读取
                for (int i=0; i<4; ++i)
                {
                    block::price[i] = atoi(map_Info_Vector[i+3].c_str());
                }
                for (int i=0; i<4; ++i)
                {
                    block::rent[i] = atoi(map_Info_Vector[i+7].c_str());
                }
                block::level = 0;
                break;
            default:
                break;
        }
    }
}
