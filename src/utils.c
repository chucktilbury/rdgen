
#include <string.h>

#include "errors.h"
#include "memory.h"
#include "utils.h"

PTRLST_IMPL(str_lst, STR)
PTRLST_IMPL(rule_lst, Rule)
PTRLST_IMPL(pattern_lst, Pattern)
PTRLST_IMPL(pat_elem_lst, PatElem)

// Returns true if the string is in the str_lst. This has to
// work without disturbing the state of the actual list.
bool str_in_lst(str_lst_t* lst, const char* str) {

    for(unsigned int i = 0; i < str_lst_len(lst); i++) {
        if(!strcmp(raw_str(str_lst_get(lst, i)), str))
            return true;
    }

    return false;
}

STR header_name(STR name) {

    STR tmp = copy_str(name);
    if(len_str(tmp) == 0)
        FATAL("header file name does not exist");
    cat_str_str(tmp, ".h");
    return tmp;
}

STR source_name(STR name) {

    STR tmp = copy_str(name);
    if(len_str(tmp) == 0)
        FATAL("source file name does not exist");
    cat_str_str(tmp, ".c");
    return tmp;
}
