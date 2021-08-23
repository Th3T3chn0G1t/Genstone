#include "gencommon.h"

#ifndef GEN_TOOLING_DEPTH
#define GEN_TOOLING_DEPTH 64
#endif

typedef struct {
    size_t next;
    const char* stack[GEN_TOOLING_DEPTH];
    const char* files[GEN_TOOLING_DEPTH];
} gen_tooling_stack_t;

typedef void (*gen_tooling_stack_push_handler_t) (void);
typedef void (*gen_tooling_stack_pop_handler_t) (void);

extern gen_tooling_stack_t gen_tooling_call_stack;

extern gen_tooling_stack_push_handler_t gen_tooling_push_handler;
extern gen_tooling_stack_pop_handler_t gen_tooling_pop_handler;

extern void gen_tooling_stack_push(const char* restrict frame, const char* restrict file);
extern void gen_tooling_stack_pop(void);

extern void gen_internal_tooling_frame_scope_end(const char* const restrict passthrough);

#define GEN_FRAME_BEGIN \
    __attribute__((cleanup (gen_internal_tooling_frame_scope_end))) __unused const char __frame_scope_tmpvar; \
    gen_tooling_stack_push(__func__, __FILE__)

#define GEN_LOGGER_TRACE_PREFIX GEN_ANSI_COLOR_LIGHT(GEN_ANSI_GRAY) GEN_ANSI_SEQUENCE(GEN_ANSI_BOLD) "Trace: " GEN_ANSI_SEQUENCE(GEN_ANSI_CLEAR)

/**
 * Outputs backtrace information
 */
#define gtrace \
    GEN_FOREACH(i, trace, gen_tooling_call_stack.next, gen_tooling_call_stack.stack) \
        printf("%s%s() %s\n", GEN_LOGGER_TRACE_PREFIX, trace, gen_tooling_call_stack.files[i]) \
