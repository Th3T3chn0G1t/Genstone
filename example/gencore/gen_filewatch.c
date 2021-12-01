// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfs.h>

int main(void) {
	gen_filesystem_handle_t my_file;
	gen_error_t error = gen_handle_open(&my_file, "./path/to/file/or/folder");

	gen_filesystem_handle_t handle;

	error = gen_filewatch_create(&handle, &my_file);
    GEN_REQUIRE_NO_ERROR(error);

	gen_filewatch_event_t event = GEN_FILEWATCH_NONE;

	while(true) {
		error = gen_filewatch_poll(&handle, &event);
        GEN_REQUIRE_NO_ERROR(error);

		if(event) {
			if(event & GEN_FILEWATCH_CREATED) glog(INFO, "File created");
			if(event & GEN_FILEWATCH_MODIFIED) glog(INFO, "File modified");
			if(event & GEN_FILEWATCH_DELETED) glog(INFO, "File deleted");
			if(event & GEN_FILEWATCH_MOVED) glog(INFO, "File moved");
		}
	}

	error = gen_filewatch_destroy(&handle);
    GEN_REQUIRE_NO_ERROR(error);
}
