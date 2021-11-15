// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright(C) 2021 TTG <prs.ttg+glean@pm.me>

#ifndef GLEAN_H
#define GLEAN_H

#include <gendiag.h>

GEN_DIAG_REGION_BEGIN
GEN_DIAG_IGNORE_ALL

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms-compat.h>
#include <vte/vte.h>

#include <libgen.h>

#include <sys/inotify.h>

#include <gencommon.h>
#include <genfs.h>

GEN_DIAG_REGION_END

/**
 * Dynamic array type
 */
typedef struct {
	/**
	 * The current number of members in the vector
	 */
	size_t n_members;
	/**
	 * The members the vector is holding
	 */
	void** members;
} vector_T;

/**
 * A node in a file tree
 */
typedef struct {
	/**
	 * The data this node holds
	 * Should be the path to the node
	 */
	char* data;
	/**
	 * The child nodes of this node
	 */
	vector_T* children;
	/**
	 * Flag, set if this node represents a directory
	 */
	bool dir;
} tree_node_T;

/**
 * A node in an undo stack
 * Represents data for a state change in a buffer
 */
typedef struct {
	/**
	 * The start index of the changes in the buffer at the time of edit
	 */
	size_t position;
	/**
	 * The number of characters changed in the edit
	 * Can be negative to reflect the removal of characters
	 */
	long n_changed;
	/**
	 * If `n_changed` <0, holds the removed characters
	 */
	char* data;
} undo_stack_node_T;

/**
 * Metadata for an editor tab
 */
typedef struct {
	/**
	 * The display name of the tab
	 */
	char* name;
	/**
	 * A unique identifer for the tab to prevent opening multiple copies
	 * With file-sourced tabs, the full path to the source file of the tab
	 */
	char* full_identifier;
	/**
	 * Whether the tab has been edited since last save
	 */
	bool edited;
	/**
	 * Flag stating whether the next tab update is internal
	 * Should block update triggered events
	 */
	bool internal_update;
	/**
	 * The tab view accociated with this tab
	 */
	GtkWidget* tab_view;
	/**
	 * Inotify watch descriptor for the tab's file
	 */
	int wd;
	/**
	 * The number of nodes from the end of the undo stack the current state is
	 * The negated offset will be trimmed upwards from when the state is updated
	 */
	size_t current_undo_stack_offset;
	/**
	 * The data to revert to on undo/redo
	 */
	vector_T* undo_stack;
	/**
	 * A pointer to the change-previewing tab if open
	 */
	void* preview;
} editor_tab_T;

/**
 * An identification method for mapping a file to an icon
 */
typedef struct {
	/**
	 * The type of identification method to use
	 */
	enum { NAME, EXTENSION } type;
	/**
	 * The data to compare to the filename under method
	 * Only valid for `type` = `NAME` or `EXTENSION`
	 */
	char* data;
} icon_map_key_T;

/**
 * A mapping from file extensions to Gtk named icons
 */
typedef struct {
	/**
	 * The number of mappings
	 */
	size_t n_mappings;
	/**
	 * A list of file identification methods(name, extension etc.) for mapping to values
	 */
	icon_map_key_T* keys;
	/**
	 * A list of Gtk named icon names - values
	 */
	char** icons;
} icons_T;

#ifndef MAX_USER_PROMPT_BUTTONS
/**
 * The maximum number of user defined buttons allowed to be passed to a `main_thread_message_T` of type `PROMPT`
 */
#define MAX_USER_PROMPT_BUTTONS 6
#endif
/**
 * A message to the main gtk thread, to be handled by GIO
 */
typedef struct {
	/**
	 * The type of the messsage
	 * Reflects the action requested to perform
	 */
	enum { PROMPT } type;

	/**
	 * The title to use for the dialog window
	 * Only valid for `type` = `PROMPT`
	 */
	char* title;
	/**
	 * The widget to display in the dialog
	 * Only valid for `type` = `PROMPT`
	 */
	GtkWidget* child;
	/**
	 * The number of user-defined buttons, or 0 for default layout
	 * Only valid for `type` = `PROMPT`
	 */
	size_t n_buttons;
	/**
 	 * The button labels to use for dialog responses, or `NULL` for default layout
	 * Only valid for `type` = `PROMPT`
	 */
	char* button_labels[MAX_USER_PROMPT_BUTTONS];
	/**
	 * The response ids to use for buttons, or `NULL` for default layout
	 * Only valid for `type` = `PROMPT`
	 */
	int response_ids[MAX_USER_PROMPT_BUTTONS];
	/**
	 * The function to call when a response is selected in the dialog. Should be the signature of the `GtkDialog` `"response"` signal
	 * Only valid for `type` = `PROMPT`
	 */
	GCallback callback;
	/**
	 * A passthrough for the response callback
	 * Only valid for `type` = `PROMPT`
	 */
	void* pass;
} main_thread_message_T;

/**
 * Signature for inotify watch thread callbacks
 */
typedef void (*inotify_watch_callback_T) (struct inotify_event*, int, void*);

/**
 * Helper macro, the read end of a pipe as output by `pipe`
 */
#define PIPE_READ 0
/**
 * Helper macro, the write end of a pipe as output by `pipe`
 */
#define PIPE_WRITE 1

extern tree_node_T* root;
extern vector_T* editor_tabs;

extern icons_T* icon_map;
extern vector_T* file_template_names;
extern vector_T* file_templates;

/// Error wrapping functions

extern void glean_signal_connect(GObject* instance, char* signal, GCallback callback, void* pass);
extern void glean_signal_connect_after(GObject* instance, char* signal, GCallback callback, void* pass);
extern void glean_signal_connect_swapped(GObject* instance, char* signal, GCallback callback, void* pass);

// Vector operations

extern void vector_append(vector_T* vector, void* data);
extern void vector_remove(vector_T* vector, size_t index);

/// Path operations

extern char* get_extensionless_filename(const char* path);
extern char* get_home_relative_path(char* path);
extern char* get_path_from_home_relative(char* path);

/// Directory tree operations

extern void directory_tree_refresh(char* directory);
extern void tree_view_populate(GtkTreeView* view, bool refill);
extern void tree_view_refresh(void);
extern tree_node_T* directory_tree_get_node(tree_node_T* current, int* rel_path, int depth);
extern void directory_tree_free(void);

/// Inotify thread operations

extern pthread_t file_begin_inotify_watcher(int comm_pipe);
extern int file_add_inotify_handle(char* path, uint32_t mask, inotify_watch_callback_T callback, void* pass 	);
extern void file_remove_inotify_handle(int wd);
extern void file_stop_inotify_root(void);

/// Editor state operations

extern void editor_init(GtkBuilder* builder);
extern void editor_cleanup(void);

/// Various prompts

extern void editor_open_url(const char* url);
extern void editor_show_prompt(char* title, GtkWidget* child, size_t n_buttons, char** button_labels, int* response_ids, GCallback callback, void* pass);
extern void editor_show_new_prompt(void);
extern void editor_show_save_as_prompt(editor_tab_T* tab);
extern void editor_show_open_prompt(void);
extern void editor_show_about_prompt(void);

/// Editor tab operations

extern editor_tab_T* editor_tab_get_active(void);
extern editor_tab_T* editor_tab_open(GtkWidget* child, char* id, char* name);
extern editor_tab_T* editor_tab_open_from_file_with_id(char* path, char* name, char* id);
extern size_t editor_tab_find(editor_tab_T* tab);
extern void editor_tab_close(editor_tab_T* tab);
extern void editor_tab_update_contents_from_file(editor_tab_T* tab, char* path);
extern void editor_tab_undo_stack_push(editor_tab_T* tab, undo_stack_node_T* node);
extern void editor_tab_undo_stack_move_back(editor_tab_T* tab);
extern void editor_tab_undo_stack_move_forward(editor_tab_T* tab);
extern void editor_tab_save_as(editor_tab_T* tab, char* path);

extern void editor_tab_active_show_save_as_prompt(void);
extern void editor_tab_active_save(void);
extern void editor_tab_active_undo_stack_move_back(void);
extern void editor_tab_active_undo_stack_move_forward(void);
extern void editor_tab_active_cut(void);
extern void editor_tab_active_paste(void);
extern void editor_tab_active_copy(void);

/**
 * Convenience macro for `editor_tab_save_as`
 * Saves the text buffer of a tab to disk, using the tab ID as a filepath
 * @param tab the tab to save
 */
#define editor_tab_save(tab) tab->edited ? editor_tab_save_as(tab, tab->full_identifier) : (void) tab
extern void editor_tab_mark_edited(editor_tab_T* tab);

/// Config state operations

extern void config_init(void);
extern void config_cleanup(void);
#endif
