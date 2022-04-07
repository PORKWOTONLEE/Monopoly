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
#include "render.h"
#include "log.h"

extern vita2d_font *font;

// 当前操作玩家
int current_Player_No;

// 当前骰子数
int dice_Num;
char dice_Num_Buffer[20];

// 全局玩家vector
vector<player> player_Vector;

// 全局地图块vector
vector<block> map_Vector;

// 全局渲染类
render draw; 

// 全局控制器
SceCtrlData pad;

void Set_Current_Player(int player_No)
{
    if ((player_No>=0) && player_No<player_Vector.size())
    {
        current_Player_No = player_No;
    }

    return;
}

int Get_Current_Player(void)
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
            if (*(int *)player_Num<6)
            {
                *(int *)player_Num+=1;
            }
            sceKernelDelayThread(100000);
        }
        if (pad.buttons & SCE_CTRL_DOWN)
        {
            if (*(int *)player_Num > 2)
            {
                *(int *)player_Num-=1;
            }
            sceKernelDelayThread(100000);
        }
    }

    return NULL;
}

void *Change_Dice_Num(void *buffer)
{
    sceCtrlPeekBufferPositive(0, &pad, 1);
    while (!(pad.buttons & SCE_CTRL_CIRCLE))
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        dice_Num = (int)rand()%6+1; 

        sprintf(dice_Num_Buffer, "按O键停下 %d", dice_Num);
    }

    return NULL;
}

// 设定玩家人数
void Set_Player_Num(int *player_Num)
{
    char player_Num_Buffer[20];

    Log_Info("Set Player Num\n");

    pthread_t t;
    pthread_create(&t, NULL, Change_Player_Num, player_Num);

    while (!(pad.buttons & SCE_CTRL_START))
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);

        sprintf(player_Num_Buffer, " 玩家数量：%d", *player_Num);

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

    Log_Info("Player Data Init: Total Number Of Player %d\n", player_Num);

    return;
}

void Create_Map(void)
{
    char* word;
    char line[100];
    FILE *fp; 

    // 读取地图文件
    Log_Debug("Open Map File\n");
    fp = fopen("app0:/assets/map.csv", "r");
    if (!fp)
    {
        return;
    }

    // 舍弃第一行
    fgets(line, sizeof(line), fp);

    // 按行循环读取地图块数据
    Log_Debug("Load Map Data\n");
    while(fgets(line, sizeof(line), fp))
    {
        vector<string> map_Info_Vector;

        strcpy(&line[strlen(line)-1], "\0");

        // 读取地图块数据
        word = strtok(line, ",");
        
        while(NULL != word)
        {
            map_Info_Vector.push_back(word);
            word = strtok(NULL, ",");
        }

        block block_Entity(map_Info_Vector);

        map_Vector.push_back(block_Entity);
    }

    Log_Info("Map Data Init: Total Number Of Map Block %d\n", map_Vector.size());

    fclose(fp);

    return;
}

void Set_First_Move_Player(void)
{
    Log_Debug("Set First Player\n");

    Set_Current_Player((int)rand()%player_Vector.size());

    return;
}

void Dice(void)
{
    pthread_t t;

    dice_Num = (int)rand()%6+1; 

    sceCtrlPeekBufferPositive(0, &pad, 1);

    draw.Screen_With_Confirm(SCE_CTRL_CIRCLE, "是否要投掷骰子？按O键确认");

    pthread_create(&t, NULL, Change_Dice_Num, NULL);

    draw.Screen_With_Confirm(SCE_CTRL_CIRCLE, dice_Num_Buffer);

    draw.Screen("骰子数值：%d", dice_Num);
    sceKernelDelayThread(1000000);

    return;
}

void Move(void)
{
    for (;dice_Num>0; --dice_Num)
    {
        draw.Screen_With_Confirm(SCE_CTRL_CIRCLE, "按O键前进，剩余步数 %d", dice_Num);

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
            draw.Screen("站在起点，获得2000元");
            sceKernelDelayThread(1000000);
            break;
        case CHANCE:
            player_Vector[current_Player_No].coin += 2000;
            draw.Screen("彩票中奖，获得2000元");
            sceKernelDelayThread(1000000);
            break;
        case DESTINY:
            player_Vector[current_Player_No].coin -= 2000;
            draw.Screen("做变性手术，交费2000元");
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
                    if (map_Vector[player_Vector[current_Player_No].position].level <=3)
                    {
                        while (1)
                        {
                            draw.Screen("是否升级此栋房屋，O键确认，X键取消");

                            sceCtrlPeekBufferPositive(0, &pad, 1);

                            if (pad.buttons & SCE_CTRL_CIRCLE)
                            {
                                // 判断资金是否足够
                                if (player_Vector[current_Player_No].coin >= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level])
                                {
                                    player_Vector[current_Player_No].coin -= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level+1];
                                    ++map_Vector[player_Vector[current_Player_No].position].level;
                                    draw.Screen("升级成功");
                                    sceKernelDelayThread(1000000);
                                }
                                else
                                {
                                    draw.Screen("资金不足");
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
                    draw.Screen("上缴房租");
                    sceKernelDelayThread(1000000);
                }
            }
            else
            {
                while (1)
                {
                    draw.Screen("是否购买此栋房屋，O键确认，X键取消");

                    sceCtrlPeekBufferPositive(0, &pad, 1);

                    if (pad.buttons & SCE_CTRL_CIRCLE)
                    {
                        // 判断资金是否足够
                        if (player_Vector[current_Player_No].coin >= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level])
                        {
                            player_Vector[current_Player_No].coin -= map_Vector[player_Vector[current_Player_No].position].price[map_Vector[player_Vector[current_Player_No].position].level];
                            map_Vector[player_Vector[current_Player_No].position].owner = player_Vector[current_Player_No].name;
                            player_Vector[current_Player_No].own_Block.push_back(player_Vector[current_Player_No].position);
                            draw.Screen("购买成功");
                            sceKernelDelayThread(1000000);
                        }
                        else
                        {
                            draw.Screen("资金不足");
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
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) 
{
    int player_Num = 2;

    Log_Open(LOG_PATH);

    Log_Info("Random Mod Init\n");
    srand((unsigned)time(NULL));

    // 控制器初始化 
    Log_Info("Control Mod Init\n");
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);

    Set_Player_Num(&player_Num);

    Create_Player(player_Num);
    
    Create_Map();

    Set_First_Move_Player();

    while (Is_Anyone_Lost())
    {
        Dice();

        Move();

        Event();

        Change_Player();
    }

    sceKernelExitProcess(0);

    return 0;
}

