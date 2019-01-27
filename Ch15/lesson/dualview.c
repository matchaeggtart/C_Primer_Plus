
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

/* line styles     */
#define SOLID   0
#define DOTTED  1
#define DASHED  2
/* three colors */
#define BLUE    4                       // 0100
#define GREEN   2                       // 0010
#define RED     1                       // 0001
/* secondary colours */
#define BLACK   0
#define YELLOW  (RED | GREEN)           // 0001 | 0010 = 0011
#define MAGENTA (RED | BLUE)            // 0001 | 0100 = 0101
#define CYAN    (GREEN | BLUE)          // 0010 | 0100 = 0110
#define WHITE   (RED | GREEN | BLUE)    // 0001 | 0010 | 0100 = 0111

#define OPAQUE          0x1             //              ,000,1
#define FILL_BLUE       0x8             //              ,100,0
#define FILL_GREEN      0x4             //              ,010,0
#define FILL_RED        0x2             //              ,001,0
#define FILL_MASK       0xE             //              ,111,0
#define BORDER          0x100           //       ,1,0000,000,0
#define BORDER_BLUE     0x800           //   ,100,0,0000,000,0
#define BORDER_GREEN    0x400           //   ,010,0,0000,000,0
#define BORDER_RED      0x200           //   ,001,0,0000,000,0
#define BORDER_MASK     0xE00           //   ,111,0,0000,000,0
#define B_SOLID         0               // 00,000,0,0000,000,0
#define B_DOTTED        0x1000          // 01,000,0,0000,000,0
#define B_DASHED        0x2000          // 10,000,0,0000,000,0
#define STYLE_MASK      0x3000          // 11,000,0,0000,000,0

const char * colors[8] = { "black", "red", "green", "yellow", "blue", "magenta",
"cyan", "white" };

struct box_props {
    bool opaque :               1;
    unsigned int fill_color :   3;
    unsigned int :              4;
    bool show_border :          1;
    unsigned int border_color : 3;
    unsigned int border_style : 2;
    unsigned int :              2;
};

union Views 
{
    struct box_props st_view;
    unsigned short us_view;
};

void show_settings(const struct box_props * pb);
void show_settings1(unsigned short);
char * itobs(int n, char *ps);

int main(void)
{
    union Views box = { { true, YELLOW, true, GREEN, DASHED } };
    /*
                          1     011     1     010    10
                          >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                          DASHED, GREEN, true, YELLOW, true
                          10      010    1     011     1
                        0010      010    10000 011     1  
    */
    
//  char bin_str[8 * sizeof(unsigned short) + 1];
    char bin_str[8 * sizeof(unsigned int) + 1];
    printf("sizeof unsigned short = %zd\n", sizeof(unsigned short));
    printf("sizeof unsigned int   = %zd\n", sizeof(unsigned int));


    printf("Original box settings:\n");
    show_settings(&box.st_view);

    printf("\nBox settings using unsigned int view:\n");
    show_settings1(box.us_view);

    printf("bits are %s\n",
        itobs(box.us_view, bin_str));

    box.us_view &= ~FILL_MASK;                  //  11,111,1,1111,000,1 = ~00,000,0,0000,111,0
    printf("bits are %s\n",
    itobs(box.us_view, bin_str));               //&(10,010,1,0000,011,1)
                                                //=(10,010,1,0000,000,1)
    box.us_view |= (FILL_BLUE | FILL_GREEN);    //                110,0 = (1000 | 0100)
                                                //|(10,010,1,0000,000,1)
                                                //=(10,010,1,0000,110,1)
    box.us_view ^= OPAQUE;                      //                    1
                                                //^(10,010,1,0000,110,1)
                                                //=(10,010,1,0000,110,0)                                                       
    box.us_view |= BORDER_RED;                  //    ,001,0,0000,000,0
                                                //|(10,010,1,0000,110,0)
                                                //=(10,011,1,0000,110,0)
    box.us_view &= ~STYLE_MASK;                 //  00,111,1,1111,111,1 = ~11,000,0,0000,000,0
                                                //&(10,011,1,0000,110,0)
                                                //=(00,011,1,0000,110,0)            
    box.us_view |= B_DOTTED;                    //  01,000,0,0000,000,0 
                                                //|(00,011,1,0000,110,0)
                                                //=(01,011,1,0000,110,0)                 

    printf("\nModified box settings:\n");
    show_settings(&box.st_view);
    printf("\nBox settings using unsigned int view:\n");
    show_settings1(box.us_view);
    printf("bits are %s\n",
       itobs(box.us_view, bin_str));
    printf("bits are %s\n",
       itobs((box.us_view>>1) & 07, bin_str));

    return 0;

}

void show_settings(const struct box_props * pb)
{
    printf("Box is %s.\n",
        pb->opaque == true ? "opaque" : "transparent");
    printf("The fill color is %s.\n", colors[pb->fill_color]);
    printf("Border %s.\n",
        pb->show_border == true ? "shown" : "not shown");
    printf("The border color is %s.\n", colors[pb->border_color]);
    printf("The border style is ");
    switch (pb->border_style)
    {
        case SOLID: printf("solid.\n"); break;
        case DOTTED:printf("dotted.\n");break;
        case DASHED:printf("dashed.\n");break;
        default:    printf("unknow type.\n");
    }
}

void show_settings1(unsigned short us)
{
    printf("box is %s.\n",
        (us & OPAQUE) == OPAQUE ? "opaque" : "transparent");
    printf("The fill color is %s.\n",
        colors[(us >> 1) & 07]);                //  01,001,0,1000,001,1 = (10,010,1,0000,011,1) >> 1
                                                //& 00,000,0,0000,011,1
                                                //= 00,000,0,0000,001,1 = 0011 = 3 = green 
    printf("Border %s.\n",
        (us & BORDER) == BORDER ? "shown" : "not shown");
    printf("The border style is ");
    switch (us & STYLE_MASK)
    {
        case B_SOLID    : printf("solid.\n");   break;
        case B_DOTTED   : printf("dotted.\n");  break;
        case B_DASHED   : printf("dashed.\n");  break;
        default         : printf("unkown type.\n");
    }
    printf("The border color is %s.\n",
        colors[(us >> 9) & 07]);
}

char * itobs(int n, char *ps)
{
    int i;
    const static int size = CHAR_BIT * sizeof(int);

    for (i = size - 1; i >= 0; i--, n >>= 1)
        ps[i] = (01 & n) + '0';
    ps[size] = '\0';

    return ps;
}

/*
printf("\n\ntest1\n");
//show_settings(&box.st_view);
show_settings1(box.us_view);
printf("bits are %s\n",
    itobs(box.us_view, bin_str));
printf("\n\n");
*/