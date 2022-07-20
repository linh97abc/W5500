
#include <stdio.h>

extern void wiz_lowlevel_setup(void);

int main()
{
    wiz_lowlevel_setup();
    puts(__func__);
    return 0;
}
