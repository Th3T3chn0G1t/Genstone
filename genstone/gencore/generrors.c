// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#if PLATFORM == WIN
#include <Windows.h>
#endif
#include "include/gencommon.h"

#if GEN_CENTRALIZE_EH == ENABLED
gen_error_handler_t gen_error_handler = NULL;
void* gen_error_handler_passthrough = NULL;
#endif

const char* gen_error_name(const gen_error_t error) {
	switch(error) {
		case GEN_OK: return "GEN_OK";
		case GEN_UNKNOWN: return "GEN_UNKNOWN";
		case GEN_PERMISSION: return "GEN_PERMISSION";
		case GEN_INVALID_PARAMETER: return "GEN_INVALID_PARAMETER";
		case GEN_IO: return "GEN_IO";
		case GEN_TOO_LONG: return "GEN_TOO_LONG";
		case GEN_NO_SUCH_OBJECT: return "GEN_NO_SUCH_OBJECT";
		case GEN_OUT_OF_MEMORY: return "GEN_OUT_OF_MEMORY";
		case GEN_WRONG_OBJECT_TYPE: return "GEN_WRONG_OBJECT_TYPE";
		case GEN_ALREADY_EXISTS: return "GEN_ALREADY_EXISTS";
		case GEN_OUT_OF_SPACE: return "GEN_OUT_OF_SPACE";
		case GEN_OUT_OF_HANDLES: return "GEN_OUT_OF_HANDLES";
		case GEN_TOO_SHORT: return "GEN_TOO_SHORT";
		case GEN_BAD_CONTENT: return "GEN_BAD_CONTENT";
	}
}

const char* gen_error_description(const gen_error_t error) {
	switch(error) {
		case GEN_OK: return "No error occurred";
		case GEN_UNKNOWN: return "An unknown error occurred";
		case GEN_PERMISSION: return "A permission error occurred";
		case GEN_INVALID_PARAMETER: return "The provided parameter was invalid";
		case GEN_IO: return "An IO error occurred";
		case GEN_TOO_LONG: return "The provided argument is too long";
		case GEN_NO_SUCH_OBJECT: return "A nonexistent object was referenced";
		case GEN_OUT_OF_MEMORY: return "Program ran out of usable memory";
		case GEN_WRONG_OBJECT_TYPE: return "An object of the wrong type was provided";
		case GEN_ALREADY_EXISTS: return "An object already exists by the provided identifier";
		case GEN_OUT_OF_SPACE: return "The specified destination is out of space";
		case GEN_OUT_OF_HANDLES: return "Too many platform handles are open";
		case GEN_TOO_SHORT: return "The provided argument is too short";
		case GEN_BAD_CONTENT: return "The provided argument contains bad or invalid content";
	}
}

GEN_ERRORABLE_RETURN gen_convert_errno(errno_t error) {
	switch(error) {
		case EACCES: return GEN_PERMISSION;
		case EINVAL: return GEN_INVALID_PARAMETER;
		case EIO: return GEN_IO;
		case ELOOP: return GEN_TOO_LONG;
		case ENAMETOOLONG: return GEN_TOO_LONG;
		case ENOENT: return GEN_NO_SUCH_OBJECT;
		case ENOMEM: return GEN_OUT_OF_MEMORY;
		case ENOTDIR: return GEN_WRONG_OBJECT_TYPE;
#if PLATFORM != WIN // Bit random to be missing but okay
		case EDQUOT: return GEN_OUT_OF_SPACE;
#endif
		case EEXIST: return GEN_ALREADY_EXISTS;
		case EMLINK: return GEN_TOO_LONG;
		case ENOSPC: return GEN_OUT_OF_SPACE;
		case EPERM: return GEN_PERMISSION;
		case EROFS: return GEN_PERMISSION;
		case EMFILE: return GEN_OUT_OF_HANDLES;
		case ENFILE: return GEN_OUT_OF_HANDLES;
		case EBADF: return GEN_INVALID_PARAMETER;
		default: return GEN_UNKNOWN;
	}
}

// https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror/17387176#17387176
// Returns the last Win32 error, in string format. Returns an empty string if there is no error.
void gen_winerr_as_string(char* const restrict outbuff, size_t* const restrict outsize, const unsigned long error) {
#if PLATFORM == WIN
	char* locoutbuff = NULL;

	// Ask Win32 to give us the string version of that message ID.
	// The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &locoutbuff, 0, NULL);

	if(outsize) *outsize = size;
	if(outbuff) strcpy(outbuff, outbuff);

	// Free the Win32's string's buffer.
	LocalFree(locoutbuff);

	return message;
#else
	(void) outbuff;
	(void) outsize;
	(void) error;
	glog(WARNING, "Calling gen_winerr_as_string() on non-Windows platform");
#endif
}

GEN_ERRORABLE_RETURN gen_convert_winerr(unsigned long error) {
#if PLATFORM == WIN
	switch(error) {
		case ERROR_ALREADY_EXISTS: return GEN_ALREADY_EXISTS;
		case ERROR_PATH_NOT_FOUND: return GEN_NO_SUCH_OBJECT;
		default: return GEN_UNKNOWN;
	}
#else
	(void) error;
	glog(WARNING, "Calling gen_convert_winerr() on non-Windows platform");
	return GEN_UNKNOWN;
#endif
}
