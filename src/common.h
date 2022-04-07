#ifndef __COMMON_H__
#define __COMMON_H__

#define INIT_COIN 15000

#define BLACK RGBA8(0, 0, 0, 0xFF)
#define WHITE RGBA8(0xFF, 0xFF, 0xFF, 0xFF)

typedef enum
{
    START, 
    CHANCE, 
    DESTINY, 
    HOUSE

}block_Type;

enum psv_Resolution
{
    WIDTH = 960,
    HEIGHT = 544
};

#endif
