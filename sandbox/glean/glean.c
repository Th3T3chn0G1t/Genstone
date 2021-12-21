// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+glean@pm.me>

#include "include/glean.h"

/**
 * Wrapper for `g_signal_connect`
 * Handles bad return value
 * @param instance the instance whose signal should be connected
 * @param signal the signal name to connect
 * @param callback a callback function for handling the signal
 * @param pass user data to pass to the callback
 */
void glean_signal_connect(GObject* instance, char* signal, GCallback callback, void* pass) {
	GEN_FRAME_BEGIN(glean_signal_connect);

	if(!g_signal_connect(instance, signal, callback, pass))
		glogf(ERROR, "Failed to connect signal %s", signal);
}

/**
 * Wrapper for `g_signal_connect_after`
 * Handles bad return value
 * @param instance the instance whose signal should be connected
 * @param signal the signal name to connect
 * @param callback a callback function for handling the signal
 * @param pass user data to pass to the callback
 */
void glean_signal_connect_after(GObject* instance, char* signal, GCallback callback, void* pass) {
	GEN_FRAME_BEGIN(glean_signal_connect_after);

	if(!g_signal_connect_after(instance, signal, callback, pass))
		glogf(ERROR, "Failed to connect signal %s", signal);
}

/**
 * Wrapper for `g_signal_connect_swapped`
 * Handles bad return value
 * @param instance the instance whose signal should be connected
 * @param signal the signal name to connect
 * @param callback a callback function for handling the signal
 * @param pass user data to pass to the callback
 */
void glean_signal_connect_swapped(GObject* instance, char* signal, GCallback callback, void* pass) {
	GEN_FRAME_BEGIN(glean_signal_connect_swapped);

	if(!g_signal_connect_swapped(instance, signal, callback, pass))
		glogf(ERROR, "Failed to connect signal %s", signal);
}

/**
 * Appends a new item to the end of a vector
 * @param vector the vector to append to
 * @param data the data to append
 */
void vector_append(vector_T* vector, void* data) {
	GEN_FRAME_BEGIN(vector_append);

	(void) grealloc((void**) &vector->members, ++vector->n_members, sizeof(void*));
	vector->members[vector->n_members - 1] = data;
}
/**
 * Removes an item from a vector and shifts down proceeding members into the vacancy
 * @param vector the vector to remove from
 * @param index the index of the data to remove
 */
void vector_remove(vector_T* vector, size_t index) {
	GEN_FRAME_BEGIN(vector_remove);

	void** new_members = NULL;
	if(--(vector->n_members)) {
		(void) galloc((void**) &new_members, vector->n_members, sizeof(void*));
		memcpy(new_members, vector->members, index * sizeof(void*));
		memcpy(&(new_members[index]), &(vector->members[index + 1]), (vector->n_members - index) * sizeof(void*));
	}
	(void) gfree(vector->members);
	vector->members = new_members;
}

/**
 * Callback for the closure of the main window, cleans up the widget tree and exits Gtk
 */
static void shutdown(GtkWidget* widget, gpointer pass) {
	GEN_FRAME_BEGIN(shutdown);

	(void) pass;

	glog(INFO, "Stopping...");
	glog(INFO, "Cleaning up...");

	// Don't forget to clean up application state
	directory_tree_free();
	editor_cleanup();
	config_cleanup();

	gtk_widget_destroy(widget);
	gtk_main_quit();
}

/**
 * `GIOFunc` for messages from the inotify watcher thread to gtk-inhabited main thread
 */
static int inotify_watcher_message(GIOChannel* source, GIOCondition condition, void* pass) {
	GEN_FRAME_BEGIN(inotify_watcher_message);

	(void) condition;
	(void) pass;

	int fd = g_io_channel_unix_get_fd(source);
	main_thread_message_T message;
	if(read(fd, &message, sizeof(main_thread_message_T)) == -1)
		glogf(ERROR, "Failed to read value from thread communication pipe: %s", strerror(errno));

	switch(message.type) {
		case PROMPT: {
			editor_show_prompt(message.title, message.child, message.n_buttons, message.button_labels, message.response_ids, message.callback, message.pass);
			break;
		}
	}

	return true;
}

int main(int argc, char* argv[], char* envp[]) {
	GEN_FRAME_BEGIN(main);

	glog(INFO, "Starting...");

	clock_t begin;
	if((begin = clock()) == -1)
		glog(ERROR, "Unable to get time with `clock()` for some reason");

	GError* error = NULL;
	if(!gtk_init_with_args(&argc, &argv, "Nothing yet", NULL, NULL, &error)) {
		glogf(FATAL, "Failed to initialize GTK: %s", error->message);
		GEN_REQUIRE_NO_REACH;
	}

	// Load the configs!
	config_init();

	GtkBuilder* builder = gtk_builder_new();

	error = NULL;
	if(!gtk_builder_add_from_file(builder, "sandbox/glean/res/interface/eui.glade", &error)) {
		glogf(FATAL, "Failed to load UI from file: %s", error->message);
		GEN_REQUIRE_NO_REACH;
	}

	// Make GTK exit on main window closure
	glean_signal_connect(G_OBJECT(gtk_builder_get_object(builder, "window")), "destroy", G_CALLBACK(shutdown), NULL);

	// Setup the editor state
	editor_init(builder);

	// Create an in-memory file-tree for the sidebar and inotify
	GtkTreeView* tree_view = GTK_TREE_VIEW(gtk_builder_get_object(builder, "file_tree"));
	directory_tree_refresh(".");
	tree_view_populate(tree_view, false);

	int inotify_watch_thread_pipe_inward[2];
	if(pipe(inotify_watch_thread_pipe_inward) == -1)
		glogf(ERROR, "Failed to create thread communication pipe: %s", strerror(errno));

	GIOChannel* channel = g_io_channel_unix_new(inotify_watch_thread_pipe_inward[PIPE_READ]);
	g_io_add_watch(channel, G_IO_IN, inotify_watcher_message, NULL);

	// Start watching root for changes, and refresh the directory tree on changes
	thrd_t inotify_watch_thread = file_begin_inotify_watcher(inotify_watch_thread_pipe_inward[PIPE_WRITE]);

	// Start the inline terminal
	char* args[] = {getenv("SHELL"), NULL};
	vte_terminal_spawn_async(VTE_TERMINAL(gtk_builder_get_object(builder, "terminal")), VTE_PTY_DEFAULT, ".", args, envp, G_SPAWN_DEFAULT, NULL, NULL, NULL, -1, NULL, NULL, NULL);

	// Don't need the builder anymore
	g_object_unref(builder);

	glogf(INFO, "Started in %f seconds", (double) (clock() - begin) / CLOCKS_PER_SEC);

	// Let gtk enter it's main loop and wreak havoc across the land
	gtk_main();

	// Stop the inotify watcher thread and wait for it to terminate
	file_stop_inotify_root();
	thrd_join(inotify_watch_thread, NULL);
}
