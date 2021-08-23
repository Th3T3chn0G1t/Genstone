#include "include/gentooling.h"

gen_tooling_stack_t gen_tooling_call_stack = { 0, { 0 }, { 0 } };

gen_tooling_stack_push_handler_t gen_tooling_push_handler = NULL;
gen_tooling_stack_pop_handler_t gen_tooling_pop_handler = NULL;

void gen_tooling_stack_push(const char* restrict frame, const char* restrict file) {
    if(gen_tooling_push_handler) gen_tooling_push_handler();
    gen_tooling_call_stack.stack[gen_tooling_call_stack.next] = frame;
    gen_tooling_call_stack.files[gen_tooling_call_stack.next] = file;
    ++gen_tooling_call_stack.next;
}

void gen_tooling_stack_pop(void) {
    if(gen_tooling_pop_handler) gen_tooling_pop_handler();
    --gen_tooling_call_stack.next;
}

void gen_internal_tooling_frame_scope_end(__unused const char* const restrict passthrough) {
    gen_tooling_stack_pop();
}
