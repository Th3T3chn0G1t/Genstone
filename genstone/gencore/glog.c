// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/gencommon.h"
#include "include/gentooling.h"

FILE* gen_glog_out_streams[GEN_GLOG_STREAM_COUNT + 1] = {0};
FILE* gen_glog_err_streams[GEN_GLOG_STREAM_COUNT + 1] = {0};

__attribute__((constructor)) static void gen_internal_initialize_glog_streams(void) {
	gen_glog_out_streams[0] = stdout;
	gen_glog_err_streams[0] = stderr;
}
