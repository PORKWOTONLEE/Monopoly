#ifndef __RENDER_H__
#define __RENDER_H__

class render
{
    public:
        render(void);
        ~render(void);
        void Screen(const char* info, ...);
        void Screen_With_Confirm(int confirm_Button, const char *info, ...);
    protected:
        void Title(void);
        void Map_Detail(void);
        void Map(void);
        void Info(const char *info);
};

#endif
