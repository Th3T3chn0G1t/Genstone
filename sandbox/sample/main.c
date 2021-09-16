// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencalc.h>
#include <gencommon.h>
#include <genesis.h>
#include <gentooling.h>

static int appticks = 0;
static gen_node_t approot = {GEN_ROOT_TYPE, sizeof(int), &appticks, 0, NULL, NULL};
static mtx_t approot_mtx;

static int thread_func(__unused void* passthrough) {
	glog(INFO, "Hello, C11 threads!");

	for(size_t i = 0; i < 10; ++i) {
		mtx_lock(&approot_mtx);
		((int*) approot.data)[0]++;
		mtx_unlock(&approot_mtx);
	}

	return 0;
}
int main() {
	glog(INFO, "Hello, Genesis!");

	mtx_init(&approot_mtx, mtx_plain);

	thrd_t thread;
	thrd_create(&thread, thread_func, NULL);

	for(size_t i = 0; i < 10; ++i) {
		mtx_lock(&approot_mtx);
		++(((int*) approot.data)[0]);
		mtx_unlock(&approot_mtx);
	}

	int result;
	thrd_join(thread, &result);

	glogf(DEBUG, "Ticks: %i", ((int*) approot.data)[0]);

	mtx_destroy(&approot_mtx);
}
