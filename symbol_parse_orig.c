#include <stdlib.h>
#include <stdio.h>

typedef struct{
    unsigned long sym_address;
    const char* sym_name;
    int type;
}symbol_info;

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

extern symbol_info symbol_array[];
extern int symbol_array_size;

int symbol_addr_cmp(const void*search, const void* element)
{
    unsigned long search_address = *(unsigned long*)search;
    const symbol_info *info = (symbol_info*)element;
    const symbol_info *next = info + 1;
    if(search_address >= info->sym_address && search_address < next->sym_address)
        return 0;
    return search_address - info->sym_address;
}

int parse_symbol_by_addr(unsigned long address, char * buffer, int buflen)
{
    symbol_info* p_symbol_info;
    int offset;
    int len;
    p_symbol_info = (symbol_info*)bsearch(&address, symbol_array, symbol_array_size,
                            sizeof(symbol_info), symbol_addr_cmp);
    if(p_symbol_info == NULL)
        return 0;
    offset = address - p_symbol_info->sym_address;
    len = snprintf(buffer, buflen, "%s+0x%x/%d", p_symbol_info->sym_name, offset, offset);
    buffer[buflen - 1] = '\0';
    return len;
}

