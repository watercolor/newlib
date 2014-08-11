#include <stdio.h>

extern int parse_symbol_by_addr(unsigned long , char*, int);
int main(int argc, const char *argv[])
{
    char buffer[256];
    char addrbuffer[64];
    unsigned long addr;
    int ret;
    while(fgets(addrbuffer, 64, stdin)) {
        sscanf(addrbuffer, "%lx", &addr);
        ret = parse_symbol_by_addr(addr, buffer, 256);
        if(ret > 0)
            printf("addr:0x%lx, buffer:%s\n", addr, buffer);
        buffer[0] = '\0';
    }
    return 0;
}
