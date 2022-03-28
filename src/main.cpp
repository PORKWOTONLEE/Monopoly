#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

#include <vita2d.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#include "common.h"
#include "player.h"
#include "block.h"

// 当前操作玩家
unsigned int current_Player_No;

// 骰子数
int dice_Num;

// 全局玩家vector
vector<player> player_Vector;

// 全局地图块vector
vector<block> map_Vector;

// 全局字体
vita2d_font *font;

// 全局地图素材
vita2d_texture *horizontal_Line;
vita2d_texture *verizontal_Line;

// 全局控制器
SceCtrlData pad;

enum psv_Resolution
{
    WIDTH = 960,
    HEIGHT = 544
};

void Draw_Title(void)
{
    char title_Buffer[50];
    
    sprintf(title_Buffer, "当前玩家：player%u", current_Player_No);
    vita2d_font_draw_text(font, 5, 20, RGBA8(255, 255, 255, 255), 20, title_Buffer);
    sprintf(title_Buffer, "金币数：%d", player_Vector[current_Player_No].coin);
    vita2d_font_draw_text(font, 5, 40, RGBA8(255, 255, 255, 255), 20, title_Buffer);
    sprintf(title_Buffer, "拥有房产：");
    for (int i=0; i<player_Vector[current_Player_No].own_Block.size(); ++i)
    {
       strcat(title_Buffer, map_Vector[player_Vector[current_Player_No].own_Block[i]].name.c_str());
       strcat(title_Buffer, " ");
    }
    vita2d_font_draw_text(font, 5, 60, RGBA8(255, 255, 255, 255), 20, title_Buffer);

    return;
}

void Draw_Map(void)
{
    vita2d_draw_texture(horizontal_Line, 0, 80);
    vita2d_draw_texture(verizontal_Line, 192, 80);
    vita2d_draw_texture(verizontal_Line, 384, 80);
    vita2d_draw_texture(verizontal_Line, 576, 80);
    vita2d_draw_texture(verizontal_Line, 768, 80);
    vita2d_draw_texture(horizontal_Line, 0, 272);

    return;
}

void Draw_Map_Detail(void)
{
    char map_Info_Buffer[50];
    int block_No;

    for (int i=0; i<5; ++i)
    {
        block_No = player_Vector[current_Player_No].position + i - 2;

        if (block_No < 0)
        {
            block_No += map_Vector.size();
        }
        else if (block_No >= map_Vector.size())
        {
            block_No -= map_Vector.size(); 
        }

        sprintf(map_Info_Buffer, "%d：%s", block_No, map_Vector[block_No].name.c_str());
        vita2d_font_draw_text(font, 5+i*192, 100, RGBA8(255, 255, 255, 255), 20, map_Info_Buffer);

        if (HOUSE == map_Vector[block_No].type)
        {
            sprintf(map_Info_Buffer, "房主：%s", map_Vector[block_No].owner.c_str());
            vita2d_font_draw_text(font, 5+i*192, 120, RGBA8(255, 255, 255, 255), 20, map_Info_Buffer);

            sprintf(map_Info_Buffer, "价格：%d", map_Vector[block_No].price[0]);
            vita2d_font_draw_text(font, 5+i*192, 140, RGBA8(255, 255, 255, 255), 20, map_Info_Buffer);

            sprintf(map_Info_Buffer, "租金：%d", map_Vector[block_No].rent[map_Vector[block_No].level]);
            vita2d_font_draw_text(font, 5+i*192, 160, RGBA8(255, 255, 255, 255), 20, map_Info_Buffer);
        }
    }

    return;
}

void Draw_Screen(void)
{
    Draw_Title();
    Draw_Map();
    Draw_Map_Detail();

    return;
}

void Draw_Info(char* info)
{
    vita2d_start_drawing();		
    vita2d_clear_screen();
    Draw_Screen();
    vita2d_font_draw_text(font, 50, HEIGHT/2+150, RGBA8(255, 255, 255, 255), 20, info);
    vita2d_end_drawing();
    vita2d_swap_buffers();

    return;
}

void Set_Current_Player(unsigned int player_No)
{
    if ((player_No>=0) && player_No<player_Vector.size())
    {
        current_Player_No = player_No;
    }

    return;
}

unsigned int Get_Current_Player(void)
{
    return current_Player_No;
}

// 设定玩家人数按键识别部分
void *Change_Player_Num(void *player_Num)
{
    while (!(pad.buttons & SCE_CTRL_START))
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        if (pad.buttons & SCE_CTRL_UP)
        {
            if (*(unsigned int *)player_Num<6)
            {
                *(unsigned int *)player_Num+=1;
            }
            sceKernelDelayThread(100000);
        }
        if (pad.buttons & SCE_CTRL_DOWN)
        {
            if (*(unsigned int *)player_Num > 2)
            {
                *(unsigned int *)player_Num-=1;
            }
            sceKernelDelayThread(100000);
        }
    }

    return NULL;
}

// 设定玩家人数
void Set_Player_Num(unsigned int *player_Num)
{
    char player_Num_Buffer[20];

    pthread_t t;
    pthread_create(&t, NULL, Change_Player_Num, player_Num);

    while (!(pad.buttons & SCE_CTRL_START))
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        sprintf(player_Num_Buffer, "玩家数量：%u", *player_Num);

        vita2d_start_drawing();		
        vita2d_clear_screen();
        vita2d_font_draw_text(font, WIDTH/2-100, HEIGHT/2, RGBA8(255, 255, 255, 255), 32, player_Num_Buffer);
        vita2d_end_drawing();
        vita2d_swap_buffers();
    }

    return;
}

void Create_Player(int player_Num)
{
    int i = 0;

    // 最多六名玩家
    if (player_Num>0 && player_Num<=6)
    {
        while (i < player_Num)
        {
            player player_Entity = player("player" + std::to_string(i));

            player_Vector.push_back(player_Entity);

            ++i; 
        }
    }

    return;
}

void Create_Map(void)
{
    // 读取地图文件
    std::ifstream fp("app0:/assets/map.csv");
    if (!fp)
    {
        return;
    }
    std::string line;

    // 舍弃第一行
    std::getline(fp, line);

    // 按行循环读取地图块数据
    while(std::getline(fp, line))
    {
        vector<string> map_Info_Vector;
        std::string map_Info;

        // 读取地图块数据
        std::istringstream read_line(line);
        
        while(std::getline(read_line, map_Info, ','))
        {
            map_Info_Vector.push_back(map_Info);
        }

        block block_Entity(map_Info_Vector);

        vector<string>().swap(map_Info_Vector);

        map_Vector.push_back(block_Entity);
    }

    fp.close();

    return;
}

void Set_First_Move_Player(void)
{
    Set_Current_Player((unsigned int)rand()%player_Vector.size());

    return;
}

void Dice(void)
{
    char dice_Num_Buffer[20];
    dice_Num = (unsigned int)rand()%6+1; 
    int i = 100;

    sceCtrlPeekBufferPositive(0, &pad, 1);

    Draw_Info((char *)"是否要投掷骰子？按O键确认");

    while (!(pad.buttons & SCE_CTRL_CIRCLE))
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);
    }

    sceKernelDelayThread(100000);

    while (1)
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_CIRCLE)
        {
            break;
        }

        dice_Num = (unsigned int)rand()%6+1; 

        sprintf(dice_Num_Buffer, "按O键停下 %d", dice_Num);

        Draw_Info(dice_Num_Buffer);
    }


    sprintf(dice_Num_Buffer, "骰子数值：%d", dice_Num);

    Draw_Info(dice_Num_Buffer);
    sceKernelDelayThread(1000000);

    return;
}

void Move(void)
{
    for (;dice_Num>0; --dice_Num)
    {
        Draw_Info((char *)"按O键前进");

        sceCtrlPeekBufferPositive(0, &pad, 1);

        while (!(pad.buttons & SCE_CTRL_CIRCLE))
        {
            sceCtrlPeekBufferPositive(0, &pad, 1);
        }
        sceKernelDelayThread(100000);

        ++player_Vector[current_Player_No].position;

        if (player_Vector[current_Player_No].position >= map_Vector.size())
        {
            player_Vector[current_Player_No].position -= map_Vector.size(); 
        }

        sceKernelDelayThread(100000);
    }

    return;
}

void Event(void)
{
    switch (map_Vector[player_Vector[current_Player_No].position].type)
    {
        case START:
            player_Vector[current_Player_No].coin += 2000;
            Draw_Info((char *)"站在起点，获得2000元");
            sceKernelDelayThread(1000000);
            break;
        case CHANCE:
            player_Vector[current_Player_No].coin += 2000;
            Draw_Info((char *)"彩票中奖，获得2000元");
            sceKernelDelayThread(1000000);
            break;
        case DESTINY:
            player_Vector[current_Player_No].coin -= 2000;
            Draw_Info((char *)"做变性手术，交费2000元");
            sceKernelDelayThread(1000000);
            break;
        case HOUSE:
            // 判断房子是否有主人
            if ("" != map_Vector[player_Vector[current_Player_No].position].owner)       
            {
                // 判断房子主人是否是自己
                if (map_Vector[player_Vector[current_Player_No].position].owner == player_Vector[current_Player_No].name)
                {
                    // 判断房子可否升级
                    if (map_Vector[player_Vector[current_Player_No].position].level <=4)
                    {
                        while (1)
                        {
                            Draw_Info((char *)"是否升级此栋房屋，O键确认，X键取消");

                            sceCtrlPeekBufferPositive(0, &pad, 1);

                            if (pad.buttons & SCE_CTRL_CIRCLE)
                            {
                                // 判断资金是否足够
                                if (player_Vector[current_Player_No].coin >= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level])
                                {
                                    player_Vector[current_Player_No].coin -= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level+1];
                                    ++map_Vector[player_Vector[current_Player_No].position].level;
                                    Draw_Info((char *)"升级成功");
                                    sceKernelDelayThread(1000000);
                                }
                                else
                                {
                                    Draw_Info((char *)"资金不足");
                                    sceKernelDelayThread(1000000);
                                }
                                break;
                            }
                            if (pad.buttons & SCE_CTRL_CROSS)
                            {
                                break;
                            }
                        }
                    }
                }
                else
                {
                    player_Vector[current_Player_No].coin -= map_Vector[player_Vector[current_Player_No].position].rent[map_Vector[player_Vector[current_Player_No].position].level];
                    for (int i=0; i<player_Vector.size(); ++i)
                    {
                        if (player_Vector[i].name == map_Vector[player_Vector[current_Player_No].position].name)
                        {
                            player_Vector[i].coin += map_Vector[player_Vector[current_Player_No].position].rent[map_Vector[player_Vector[current_Player_No].position].level];
                        }
                    }
                    Draw_Info((char *)"上缴房租");
                    sceKernelDelayThread(1000000);
                }
            }
            else
            {
                while (1)
                {
                    Draw_Info((char *)"是否购买此栋房屋，O键确认，X键取消");

                    sceCtrlPeekBufferPositive(0, &pad, 1);

                    if (pad.buttons & SCE_CTRL_CIRCLE)
                    {
                        // 判断资金是否足够
                        if (player_Vector[current_Player_No].coin >= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level])
                        {
                            player_Vector[current_Player_No].coin -= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level];
                            map_Vector[player_Vector[current_Player_No].position].owner = player_Vector[current_Player_No].name;
                            player_Vector[current_Player_No].own_Block.push_back(player_Vector[current_Player_No].position);
                            Draw_Info((char *)"购买成功");
                            sceKernelDelayThread(1000000);
                        }
                        else
                        {
                            Draw_Info((char *)"资金不足");
                            sceKernelDelayThread(1000000);
                        }
                        break;
                    }
                    if (pad.buttons & SCE_CTRL_CROSS)
                    {
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void Change_Player(void)
{
    ++current_Player_No;
    if (current_Player_No >= player_Vector.size())
    {
        current_Player_No -= player_Vector.size(); 
    }
}

int Is_Anyone_Lost(void)
{
    for (int i=0; i<player_Vector.size(); ++i)
    {
        if (0 >= player_Vector[i].coin)
        {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) 
{
    unsigned int player_Num = 2;

    srand((unsigned)time(NULL));

    // 渲染模组初始化
    vita2d_init();
    vita2d_set_clear_color(BLACK);

    // 加载字体
    font = vita2d_load_font_file("app0:assets/font.ttf");

    // 加载地图块素材 
    horizontal_Line = vita2d_load_PNG_file("app0:/assets/line.png");
    verizontal_Line = vita2d_load_PNG_file("app0:/assets/delim.png");

    // 控制器初始化 
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);

    Set_Player_Num(&player_Num);

    Create_Player(player_Num);
    
    Create_Map();

    Set_First_Move_Player();

    while (1)
    {
        Dice();

        Move();

        Event();

        Change_Player();

        if (Is_Anyone_Lost())
        {
            break;
        }
    }

    // 渲染模组释放
    vita2d_fini();

    vita2d_free_font(font);
    vita2d_free_texture(verizontal_Line);
    vita2d_free_texture(horizontal_Line);

    sceKernelExitProcess(0);

    return 0;
}

