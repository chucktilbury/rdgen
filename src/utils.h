#ifndef _UTILS_H
#define _UTILS_H

#include "ptrlst.h"
#include "strs.h"
#include <stdbool.h>

typedef struct pattern_t Pattern;
typedef struct pat_elem_t PatElem;
typedef struct rule_t Rule;

PTRLST_HEADER(str_lst, STR)
PTRLST_HEADER(rule_lst, Rule)
PTRLST_HEADER(pattern_lst, Pattern)
PTRLST_HEADER(pat_elem_lst, PatElem)

#include "list.h"

bool str_in_lst(str_lst_t* lst, const char* str);
STR header_name(STR name);
STR source_name(STR name);

#include "parser.h"

#endif /* _UTILS_H */
