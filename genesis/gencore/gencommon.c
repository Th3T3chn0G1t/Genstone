#include "include/gencommon.h"

#if PLATFORM == WIN
// Thanks to https://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows/26085827#26085827
#include <Windows.h>

int gettimeofday(struct timeval* const restrict tp, __unused const void* const restrict tzp) {
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970
	static const unsigned long long EPOCH = ((unsigned long long) 116444736000000000ULL);

	SYSTEMTIME system_time;
	FILETIME file_time;
	unsigned long long time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((unsigned long long) file_time.dwLowDateTime);
	time += ((unsigned long long) file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long) ((time - EPOCH) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
	return 0;
}
#endif

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
