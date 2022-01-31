// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+glean@pm.me>

#include "include/glean.h"

GEN_DIAG_REGION_BEGIN
#pragma clang diagnostic ignored "-Wreserved-identifier"

/**
 * A message to an inotify watcher thread
 */
typedef struct {
	/**
	 * The message type
	 */
	enum
	{
		ADD,
		REMOVE,
		CLOSE
	} type;

	/**
	 * The path to the file to watch
	 * Only valid for `type` = `ADD`
	 */
	char* path;
	/**
	 * The inotify event masks to monitor
	 * Only valid for `type` = `ADD`
	 */
	uint32_t mask;
	/**
	 * A pipe to send the created watch descriptor back to the calling thread
	 * Only valid for `type` = `ADD`
	 */
	int ret_fd;
	/**
	 * The callback to call when the file event occurs
	 * Only valid for `type` = `ADD`
	 */
	inotify_watch_callback_T callback;
	/**
	 * The user parameter to pass to `callback`
	 * Only valid for `type` = `ADD`
	 */
	void* pass;

	/**
	 * The watch descriptor to remove
	 * Only valid for `type` = `REMOVE`
	 */
	int wd;
} thread_message_T;

/**
 * The directory root of the open editor
 * Represents a file tree starting from the directory root
 */
tree_node_T* root = NULL;

static GtkTreeView* tree_view = NULL;

/**
 * The pipe used for inotify
 */
static int inotify_fd;

/**
 * Gets the base filename without the extension
 * @param path the path to get the name from
 * @return the filename, allocated on the heap
 */
char* get_extensionless_filename(const char* path) {
	GEN_FRAME_BEGIN(get_extensionless_filename);

	char* tmp;
	(void) gstrndup(&tmp, path, strlen(path)); // Basename sometimes is allowed modify its argument
	char* base = basename(tmp);
	char* dot = strchr(base, '.');
	if(!dot || dot == base) {
		(void) gfree(tmp);
		char* str;
		(void) gstrndup(&str, base, strlen(base));
		return str;
	}

	size_t chars_length = (size_t) ((uintptr_t) dot - (uintptr_t) base);
	(void) gfree(tmp);
	char* str;
	(void) gstrndup(&str, base, chars_length);
	return str;
}

/**
 * Gets a path relative to `~` if the path begins with `$HOME`
 * @param path the path to make relative, does not affect original string
 * @return the relative path, allocated on the heap, or NULL if the string does not begin with `$HOME`
 */
char* get_home_relative_path(char* path) {
	GEN_FRAME_BEGIN(get_home_relative_path);

	char* home = getenv("HOME");
	size_t home_length = strlen(home);

	char* home_rel_path = NULL;
	if(!strncmp(path, home, home_length)) {
		(void) gstrndup(&home_rel_path, path + (home_length - 1), strlen(path + (home_length - 1)));
		home_rel_path[0] = '~';
	}
	return home_rel_path;
}

/**
 * Replaces the first segment of a string beginning with `~` with `$HOME`
 * @param path the path to try replacement in, does not affect original string
 * @return the new path, allocated on the heap, or NULL if the string does not begin with `~`
 */
char* get_path_from_home_relative(char* path) {
	GEN_FRAME_BEGIN(get_path_from_home_relative);

	if(path[0] == '~') {
		if(access(path, F_OK) == 0) return NULL; // `path` refers to a file `./~`
		size_t needed = (size_t) snprintf(NULL, 0, "%s%s", getenv("HOME"), path + 1) + 1;
		char* outbuffer;
		(void) galloc((void**) &outbuffer, needed, sizeof(char));
		sprintf(outbuffer, "%s%s", getenv("HOME"), path + 1);
		return outbuffer;
	}
	return NULL;
}

/**
 * Populates a tree store from a directory tree
 * @param store the store to populate
 * @param parent the parent iterator (directory)
 * @param node the current node (directory)
 */
static void _tree_view_populate(GtkTreeStore* store, GtkTreeIter* parent, tree_node_T* node) {
	GEN_FRAME_BEGIN(_tree_view_populate);

	for(unsigned long i = 0; i < node->children->members_length; i++) {
		tree_node_T* child = node->children->members[i];
		char* title = basename(child->data);

		// Add the child to the tree store
		GtkTreeIter current;
		gtk_tree_store_append(store, &current, parent);
		gtk_tree_store_set(store, &current, 0, "text-x-generic", 1, title, -1);

		// If the child's file extension is mapped to an icon, apply it
		for(size_t j = 0; j < icon_map->mappings_length; j++) {
			switch(icon_map->keys[j].type) {
				case NAME: {
					if(!strcmp(icon_map->keys[j].data, title)) {
						gtk_tree_store_set(store, &current, 0, icon_map->icons[j], 1, title, -1);
						break;
					}
					break;
				}
				case EXTENSION: {
					char* ext;
					(void) galloc((void**) &ext, GEN_PATH_MAX, sizeof(char));
					(void) gen_path_extension(ext, title);
					if(!strcmp(icon_map->keys[j].data, ext)) {
						gtk_tree_store_set(store, &current, 0, icon_map->icons[j], 1, title, -1);
						break;
					}
					(void) gfree(ext);
					break;
				}
			}
		}

		if(child->dir) {
			gtk_tree_store_set(store, &current, 0, "folder", 1, title, -1);
			_tree_view_populate(store, &current, child);
		}
	}
}

/**
 * `G_CALLBACK` for mouse button events
 * Tree view context menu handled here
 */
static bool file_tree_view_button_pressed(GtkWidget* widget, GdkEvent* event, void* pass) {
	GEN_FRAME_BEGIN(file_tree_view_button_pressed);

	(void) widget;
	(void) pass;

	GdkEventButton* button_press = (GdkEventButton*) event;
	if(button_press->button == GDK_BUTTON_SECONDARY) {
		GtkMenu* menu = GTK_MENU(gtk_menu_new());

		GtkMenuItem* new = GTK_MENU_ITEM(gtk_menu_item_new_with_label("New..."));
		glean_signal_connect_swapped(G_OBJECT(new), "activate", G_CALLBACK(editor_show_new_prompt), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(new), 0, 1, 1, 2);

		GtkMenuItem* refresh = GTK_MENU_ITEM(gtk_menu_item_new_with_label("Refresh"));
		glean_signal_connect_swapped(G_OBJECT(refresh), "activate", G_CALLBACK(tree_view_refresh), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(refresh), 0, 1, 2, 3);

		gtk_menu_attach(menu, gtk_separator_menu_item_new(), 0, 1, 3, 4);

		gtk_widget_show_all(GTK_WIDGET(menu));
		gtk_menu_popup_at_pointer(menu, event);
	}

	return false;
}

/**
 * `G_CALLBACK` for tree view row selection
 * Opens the selected file in a new tab
 */
static void tree_view_row_selected(GtkTreeView* widget, GtkTreePath* path, GtkTreeViewColumn* column, void* pass) {
	GEN_FRAME_BEGIN(tree_view_row_selected);

	(void) widget;
	(void) column;
	(void) pass;

	// Get the location of the selection in the tree store
	int depth;
	int* indices = gtk_tree_path_get_indices_with_depth(path, &depth);

	// Get the related tree node
	tree_node_T* node = directory_tree_get_node(root, indices, depth);
	if(node->dir) return;

	char* last_path_segment = basename(node->data);
	editor_tab_open_from_file_with_id(node->data, last_path_segment, node->data);
}

/**
 * Populates a tree view by reassigning and filling it's store from the global state directory tree (`root`)
 * @param view the view to populate
 * @param refill set to `false` if this is the first fill of the tree, otherwise `true`
 */
void tree_view_populate(GtkTreeView* view, bool refill) {
	GEN_FRAME_BEGIN(tree_view_populate);

	if(!tree_view) tree_view = view;

	GtkTreeStore* model = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

	_tree_view_populate(model, NULL, root);

	// Setting up the tree view to render the generated model
	gtk_tree_view_set_model(view, GTK_TREE_MODEL(model));
	GtkTreeSelection* selection = gtk_tree_view_get_selection(view);
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

	// Don't re-add columns on refill - bad things will happen
	if(!refill) {
		gtk_tree_view_insert_column_with_attributes(view, 0, NULL, gtk_cell_renderer_pixbuf_new(), "icon-name", 0, NULL);
		char* home_rel_path = get_home_relative_path(root->data);
		gtk_tree_view_insert_column_with_attributes(view, 1, home_rel_path, gtk_cell_renderer_text_new(), "text", 1, NULL);
		(void) gfree(home_rel_path);
	}
	// Callback for tree view rows being clicked
	glean_signal_connect(G_OBJECT(view), "row-activated", G_CALLBACK(tree_view_row_selected), NULL);

	// Callback for secondary mouse button presses (Context menu)
	glean_signal_connect(G_OBJECT(view), "button-press-event", G_CALLBACK(file_tree_view_button_pressed), NULL);
}

typedef struct {
	char* path;
	tree_node_T* upper_node;
	char* directory_path;
} directory_tree_populate_listdir_handler_passthrough_T;

static void directory_tree_populate(char* directory_path, tree_node_T* upper_node);

static void directory_tree_populate_listdir_handler(const char* const restrict path, void* const restrict passthrough) {
	directory_tree_populate_listdir_handler_passthrough_T* pass = passthrough;

	// If the file is the current directory (`.`) or the parent directory (`..`), skip the current iteration
	if(strncmp(path, "..", 3) == 0 || strncmp(path, ".", 2) == 0) return;

	// Concatonate the two paths with a path delimiter (`/`) in between
	size_t needed = (size_t) snprintf(NULL, 0, "%s/%s", pass->path, path) + 1ul;
	char* buffer;
	(void) galloc((void**) &buffer, needed, sizeof(char));
	sprintf(buffer, "%s/%s", pass->path, path);

	// Push back a new node into the children of the parent
	tree_node_T* child;
	(void) gzalloc((void**) &child, 1, sizeof(tree_node_T));
	child->data = buffer;
	vector_append(pass->upper_node->children, child);

	// If child is a directory, mark it as such and recurse another call
	struct stat file_info;
	if(stat(buffer, &file_info) == -1) {
		glogf(ERROR, "Failed to get file info %s: %s", buffer, strerror(errno));
		return;
	}

	if(S_ISDIR(file_info.st_mode)) {
		child->dir = true;
		(void) gzalloc((void**) &child->children, 1, sizeof(vector_T));
		directory_tree_populate(buffer, child);
	}
}

/**
 * Populates a directory tree (`tree_node_T`) from disk
 * Does not affect global state
 * @param directory_path the path to source files from for this call
 * @param upper_node the parent node in the directory tree
 */
static void directory_tree_populate(char* directory_path, tree_node_T* upper_node) {
	GEN_FRAME_BEGIN(directory_tree_populate);

	// Get the absolute path from an aesthetic path (home relative), otherwise duplicate the directory path to allow path to be `(void) gfree`ed at the end of the call
	char* path;
	if(!(path = get_path_from_home_relative(directory_path)))
		(void) gstrndup(&path, directory_path, strlen(directory_path));

	gen_filesystem_handle_t handle;
	(void) galloc((void**) &handle.path, GEN_PATH_MAX, sizeof(char));
	(void) gen_handle_open(&handle, path);
	directory_tree_populate_listdir_handler_passthrough_T passthrough = {path, upper_node, directory_path};
	(void) gen_directory_list(&handle, directory_tree_populate_listdir_handler, &passthrough);

	(void) gen_handle_close(&handle);
	(void) gfree(path);
}

/**
 * Creates an in-memory file tree from disk (`tree_node_T`)
 * Stores results to the global application state (`root`)
 * @param directory the directory to propogate downwards from
 */
void directory_tree_refresh(char* directory) {
	GEN_FRAME_BEGIN(directory_tree_refresh);

	glog(DEBUG, "Refreshing directory root...");

	// Different behaviour for first fill
	bool refill = false;
	if(root) refill = true;

	// Use the canonical path for actual FS ops
	char* canonical_path = realpath(directory, NULL);
	if(!canonical_path) {
		glogf(ERROR, "Could not accertain canonical path for %s: %s", directory, strerror(errno));
		canonical_path = directory;
	}

	// Use the home-relative path for aesthetic paths
	char* relative_path = get_home_relative_path(canonical_path);

	// Realloc time
	if(refill) directory_tree_free();
	(void) gzalloc((void**) &root, 1, sizeof(tree_node_T));

	// Populate the tree with POSIX magic
	(void) gstrndup(&root->data, canonical_path, strlen(canonical_path));
	(void) gzalloc((void**) &root->children, 1, sizeof(vector_T));
	directory_tree_populate(directory, root);

	// Don't forget to clean up
	(void) gfree(canonical_path);
	(void) gfree(relative_path);

	glog(DEBUG, "Refreshed!");
}

/**
 * Helper function
 * Refreshes the tree view and directory tree
 */
void tree_view_refresh(void) {
	GEN_FRAME_BEGIN(tree_view_refresh);

	char* dirroot;
	(void) gstrndup(&dirroot, root->data, strlen(root->data));
	directory_tree_refresh(dirroot);
	tree_view_populate(tree_view, true);
	(void) gfree(dirroot);
}

/**
 * Gets a node from a directory tree
 * @param current the node being accessed by the current call, or the root for primary calls
 * @param rel_path the remaining path to traverse to the node
 * @param depth the remaining depth to traverse to the node
 */
tree_node_T* directory_tree_get_node(tree_node_T* current, int* rel_path, int depth) {
	GEN_FRAME_BEGIN(directory_tree_get_node);

	if(depth == 0) return current;
	return directory_tree_get_node(current->children->members[*rel_path], rel_path + 1, depth - 1);
}

/**
 * Frees a directory tree downward from a node
 * @param current the current node being freed, or the base node to (void) gfree from
 */
static void _directory_tree_free(tree_node_T* current) {
	GEN_FRAME_BEGIN(_directory_tree_free);

	(void) gfree(current->data);
	if(current->children) {
		for(unsigned long i = 0; i < current->children->members_length; i++) {
			_directory_tree_free(current->children->members[i]);
			if(current->children->members[i]) (void) gfree(current->children->members[i]);
		}
		if(current->children->members) (void) gfree(current->children->members);
	}
	if(current->children) (void) gfree(current->children);
}

/**
 * Frees the directory tree in the application state (`root`)
 */
void directory_tree_free(void) {
	GEN_FRAME_BEGIN(directory_tree_free);

	_directory_tree_free(root);
	(void) gfree(root);
}

/**
 * Simple linear search
 * Finds the first occurrence in an array
 * @param key the key to search for
 * @param values the values to search in
 * @param values_length the number of values to search
 * @return the index of `key` in `values`, or SIZE_MAX if not present
 */
static size_t find(int key, int* values, size_t values_length) {
	GEN_FRAME_BEGIN(find);

	for(size_t i = 0; i < values_length; i++)
		if(values[i] == key) return i;
	return SIZE_MAX;
}

/**
 * A pipe for communicating with the watcher thread
 */
static int inotify_watch_thread_fd[2] = {0};
/**
 * A pipe for communicating with the main thread
 */
static int main_thread_fd = -1;
/**
 * Thread "callback" function which watches the current project root for changes
 */
static int inotify_watcher_func(void* vargp) {
	GEN_FRAME_BEGIN(inotify_watcher_func);

	(void) vargp;

	// Mappings from watch descriptors to callbacks and callback data
	size_t mappings_length = 0;
	int* other_wds = NULL;
	inotify_watch_callback_T* callbacks = NULL;
	void** passthroughs = NULL;

	// Loop until termination is signalled down the pipe
	fd_set set;
	size_t needed = sizeof(struct inotify_event) + PATH_MAX + 1;
	struct inotify_event* event;
	glog(INFO, "Starting directory watcher...");
	while(1) {
		// UNIX magic
		FD_ZERO(&set);
		FD_SET(inotify_fd, &set);
		FD_SET(inotify_watch_thread_fd[PIPE_READ], &set);
		if(select(MAX(inotify_fd, inotify_watch_thread_fd[PIPE_READ]) + 1, &set, NULL, NULL, NULL) == -1) {
			glogf(ERROR, "Failed to monitor %lu fds: %s", mappings_length + 1, strerror(errno));
			continue;
		}

		// If the thread communication pipe is written to, stop monitoring and exit the thread
		if(FD_ISSET(inotify_watch_thread_fd[PIPE_READ], &set)) {
			thread_message_T message;
			if(read(inotify_watch_thread_fd[PIPE_READ], &message, sizeof(thread_message_T)) == -1)
				glogf(ERROR, "Failed to read from thread communication pipe: %s", strerror(errno));
			switch(message.type) {
				case ADD: {
					// Allocate a new mapping for the incoming data
					(void) grealloc((void**) &other_wds, mappings_length, ++mappings_length, sizeof(int));
					(void) grealloc((void**) &callbacks, mappings_length - 1, mappings_length, sizeof(inotify_watch_callback_T));
					(void) grealloc((void**) &passthroughs, mappings_length - 1, mappings_length, sizeof(void*));
					// Add the watcher
					int wd = inotify_add_watch(inotify_fd, message.path, message.mask);
					if(wd == -1)
						glogf(ERROR, "Could not start watching %s: %s", message.path, strerror(errno));
					else {
						// Return the watch descriptor
						if(write(message.ret_fd, &wd, sizeof(int)) == -1)
							glogf(ERROR, "Failed to write return value to thread communication pipe: %s", strerror(errno));
						// Set the new mappings
						other_wds[mappings_length - 1] = wd;
						callbacks[mappings_length - 1] = message.callback;
						passthroughs[mappings_length - 1] = message.pass;

						glogf(DEBUG, "Added file watcher for %s", message.path);
					}
					break;
				}
				case REMOVE: {
					// Shifts all elements of the arrays back into the new vacancy in the mappings
					// This is unwiely as all hell but fixing it seems like it would just create more problems
					size_t index = find(message.wd, other_wds, mappings_length);
					if(index == SIZE_MAX)
						glogf(ERROR, "Could not find fd %i in watched file mappings", message.wd);
					else {
						int* new_other_wds = NULL;
						inotify_watch_callback_T* new_callbacks = NULL;
						void** new_passthroughs = NULL;
						if(--mappings_length) {
							(void) galloc((void**) &new_other_wds, mappings_length, sizeof(int));
							(void) galloc((void**) &new_callbacks, mappings_length, sizeof(inotify_watch_callback_T));
							(void) galloc((void**) &new_passthroughs, mappings_length, sizeof(void*));
							memcpy(new_other_wds, other_wds, index * sizeof(int));
							memcpy(new_callbacks, callbacks, index * sizeof(inotify_watch_callback_T));
							memcpy(new_passthroughs, passthroughs, index * sizeof(void*));
							memcpy(&(new_other_wds[index]), &(other_wds[index + 1]), (mappings_length - index) * sizeof(int));
							memcpy(&(new_callbacks[index]), &(callbacks[index + 1]), (mappings_length - index) * sizeof(inotify_watch_callback_T));
							memcpy(&(new_passthroughs[index]), &(passthroughs[index + 1]), (mappings_length - index) * sizeof(void*));
						}
						(void) gfree(other_wds);
						(void) gfree(callbacks);
						(void) gfree(passthroughs);
						other_wds = new_other_wds;
						callbacks = new_callbacks;
						passthroughs = new_passthroughs;

						glog(DEBUG, "Removed a file watcher"); // We sure did
					}
					break;
				}
				case CLOSE: {
					glog(INFO, "Stopping directory watcher...");
					goto thread_exit;
				}
			}
		}

		// Handle inotify events
		if(FD_ISSET(inotify_fd, &set)) {
			(void) galloc((void**) &event, needed, sizeof(char));
			if(read(inotify_fd, event, needed) == -1)
				glogf(ERROR, "Failed to read from inotify pipe: %s", strerror(errno));

			size_t index = find(event->wd, other_wds, mappings_length);
			if(index == SIZE_MAX)
				glogf(ERROR, "Could not find fd %i in watched file mappings", event->wd);
			else {
				callbacks[index](event, main_thread_fd, passthroughs[index]);
			}

			(void) gfree(event);
		}
	}
thread_exit:

	for(size_t i = 0; i < mappings_length; i++) {
		if(inotify_rm_watch(inotify_fd, other_wds[i]) == -1)
			glogf(ERROR, "Failed to remove inotify watcher: %s", strerror(errno));
	}
	(void) gfree(other_wds);
	(void) gfree(callbacks);
	(void) gfree(passthroughs);

	return 0;
}

/**
 * Callback for an inotify event on the current project root directory
 */
static void root_modified(struct inotify_event* event, int main_comm_fd, void* pass) {
	GEN_FRAME_BEGIN(root_modified);

	(void) event;
	(void) main_comm_fd;
	(void) pass;

	tree_view_refresh();
}

/**
 * Start a thread to watch the root of the current project
 * Updates the directory tree view on change
 * Path derived from the global state directory tree (`root`)
 * Add additional files to be watched with `file_add_inotify_handle`
 * @param comm_pipe an fd for communicating with the main thread
 * @return a handle to the created thread
 */
thrd_t file_begin_inotify_watcher(int comm_pipe) {
	GEN_FRAME_BEGIN(file_begin_inotify_watcher);

	main_thread_fd = comm_pipe;
	// Initiate the directory stalker!
	inotify_fd = inotify_init();
	// Create a pipe for communicating with the thread
	if(pipe(inotify_watch_thread_fd) == -1)
		glogf(ERROR, "Failed to create thread communication pipe: %s", strerror(errno));
	else {
		// Create the watcher thread
		thrd_t thread = 0;
		if(thrd_create(&thread, inotify_watcher_func, NULL))
			glogf(ERROR, "Failed to create watcher thread: %s", strerror(errno));
		else {
			// Add a watcher for directory root
			file_add_inotify_handle(root->data, IN_MOVED_TO | IN_DELETE | IN_CREATE, root_modified, NULL);
			return thread;
		}
	}

	return 0;
}

/**
 * Sends a message to the inotify watcher thread
 * @param message the message data to send
 */
static void file_send_inotify_message(thread_message_T* message) {
	GEN_FRAME_BEGIN(file_send_inotify_message);

	if(write(inotify_watch_thread_fd[PIPE_WRITE], message, sizeof(thread_message_T)) == -1)
		glogf(ERROR, "Failed to write message value to thread communication pipe: %s", strerror(errno));
}

/**
 * Adds a file to the list of monitored files in the inotify watcher thread
 * @param path the path of the file/folder to watch
 * @param mask a bitmask of inotify event flags to watch for
 * @param callback the callback function to be called when the event triggers
 * @param pass a user parameter to be passed to the callback
 * @return the watch handle for inotify
 */
int file_add_inotify_handle(char* path, uint32_t mask, inotify_watch_callback_T callback, void* pass) {
	GEN_FRAME_BEGIN(file_add_inotify_handle);

	int return_fd[2] = {0};
	pipe(return_fd);

	thread_message_T message;
	memset(&message, 0, sizeof(thread_message_T));
	message.type = ADD;
	message.path = path;
	message.mask = mask;
	message.ret_fd = return_fd[PIPE_WRITE];
	message.callback = callback;
	message.pass = pass;
	file_send_inotify_message(&message);

	// Keep a handle on things!
	int wd = 0;
	if(read(return_fd[PIPE_READ], &wd, sizeof(int)) == -1)
		glogf(ERROR, "Failed to read return value from thread communication pipe: %s", strerror(errno));
	if(close(return_fd[PIPE_READ]) == -1)
		glogf(ERROR, "Failed to close thread communuication pipe: %s", strerror(errno));
	if(close(return_fd[PIPE_WRITE]) == -1)
		glogf(ERROR, "Failed to close thread communuication pipe: %s", strerror(errno));
	return wd;
}

/**
 * Removes a file from the list of monitored files in the inotfy watcher thread
 * @param wd the watch handle of the file to remove
 */
void file_remove_inotify_handle(int wd) {
	GEN_FRAME_BEGIN(file_remove_inotify_handle);

	thread_message_T message;
	memset(&message, 0, sizeof(thread_message_T));
	message.type = REMOVE;
	message.wd = wd;

	file_send_inotify_message(&message);
}

/**
 * Stops the inotify watcher thread
 */
void file_stop_inotify_root(void) {
	GEN_FRAME_BEGIN(file_stop_inotify_root);

	thread_message_T message;
	memset(&message, 0, sizeof(thread_message_T));
	message.type = CLOSE;

	file_send_inotify_message(&message);
}

GEN_DIAG_REGION_END
