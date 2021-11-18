#include <new>
#include <mimalloc.h>

#include "include/gepp.hpp"

#define GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY \
    do { \
        gen_error_t error = gfree(ptr); \
        if(error) glogf(FATAL, "`delete` failed on pointer %p - %s: %s", ptr, gen_error_name(error), gen_error_description(error)); \
    } while(0)
#define GEN_INTERNAL_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY \
    do { \
        void* outp = NULL; \
        gen_error_t error = gzalloc(&outp, size, sizeof(char)); \
        if(error) glogf(FATAL, "`new` failed on size %zu - %s: %s", size, gen_error_name(error), gen_error_description(error)); \
        return outp; \
    } while(0)
#define GEN_INTERNAL_ALIGNED_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY \
    do { \
        void* outp = NULL; \
        gen_error_t error = gzalloc_aligned(&outp, size, sizeof(char), alignment); \
        if(error) glogf(FATAL, "`new` failed on size %zu - %s: %s", size, gen_error_name(error), gen_error_description(error)); \
        return outp; \
    } while(0)

void operator delete(void* const ptr) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }
void operator delete[](void* const ptr) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }

void* operator new(const size_t size) { GEN_INTERNAL_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }
void* operator new[](const size_t size) { GEN_INTERNAL_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }

void* operator new(const size_t size, __unused const std::nothrow_t& const tag) noexcept { GEN_INTERNAL_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }
void* operator new[](const size_t size, __unused const std::nothrow_t& const tag) noexcept { GEN_INTERNAL_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }

void operator delete(void* const ptr, __unused const size_t size) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }
void operator delete[](void* const ptr, __unused const size_t size) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }

void operator delete(void* const ptr, __unused const size_t alignment) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }
void operator delete[](void* const ptr, __unused const size_t alignment) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }
void operator delete(void* const ptr, __unused const size_t size, __unused const size_t alignment) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }
void operator delete[](void* const ptr, __unused const size_t size, __unused const size_t alignment) noexcept { GEN_INTERNAL_OPERATOR_DELETE_GLOBAL_OVERLOAD_BODY; }

void* operator new(const size_t size, const size_t alignment) { GEN_INTERNAL_ALIGNED_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }
void* operator new[](const size_t size, const size_t alignment) { GEN_INTERNAL_ALIGNED_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }
void* operator new(const size_t size, const size_t alignment, __unused const std::nothrow_t& const tag) noexcept { GEN_INTERNAL_ALIGNED_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }
void* operator new[](const size_t size, const size_t alignment, __unused const std::nothrow_t& const tag) noexcept { GEN_INTERNAL_ALIGNED_OPERATOR_NEW_GLOBAL_OVERLOAD_BODY; }
