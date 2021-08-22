#include "include/gentooling.h"

gen_tooling_stack_t gen_tooling_call_stack = { 0, { 0 }, { 0 } };

gen_tooling_stack_push_handler_t gen_tooling_push_handler = NULL;
gen_tooling_stack_pop_handler_t gen_tooling_pop_handler = NULL;

void gen_tooling_stack_push(gen_tooling_stack_t* const restrict call_stack, const char* restrict frame, const char* restrict file) {
    if(gen_tooling_push_handler) gen_tooling_push_handler(call_stack);
    call_stack->stack[call_stack->next] = frame;
    call_stack->files[call_stack->next] = file;
    ++call_stack->next;

    GEN_FRAME_END;
}

void gen_tooling_stack_pop(gen_tooling_stack_t* const restrict call_stack) {
    if(gen_tooling_pop_handler) gen_tooling_pop_handler(call_stack);
    --call_stack->next;
}
