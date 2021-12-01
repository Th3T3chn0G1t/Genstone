// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfs.h>
#include <genlocale.h>

int main(void) {
	glog(INFO, "Hello, Genstone!");

	gen_filesystem_handle_t cwd;
	gen_error_t error = gen_handle_open(&cwd, "sandbox/sample");
	GEN_REQUIRE_NO_ERROR(error);

	gen_filesystem_handle_t handle;
	error = gen_filewatch_create(&handle, &cwd);
	GEN_REQUIRE_NO_ERROR(error);
	gen_filewatch_event_t event = GEN_FILEWATCH_NONE;
	while(true) {
		error = gen_filewatch_poll(&handle, &event);
		GEN_REQUIRE_NO_ERROR(error);

		if(event & GEN_FILEWATCH_CREATED) glog(INFO, "File created");
		if(event & GEN_FILEWATCH_MODIFIED) glog(INFO, "File modified");
		if(event & GEN_FILEWATCH_DELETED) glog(INFO, "File deleted");
		if(event & GEN_FILEWATCH_MOVED) glog(INFO, "File moved");
	}
}
