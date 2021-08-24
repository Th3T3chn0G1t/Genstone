// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/gentooling.h"

gen_tooling_stack_t gen_tooling_call_stack = { 0, { 0 }, { 0 }, { 0 } };
gen_tooling_freq_profile_t gen_tooling_freq_profiles[GEN_FREQ_PROFILE_MAX] = { 0 };
size_t gen_tooling_freq_profile_next = 0;

gen_tooling_stack_push_handler_t gen_tooling_push_handler = NULL;
gen_tooling_stack_pop_handler_t gen_tooling_pop_handler = NULL;

void gen_tooling_stack_push(const char* restrict frame, const uintptr_t address, const char* restrict file) {
    if(gen_tooling_push_handler) gen_tooling_push_handler();
    gen_tooling_call_stack.functions[gen_tooling_call_stack.next] = frame;
    gen_tooling_call_stack.addresses[gen_tooling_call_stack.next] = address;
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

/** @see https://gist.github.com/vchernov/4774682#file-timeval_add-cpp */
static void gen_internal_timeval_add(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result) {
    result->tv_sec = a->tv_sec + b->tv_sec;
    result->tv_usec = a->tv_usec + b->tv_usec;
    if (result->tv_usec >= GEN_INTERNAL_USECONDS_PER_SECOND) {
        result->tv_sec++;
        result->tv_usec -= GEN_INTERNAL_USECONDS_PER_SECOND;
    }
}

/** @see https://gist.github.com/vchernov/4774682#file-timeval_sub-cpp */
static void gen_internal_timeval_sub(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result) {
    if (a->tv_sec > b->tv_sec) {
        if (a->tv_usec >= b->tv_usec) {
            result->tv_sec = a->tv_sec - b->tv_sec;
            result->tv_usec = a->tv_usec - b->tv_usec;
        }
        else {
            result->tv_sec = a->tv_sec - b->tv_sec - 1;
            result->tv_usec = GEN_INTERNAL_USECONDS_PER_SECOND - b->tv_usec + a->tv_usec;
        }
    }
    else if (a->tv_sec < b->tv_sec) {
        if (a->tv_usec > b->tv_usec) {
            result->tv_sec = b->tv_sec - a->tv_sec - 1;
            result->tv_usec = GEN_INTERNAL_USECONDS_PER_SECOND - a->tv_usec + b->tv_usec;
        }
        else {
            result->tv_sec = b->tv_sec - a->tv_sec;
            result->tv_usec = b->tv_usec - a->tv_usec;
        }
    }
    else {
        result->tv_sec = 0;
        if (a->tv_usec >= b->tv_usec) {
            result->tv_usec = a->tv_usec - b->tv_usec;
        }
        else {
            result->tv_usec = b->tv_usec - a->tv_usec;
        }
    }
}

void gen_tooling_freq_profile_ping(const char* const restrict name) {
    GEN_FOREACH_PTR(i, profile, gen_tooling_freq_profile_next, gen_tooling_freq_profiles) {
        if(profile->name == name) {
            struct timeval current_time;
            gettimeofday(&current_time, NULL);

            struct timeval delta;
            gen_internal_timeval_sub(&current_time, &profile->last, &delta);
            gen_internal_timeval_add(&profile->running, &delta, &profile->running);
            profile->last.tv_sec = current_time.tv_sec;
            profile->last.tv_usec = current_time.tv_usec;
            ++profile->n_calls;

            return;
        }
    }

    gen_tooling_freq_profile_t* const new_profile = &gen_tooling_freq_profiles[gen_tooling_freq_profile_next++];

    new_profile->name = name;

    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    new_profile->first.tv_sec = current_time.tv_sec;
    new_profile->first.tv_usec = current_time.tv_usec;
    new_profile->last.tv_sec = current_time.tv_sec;
    new_profile->last.tv_usec = current_time.tv_usec;
    ++new_profile->n_calls;
}
