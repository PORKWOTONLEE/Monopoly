#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <vita2d.h>
#include <vector>

#include "common.h"
#include "player.h"
#include "block.h"
#include "log.h"
#include "render.h"

extern int current_Player_No;
extern std::vector<player> player_Vector;
extern std::vector<block> map_Vector;
extern SceCtrlData pad;

// 全局字体
vita2d_font *font;

// 全局地图素材
vita2d_texture *horizontal_Line;
vita2d_texture *verizontal_Line;

render::render(void)
{
    // 渲染模组初始化
    Log_Debug("Vita2D Mod Init\n");
    vita2d_init();
    vita2d_set_clear_color(BLACK);

    // 加载字体
    Log_Debug("Load Font\n");
    font = vita2d_load_font_file("app0:assets/font.ttf");

    // 加载地图块素材 
    Log_Debug("Load Source\n");
    horizontal_Line = vita2d_load_PNG_file("app0:/assets/line.png");
    verizontal_Line = vita2d_load_PNG_file("app0:/assets/delim.png");
}

render::~render(void)
{
    // 渲染模组释放
    vita2d_fini();

    // 字体释放
    vita2d_free_font(font);

    // 地图素材释放
    vita2d_free_texture(verizontal_Line);
    vita2d_free_texture(horizontal_Line);
}

void render::Title(void)
{
    char title_Buffer[50];
    
    sprintf(title_Buffer, "当前玩家：player%d", current_Player_No);
    vita2d_font_draw_text(font, 5, 20, WHITE, 20, title_Buffer);
    sprintf(title_Buffer, "金币数：%d", player_Vector[current_Player_No].coin);
    vita2d_font_draw_text(font, 5, 40, WHITE, 20, title_Buffer);
    sprintf(title_Buffer, "拥有房产：");
    for (int i=0; i<player_Vector[current_Player_No].own_Block.size(); ++i)
    {
       strcat(title_Buffer, map_Vector[player_Vector[current_Player_No].own_Block[i]].name.c_str());
       strcat(title_Buffer, " ");
    }
    vita2d_font_draw_text(font, 5, 60, WHITE, 20, title_Buffer);

    return;
}

void render::Map(void)
{
    vita2d_draw_texture(horizontal_Line, 0, 80);
    vita2d_draw_texture(verizontal_Line, 192, 80);
    vita2d_draw_texture(verizontal_Line, 384, 80);
    vita2d_draw_texture(verizontal_Line, 576, 80);
    vita2d_draw_texture(verizontal_Line, 768, 80);
    vita2d_draw_texture(horizontal_Line, 0, 272);

    return;
}

void render::Map_Detail(void)
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
        vita2d_font_draw_text(font, 5+i*192, 100, WHITE, 20, map_Info_Buffer);

        if (HOUSE == map_Vector[block_No].type)
        {
            sprintf(map_Info_Buffer, "房主：%s", map_Vector[block_No].owner.c_str());
            vita2d_font_draw_text(font, 5+i*192, 120, WHITE, 20, map_Info_Buffer);

            sprintf(map_Info_Buffer, "价格：%d", map_Vector[block_No].price[0]);
            vita2d_font_draw_text(font, 5+i*192, 140, WHITE, 20, map_Info_Buffer);

            sprintf(map_Info_Buffer, "租金：%d", map_Vector[block_No].rent[map_Vector[block_No].level]);
            vita2d_font_draw_text(font, 5+i*192, 160, WHITE, 20, map_Info_Buffer);
        }
    }

    return;
}

void render::Info(const char* info)
{
    if (NULL != info)
    {
        vita2d_font_draw_text(font, 50, HEIGHT/2+150, WHITE, 20, info);
    }

    return;
}

void render::Screen(const char* info, ...)
{
    va_list parg;
    char buffer[1024];

    vita2d_start_drawing();		
    vita2d_clear_screen();
    render::Title();
    render::Map();
    render::Map_Detail();

    // 格式化输入
    va_start(parg, info);
    vsprintf(buffer, info, parg);
    render::Info(buffer);
    va_end(parg);

    vita2d_end_drawing();
    vita2d_swap_buffers();

    return;
}

void render::Screen_With_Confirm(int confirm_Button, const char *info, ...)
{
    va_list parg;
    char buffer[1024];

    sceCtrlPeekBufferPositive(0, &pad, 1);
    while (!(pad.buttons & confirm_Button))
    {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        
        va_start(parg, info);
        vsprintf(buffer, info, parg);
        render::Screen(buffer);
        va_end(parg);
    }
    sceKernelDelayThread(100000);

    return;
}
