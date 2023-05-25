
#include <string.h>

#include "utils.h"
#include "memory.h"

PTRLST_IMPL(str_lst, Str*)
PTRLST_IMPL(rule_lst, Rule*)
PTRLST_IMPL(pattern_lst, Pattern*)
PTRLST_IMPL(pat_elem_lst, PatElem*)

// Returns true if the string is in the str_lst. This has to
// work without disturbing the state of the actual list.
bool str_in_lst(str_lst_t* lst, const char* str) {

    for(unsigned int i = 0; i < lst->len; i++) {
        if(!strcmp(lst->list[i]->buffer, str))
            return true;
    }

    return false;
}
