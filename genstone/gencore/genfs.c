// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genfs.h"

#include "include/gendbg.h"
#include "include/gentooling.h"

#if GEN_DEBUG_PATH_VALIDATION == ENABLED
/**
 * Validates the path parameter to filesystem functions
 * @param path the path to validate
 * @see GEN_DEBUG_PATH_VALIDATION
 */
#define GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path) \
	do { \
		if(!path) return GEN_INVALID_PARAMETER; \
		gen_error_t path_error = gen_path_validate(path); \
		if(path_error) return path_error; \
	} while(0)
#else
#define GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path) (void) path
#endif

#define GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, fhandle_index, error) \
	do { \
		if(!error) { \
			if(ferror(handle->file_handles[fhandle_index])) \
				error = errno; \
			else \
				error = 0; \
			clearerr(handle->file_handles[fhandle_index]); \
			if(error) return gen_convert_errno(error); \
		} \
	} while(0)

#if PLATFORM == WIN
// Source adapted from MUSL LibC
// See `genstone/vendor/c11compat/musl/COPYRIGHT` for further licence information

__inline char* strchrnul(char* restrict s, int c) {
	c = (unsigned char) c;
	if(!c) return s + strlen(s);

	for(; *s && *(unsigned char*) s != c; s++)
		;
	return (char*) s;
}

__inline size_t slash_len(const char* s) {
	const char* s0 = s;
	while(*s == '/') s++;
	return (uintptr_t) (s - s0);
}

__inline char* realpath(const char* restrict filename, char* const restrict resolved) {
	char stack[GEN_PATH_MAX + 1];
	char output[GEN_PATH_MAX];
	size_t p, q, l, l0, cnt = 0, nup = 0;
	int check_dir = 0;

	if(!filename) {
		errno = EINVAL;
		return 0;
	}
	l = strlen(filename);
	if(!l) {
		errno = ENOENT;
		return 0;
	}
	if(l >= GEN_PATH_MAX) goto toolong;
	p = sizeof(stack) - l - 1;
	q = 0;
	memcpy_s(stack + p, sizeof(stack) - p, filename, l + 1);

	/* Main loop. Each iteration pops the next part from stack of
	 * remaining path components and consumes any slashes that follow.
	 * If not a link, it's moved to output; if a link, contents are
	 * pushed to the stack. */
restart:
	for(;; p += slash_len(stack + p)) {
		/* If stack starts with /, the whole component is / or //
		 * and the output state must be reset. */
		if(stack[p] == '/') {
			check_dir = 0;
			nup = 0;
			q = 0;
			output[q++] = '/';
			p++;
			/* Initial // is special. */
			if(stack[p] == '/' && stack[p + 1] != '/')
				output[q++] = '/';
			continue;
		}

		char* z = strchrnul(stack + p, '/');
		l0 = l = (uintptr_t) (z - (stack + p));

		if(!l && !check_dir) break;

		/* Skip any . component but preserve check_dir status. */
		if(l == 1 && stack[p] == '.') {
			p += l;
			continue;
		}

		/* Copy next component onto output at least temporarily, to
		 * call readlink, but wait to advance output position until
		 * determining it's not a link. */
		if(q && output[q - 1] != '/') {
			if(!p) goto toolong;
			stack[--p] = '/';
			l++;
		}
		if(q + l >= GEN_PATH_MAX) goto toolong;
		memcpy_s(output + q, sizeof(output) - q, stack + p, l);
		output[q + l] = 0;
		p += l;

		int up = 0;
		if(l0 == 2 && stack[p - 2] == '.' && stack[p - 1] == '.') {
			up = 1;
			/* Any non-.. path components we could cancel start
			 * after nup repetitions of the 3-byte string "../";
			 * if there are none, accumulate .. components to
			 * later apply to cwd, if needed. */
			if(q <= 3 * nup) {
				nup++;
				q += l;
				continue;
			}
			/* When previous components are already known to be
			 * directories, processing .. can skip readlink. */
			if(!check_dir) goto skip_readlink;
		}
		ssize_t k = readlink(output, stack, p);
		if((size_t) k == p) goto toolong;
		if(!k) {
			errno = ENOENT;
			return 0;
		}
		if(k < 0) {
			if(errno != EINVAL) return 0;
		skip_readlink:
			check_dir = 0;
			if(up) {
				while(q && output[q - 1] != '/') q--;
				if(q > 1 && (q > 2 || output[0] != '/')) q--;
				continue;
			}
			if(l0) q += l;
			check_dir = stack[p];
			continue;
		}
		if(++cnt == 40 /*Hardcoding because garbage code is more fun*/) {
			errno = ELOOP;
			return 0;
		}

		/* If link contents end in /, strip any slashes already on
		 * stack to avoid /->// or //->/// or spurious toolong. */
		if(stack[k - 1] == '/')
			while(stack[p] == '/') p++;
		p -= (size_t) k;
		memmove(stack + p, stack, (size_t) k);

		/* Skip the stack advancement in case we have a new
		 * absolute base path. */
		goto restart;
	}

	output[q] = 0;

	if(output[0] != '/') {
		if(!getcwd(stack, sizeof stack)) return 0;
		l = strlen(stack);
		/* Cancel any initial .. components. */
		p = 0;
		while(nup--) {
			while(l > 1 && stack[l - 1] != '/') l--;
			if(l > 1) l--;
			p += 2;
			if(p < q) p++;
		}
		if(q - p && stack[l - 1] != '/') stack[l++] = '/';
		if(l + (q - p) + 1 >= GEN_PATH_MAX) goto toolong;
		memmove(output + l, output + p, q - p + 1);
		memcpy_s(output, sizeof(output), stack, l);
		q = l + q - p;
	}

	if(resolved)
		return memcpy(resolved, output, q + 1);
	else
		return strdup(output);

toolong:
	errno = ENAMETOOLONG;
	return 0;
}
#endif

gen_error_t gen_path_canonical(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_canonical);

	if(!output_path) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	// Getting an error out of this function is very strange
	// We just have to presume that GEN_PATH_MAX will always be enough storage
	unsigned long error = GetFullPathNameA(path, GEN_PATH_MAX, output_path, NULL);
	if(!error) return gen_convert_winerr(GetLastError());
#else
	char* error = realpath(path, output_path);
	if(!error) return gen_convert_errno(errno);
#endif

	return GEN_OK;
}

gen_error_t gen_path_filename(char* restrict output_filename, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_filename);

	if(!output_filename) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = strcpy_s(output_filename, GEN_PATH_MAX, strrchr(path, '/') + 1);
	if(error) return gen_convert_errno(error);

	return GEN_OK;
}

gen_error_t gen_path_pathname(char* restrict output_path, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_pathname);

	if(!output_path) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) ((strrchr(path, '/') + 1) - path);
	errno_t error = strncpy_s(output_path, GEN_PATH_MAX, path, mark);
	if(error) return gen_convert_errno(error);
	output_path[mark - 1] = '\0';

	return GEN_OK;
}

gen_error_t gen_path_extension(char* restrict output_extension, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_extension);

	if(!output_extension) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	size_t mark = (size_t) (strchr(strrchr(path, '/'), '.') - path);
	errno_t error = strcpy_s(output_extension, GEN_PATH_MAX, path + mark);
	if(error) return gen_convert_errno(error);
	output_extension[mark - 1] = '\0';

	return GEN_OK;
}

bool gen_path_exists(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_exists);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	return PathFileExistsA(path);
#else
	return !access(path, F_OK);
#endif
}

gen_error_t gen_path_validate(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_validate);

	if(!path) return GEN_INVALID_PARAMETER;
	if(!path[0]) return GEN_TOO_SHORT;

		// macOS apparently has no restrictions on file names?
		// and Linux/BSD apparently only prevent using /
		// In practise its probably more complicated, but this will do for now
#if PLATFORM == WIN
	const size_t len = strlen(path);

	if(len > GEN_PATH_MAX) {
		return GEN_TOO_LONG;
	}
	GEN_FOREACH_PTR(i, path_char, len, path) {
		const static char invalid_chars[] = "/\\:*?\"<>|";
		GEN_FOREACH_PTR(j, invalid, sizeof(invalid_chars), invalid_chars) {
			if(*path_char == *invalid) {
				return GEN_WRONG_OBJECT_TYPE;
			}
		}
	}
#else
	if(strlen(path) > GEN_PATH_MAX) return GEN_TOO_LONG;
#endif

	return GEN_OK;
}

gen_error_t gen_path_create_dir(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_create_dir);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

#if PLATFORM == WIN
	unsigned long error = CreateDirectoryA(path, NULL);
	if(!error) return gen_convert_winerr(GetLastError());
#else
	errno_t error = mkdir(path, 0777);
	if(error) return gen_convert_errno(errno);
#endif

	return GEN_OK;
}

gen_error_t gen_path_delete(const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_path_delete);

	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = remove(path);
	if(error) return gen_convert_errno(errno);

	return GEN_OK;
}

gen_error_t gen_handle_open(gen_filesystem_handle_t* restrict output_handle, const char* const restrict path) {
	GEN_FRAME_BEGIN(gen_handle_open);

	if(!output_handle) return GEN_INVALID_PARAMETER;
	GEN_INTERNAL_FS_PATH_PARAMETER_VALIDATION(path);

	errno_t error = 0;
	error = strcpy_s(output_handle->path, GEN_PATH_MAX, path);
	if(error) return gen_convert_errno(errno);

	struct stat s;
	error = stat(path, &s);
	if(error && errno != ENOENT) return gen_convert_errno(errno);
	if(S_ISDIR(s.st_mode)) {
		output_handle->dir = true;
		output_handle->directory_handle = opendir(path);
		if(!output_handle->directory_handle) return gen_convert_errno(errno);
	}
	else {
		output_handle->dir = false;
		output_handle->file_handles[1] = fopen(path, "w+");
		if(!output_handle->file_handles[1]) return gen_convert_errno(errno);
		output_handle->file_handles[0] = fopen(path, "r");
		if(!output_handle->file_handles[0]) return gen_convert_errno(errno);
	}

	return GEN_OK;
}

gen_error_t gen_handle_close(gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_close);

	if(!handle) return GEN_INVALID_PARAMETER;

	free(handle->path);

	if(handle->dir) {
		errno_t error = closedir(handle->directory_handle);
		if(error) return gen_convert_errno(errno);
	}
	else {
		errno_t error = fclose(handle->file_handles[0]);
		if(error) return gen_convert_errno(errno);
		error = fclose(handle->file_handles[1]);
		if(error) return gen_convert_errno(errno);
	}

	return GEN_OK;
}

size_t gen_handle_size(const gen_filesystem_handle_t* const restrict handle) {
	GEN_FRAME_BEGIN(gen_handle_size);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(handle->dir) return GEN_WRONG_OBJECT_TYPE;

	int error = fseek(handle->file_handles[0], 0, SEEK_END);
	if(error) return gen_convert_errno(errno);
	size_t mark = (size_t) ftell(handle->file_handles[0]);
	if(mark == SIZE_MAX) return gen_convert_errno(errno);

	rewind(handle->file_handles[0]);

	return mark;
}

gen_error_t gen_file_read(uint8_t* restrict output_buffer, const gen_filesystem_handle_t* const restrict handle, const size_t start, const size_t end) {
	GEN_FRAME_BEGIN(gen_file_read);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(handle->dir) return GEN_WRONG_OBJECT_TYPE;
	if(!output_buffer) return GEN_INVALID_PARAMETER;

	int error = fseek(handle->file_handles[0], (long) start, SEEK_SET);
	if(error) return gen_convert_errno(errno);

	error = (int) fread(output_buffer, sizeof(uint8_t), end - start, handle->file_handles[0]);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, 0, error);

	rewind(handle->file_handles[0]);

	return GEN_OK;
}

gen_error_t gen_file_write(const gen_filesystem_handle_t* const restrict handle, const size_t n_bytes, const uint8_t* const restrict buffer) {
	GEN_FRAME_BEGIN(gen_file_write);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(handle->dir) return GEN_WRONG_OBJECT_TYPE;
	if(!buffer) return GEN_INVALID_PARAMETER;

	int error = (int) fwrite(buffer, sizeof(uint8_t), n_bytes, handle->file_handles[1]);
	GEN_INTERNAL_FS_FP_HANDLE_ERR(handle, 1, error);

	rewind(handle->file_handles[1]);

	return GEN_OK;
}

gen_error_t gen_directory_list(const gen_filesystem_handle_t* const restrict handle, const gen_directory_list_handler_t handler, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_directory_list);

	if(!handle) return GEN_INVALID_PARAMETER;
	if(!handle->dir) return GEN_WRONG_OBJECT_TYPE;
	if(!handler) return GEN_INVALID_PARAMETER;

	struct dirent* entry;
	errno = 0;
	while((entry = readdir(handle->directory_handle))) {
		if(!entry && errno) return gen_convert_errno(errno);
		if(entry->d_name[0] == '.' && entry->d_name[1] == '\0') continue;
		if(entry->d_name[0] == '.' && entry->d_name[1] == '.' && entry->d_name[2] == '\0') continue;

		handler(entry->d_name, passthrough);

		errno = 0;
	}
	rewinddir(handle->directory_handle);

	return GEN_OK;
}
