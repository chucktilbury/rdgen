#ifndef _UTILS_H
#define _UTILS_H

#include <stdbool.h>
#include "ptrlst.h"
#include "strs.h"

typedef struct pattern_t Pattern;
typedef struct pat_elem_t PatElem;
typedef struct rule_t Rule;

PTRLST_HEADER(str_lst, Str*)
PTRLST_HEADER(rule_lst, Rule*)
PTRLST_HEADER(pattern_lst, Pattern*)
PTRLST_HEADER(pat_elem_lst, PatElem*)

bool str_in_lst(str_lst_t* lst, const char* str);
Str* header_name(Str* name);
Str* source_name(Str* name);

#include "parser.h"

#endif /* _UTILS_H */
