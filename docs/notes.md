
# Thinking about parser generators

A state machine can be constructed by identifying the state changes. A state change is where one token can follow another. This requires the driver to trace through every possible parse of the grammar. If there is a duplicate parse, then that is a parser conflict. Every possible point at which a parse makes a decision is a state. Conflicts are resolved by ordering the rules.

~~Stack based parser. States are kept on a stack instead of a state machine. Look at it like a stack based interpreter that translates the grammar into C code.~~

- First invent a way to implement a state machine from tables.
- The state machine must run arbitrary code at arbitrary states.
- The state machine only needs to use numbers to describe states.



# Thinking code

Emitted code to parse a single rule.

```C
/*
    Rules
    // Simple rule that just accepts simple patterns
    plaster
        : bacon {}        : eggs {}
        : thing {}

    // The recursive rule accepts bacon, eggs or thing as an element.
    plaster_lst
        : plaster {}
        : plaster COMMA plaster_lst {}

    Data structures
    PTRLST_HEADER(plaster_lst, plaster_t*)
    typedef struct _plaster_ plaster_t;
    struct _plaster_ {
        bacon_t* m_bacon;
        eggs_t* m_eggs;
        plaster_lst_t* m_plaster_lst;
    };
*/

bool plaster_pat1(plaster_t* ptr) {

// If there is only one element in the pattern, then this is redundant
// however, it does simplify code generation.
    if(NULL != (ptr->bacon = bacon())) {
        return true;
    }

    return false;
}

bool plaster_pat2(plaster_t* ptr) {

    if(NULL != (ptr->eggs = eggs())) {
        return true;
    }

    return false;
}

bool plaster_pat3(plaster_t* ptr) {

    if(NULL != (ptr->thing = thing())) {
        return true;
    }

    return false;
}

static plaster_t* plaster() {

    plaster_t* tmp = _alloc_obj(plaster_t);

    if(plaster_pat1(tmp)) {
        // run the pattern code
        return tmp;
    }
    else if(plaster_pat2(tmp)) {
        // run the pattern code
        return tmp;
    }
    else if(plaster_pat3(tmp)) {
        // run the pattern code
        return tmp;
    }
    else {
        syntax_error("expected a plaster but got a %s", to_name(tmp));
        return NULL;
    }
}

static bool plaster_lst_pat1(plaster_lst_t* ptr) {

    plaster_t* tmp;
    if(NULL != (tmp = plaster())) {
        plaster_lst_add(ptr, tmp);
        consume_token();
        if(token_type == COLON)
            plaster_lst();
        else
        return true;
    }
}

static bool plaster_lst_pat2(plaster_lst_t* ptr) {

}

static plaster_lst_t* plaster_lst() {

    plaster_lst_t* ptr = _alloc_obj(plaster_lst_t);

    if(plaster_lst_pat1(ptr)) {
        // pattern code
        return ptr;
    }
    else if(plaster_lst_pat2(ptr)) {
        // pattern code
        return ptr;
    }
    else {
        syntax_error("expected but got ...");
        return NULL
    }
}

```