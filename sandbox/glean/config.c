// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+glean@pm.me>

#include "include/glean.h"

/**
 * A mapping from file extensions to Gtk named icons
 */
icons_T* icon_map;

/**
 * A vector of file templates
 */
vector_T* file_templates;
/**
 * A vector of file template names
 */
vector_T* file_template_names;

/**
 * Loads configs into a map from disk
 * Will try to get keys from the project-local `./.glean/config.json` file, otherwise will use the user-local `~/.config/glean/config.json`, creating the file if it doesn't exist
 */
void config_init(void) {
	GEN_FRAME_BEGIN(config_init);

	glog(INFO, "Loading config... (Local)");
	glog(INFO, "Loading config... (Global)");

	//	gtk_icon_theme_append_search_path(gtk_icon_theme_get_default(), "res/icon/forkawesome/src/icons/svg");

	// Maps file extensions to gtk named icons for the file tree
	(void) galloc((void**) &icon_map, 1, sizeof(icon_map));
	{
		icon_map->n_mappings = 4;
		(void) galloc((void**) &icon_map->keys, icon_map->n_mappings, sizeof(icon_map_key_T));
		(void) galloc((void**) &icon_map->icons, icon_map->n_mappings, sizeof(char*));

		icon_map_key_T c_key = {EXTENSION, "c"};
		icon_map->keys[0] = c_key;
		icon_map_key_T md_key = {EXTENSION, "md"};
		icon_map->keys[1] = md_key;
		icon_map_key_T mk_key = {EXTENSION, "mk"};
		icon_map->keys[2] = mk_key;
		icon_map_key_T makefile_key = {NAME, "Makefile"};
		icon_map->keys[3] = makefile_key;

		icon_map->icons[0] = "application-x-executable";
		icon_map->icons[1] = "format-text-underline";
		icon_map->icons[2] = "application-x-executable";
		icon_map->icons[3] = "application-x-executable";
	}

	// Load global templates
	(void) gzalloc((void**) &file_templates, 1, sizeof(vector_T));
	(void) gzalloc((void**) &file_template_names, 1, sizeof(vector_T));

	// Iterate over files in `path`
	DIR* folder = opendir("sandbox/glean/usr/templates");
	if(!folder)
		glogf(ERROR, "Failed to open directory %s: %s", "sandbox/glean/usr/templates", strerror(errno));

	struct dirent* entry = NULL;
	errno = 0;
	while((entry = readdir(folder))) {
		if(errno) {
			glogf(ERROR, "Failed to read from directory %s: %s", "sandbox/glean/usr/templates", strerror(errno));
			continue;
		}
		// If the file is the current directory (`.`) or the parent directory (`..`), skip the current iteration
		if(strncmp(entry->d_name, "..", 3) == 0 || strncmp(entry->d_name, ".", 2) == 0) continue;

		// Concatonate the two paths with a path delimiter (`/`) in between
		size_t needed = (size_t) snprintf(NULL, 0, "%s/%s", "sandbox/glean/usr/templates", entry->d_name) + 1ul;
		char* buffer;
		(void) galloc((void**) &buffer, needed, sizeof(char));
		sprintf(buffer, "%s/%s", "sandbox/glean/usr/templates", entry->d_name);

		gen_filesystem_handle_t handle;
		(void) galloc((void**) &handle.path, GEN_PATH_MAX, sizeof(char));
		(void) gen_handle_open(&handle, buffer);
		size_t file_buffer_size;
		(void) gen_handle_size(&file_buffer_size, &handle);
		char* file_buffer;
		(void) galloc((void**) &file_buffer, (size_t) (file_buffer_size + 1), sizeof(char));
		(void) gen_handle_read((uint8_t*) file_buffer, &handle, 0, file_buffer_size);
		file_buffer[file_buffer_size] = '\0';

		(void) gen_handle_close(&handle);

		vector_append(file_templates, file_buffer);
		char* name = get_extensionless_filename(buffer);
		(void) gfree(buffer);
		vector_append(file_template_names, name);
	}

	// Don't forget to clean up
	if(closedir(folder))
		glogf(ERROR, "Failed to close directory %s: %s", "sandbox/glean/usr/templates", strerror(errno));

	glogf(INFO, "Config loaded from %i files!", 0);
}

/**
 * Cleans up the config state
 */
void config_cleanup(void) {
	GEN_FRAME_BEGIN(config_cleanup);

	(void) gfree(icon_map->icons);
	(void) gfree(icon_map->keys);
	(void) gfree(icon_map);

	for(size_t i = 0; i < file_templates->n_members; i++) {
		(void) gfree(file_templates->members[i]);
		(void) gfree(file_template_names->members[i]);
	}
	(void) gfree(file_templates);
	(void) gfree(file_template_names);
}
