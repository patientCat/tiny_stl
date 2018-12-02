//c++ program
//design by >^.^<
//@date    2018-11-16 22:41:29

#include <iostream> 

int main()
{
    int x = 0x12345678;
    char * a = (char *)&x;
    printf("%#x", *a);
    return 0;
}
