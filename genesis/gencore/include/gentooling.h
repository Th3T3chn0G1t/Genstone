#include "gencommon.h"

#ifndef GEN_TOOLING_DEPTH
#define GEN_TOOLING_DEPTH 64
#endif

typedef struct {
    size_t next;
    const char* stack[GEN_TOOLING_DEPTH];
    const char* files[GEN_TOOLING_DEPTH];
} gen_tooling_stack_t;

typedef void (*gen_tooling_stack_push_handler_t) (gen_tooling_stack_t* const restrict);
typedef void (*gen_tooling_stack_pop_handler_t) (gen_tooling_stack_t* const restrict);

extern gen_tooling_stack_t gen_tooling_call_stack;

extern gen_tooling_stack_push_handler_t gen_tooling_push_handler;
extern gen_tooling_stack_pop_handler_t gen_tooling_pop_handler;

extern void gen_tooling_stack_push(gen_tooling_stack_t* const restrict call_stack, const char* restrict frame, const char* restrict file);
extern void gen_tooling_stack_pop(gen_tooling_stack_t* const restrict call_stack);

#define GEN_FRAME_BEGIN gen_tooling_stack_push(&gen_tooling_call_stack, __func__, __FILE__)
#define GEN_FRAME_END gen_tooling_stack_pop(&gen_tooling_call_stack)

#define GEN_LOGGER_TRACE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Trace: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

/**
 * Outputs trace information
 */
#define gtrace \
    GEN_FOREACH(i, trace, gen_tooling_call_stack.next, gen_tooling_call_stack.stack) \
        printf("%s%s() %s\n", GEN_LOGGER_TRACE_PREFIX, trace, gen_tooling_call_stack.files[i]) \
