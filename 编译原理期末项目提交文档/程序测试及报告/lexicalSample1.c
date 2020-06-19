/* 
*   MiniC程序词法分析测试用例 
*/                      /*注释符号*/

int max(int x, int y)   /*“,”专用符号*/
{
    if(x > y)
        return x;
    else 
        return y;
}

int main(void)         /*( )专用符号,void关键字，ID标记*/
{                       /*{专用符号*/
    int array[10];      /*[ ]专用符号*/
    int x ;          /*int关键字，ID标记，=专用符号，“；”专用符号*/
    int X ;          /* ID标记*/
    int y ;          /*NUM其他标记*/
    x = 10;
    y = 6;
    X = 1;
    while( x > 0)       /*while关键字，>专用符号*/
    {  
        x = x - 1;      /* -专用符号*/
    }
    if(x >= y)          /*if关键字，>=专用符号*/
        x = x + 1;      /* +专用符号*/
    if(x < y)           /* <专用符号*/
        x = x * y;      /* *专用符号*/
    if(x <= y)          /* <=专用符号*/
        x = x / y;      /* /专用符号*/
    if(x != y)          /* !=专用符号*/
        x = x - y;      /* -专用符号*/
    if (x == y)       /* ==专用符号*/
        return x;       /*return关键字*/
    else                /*else专用符号*/
        return y;
}                       /*}专用符号*/