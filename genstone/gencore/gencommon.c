// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"

void gen_timeval_add(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result) {
	result->tv_sec = a->tv_sec + b->tv_sec;
	result->tv_usec = a->tv_usec + b->tv_usec;
	if(result->tv_usec >= GEN_MICROSECONDS_PER_SECOND) {
		result->tv_sec++;
		result->tv_usec -= GEN_MICROSECONDS_PER_SECOND;
	}
}

void gen_timeval_sub(const struct timeval* const restrict a, const struct timeval* const restrict b, struct timeval* const restrict result) {
	if(a->tv_sec > b->tv_sec) {
		if(a->tv_usec >= b->tv_usec) {
			result->tv_sec = a->tv_sec - b->tv_sec;
			result->tv_usec = a->tv_usec - b->tv_usec;
		}
		else {
			result->tv_sec = a->tv_sec - b->tv_sec - 1;
			result->tv_usec = GEN_MICROSECONDS_PER_SECOND - b->tv_usec + a->tv_usec;
		}
	}
	else if(a->tv_sec < b->tv_sec) {
		if(a->tv_usec > b->tv_usec) {
			result->tv_sec = b->tv_sec - a->tv_sec - 1;
			result->tv_usec = GEN_MICROSECONDS_PER_SECOND - a->tv_usec + b->tv_usec;
		}
		else {
			result->tv_sec = b->tv_sec - a->tv_sec;
			result->tv_usec = b->tv_usec - a->tv_usec;
		}
	}
	else {
		result->tv_sec = 0;
		if(a->tv_usec >= b->tv_usec) {
			result->tv_usec = a->tv_usec - b->tv_usec;
		}
		else {
			result->tv_usec = b->tv_usec - a->tv_usec;
		}
	}
}
