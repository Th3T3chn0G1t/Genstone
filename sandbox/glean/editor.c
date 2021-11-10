// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+glean@pm.me>

#include "include/glean.h"

/**
 * The current state of the tab list
 * Order reflects the order in the Notebook
 * @see GtkNotebook
 * @see editor_tab_T
 */
vector_T* editor_tabs;

/**
 * The editor tab notebook
 */
static GtkNotebook* editor_notebook;

/**
 * The main application window
 */
static GtkWindow* window;

/**
 * Gets the currently open tab from the editor tab view
 * @return the current tab or `NULL` if no tab is open
 */
editor_tab_T* editor_tab_get_active(void) {
	GEN_FRAME_BEGIN(editor_tab_get_active);

	if(editor_tabs->n_members) return editor_tabs->members[gtk_notebook_get_current_page(editor_notebook)];
	return NULL;
}

/**
 * Shows a `GtkDialog` parented to the main window
 * @param title the title to use for the dialog window
 * @param child the widget to display in the dialog
 * @param n_buttons the number of user-defined buttons, or 0 for default layout
 * @param button_labels the button labels to use for dialog responses, or `NULL` for default layout
 * @param response_ids the response ids to use for buttons, or `NULL` for default layout
 * @param callback the function to call when a response is selected in the dialog. Should be the signature of the `GtkDialog` `"response"` signal
 * @param pass a passthrough for the response callback
 */
void editor_show_prompt(char* title, GtkWidget* child, size_t n_buttons, char** button_labels, int* response_ids, GCallback callback, void* pass) {
	GEN_FRAME_BEGIN(editor_show_prompt);

	GtkDialog* dialog;
	if(!button_labels || !response_ids || !n_buttons)
		dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(title, window, GTK_DIALOG_DESTROY_WITH_PARENT, "_CANCEL", GTK_RESPONSE_CANCEL, "_OK", GTK_RESPONSE_OK, NULL));
	else {
		dialog = GTK_DIALOG(gtk_dialog_new());
		for(size_t i = 0; i < n_buttons; i++)
			gtk_dialog_add_button(dialog, button_labels[i], response_ids[i]);
	}

	glean_signal_connect(G_OBJECT(dialog), "response", callback, pass);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dialog)), child);
	gtk_widget_show_all(GTK_WIDGET(dialog));
}

enum
{
	NEW_FILE,
	NEW_FOLDER,
	NEW_TEMPLATE
};

const static char* template_tags[] = {
	"$name", "$NAME",
	"$path", "$PATH",
	"$parent", "$PARENT",
	"$project", "$PROJECT"};

const static size_t template_tag_compare_lengths[] = {
	5, 5,
	5, 5,
	7, 7,
	8, 8};

enum
{
	TEMPLATE_TAG_name,
	TEMPLATE_TAG_NAME,
	TEMPLATE_TAG_path,
	TEMPLATE_TAG_PATH,
	TEMPLATE_TAG_parent,
	TEMPLATE_TAG_PARENT,
	TEMPLATE_TAG_project,
	TEMPLATE_TAG_PROJECT
};

/**
 * Replaces a section of a buffer with a string, resizing the buffer as appropriate
 * @param buffer a pointer to the buffer to replace in
 * @param buffer_len a pointer to the size of the buffer, modified if buffer length changes
 * @param mark the start of the region to replace
 * @param mark_len the length of the region to replace
 * @param data the data to insert
 */
static void _replace(char** buffer, size_t* buffer_len, char* mark, size_t mark_len, char* data) {
	GEN_FRAME_BEGIN(_replace);

	size_t data_len = strlen(data);
	ptrdiff_t diff = (ptrdiff_t) (data_len - mark_len);

	if(diff > 0) {
		*buffer_len += (size_t) diff;
		size_t off = (uintptr_t) mark - (uintptr_t) *buffer;
		(void) grealloc((void**) buffer, *buffer_len, sizeof(char));
		mark = *buffer + off;
	}
	memmove(mark + mark_len + diff, mark + mark_len, (uintptr_t) (*buffer + ((ptrdiff_t) *buffer_len - diff)) - (uintptr_t) (mark + mark_len));
	strncpy(mark, data, data_len);
	if(diff < 0) {
		*buffer_len += (size_t) diff;
		(void) grealloc((void**) buffer, *buffer_len, sizeof(char));
	}
}

/**
 * Converts a string to uppercase
 * @param str the string to convert
 * @return the converted string, allocated on the heap
 */
static char* strmkupper(char* str) {
	GEN_FRAME_BEGIN(strmkupper);

	str = strdup(str);
	for(size_t i = 0; i < strlen(str); i++) {
		if(str[i] >= 'a' && str[i] <= 'z') str[i] -= 32;
	}
	return str;
}

/**
 * `G_CALLBACK` for the new file dialog's response
 */
static void editor_prompt_new_file_exists_response_handler(GtkDialog* dialog, int response_id, void* pass) {
	GEN_FRAME_BEGIN(editor_prompt_new_file_exists_response_handler);

	(void) response_id;
	(void) pass;

	gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * `G_CALLBACK` for the new file dialog's response
 */
static void editor_prompt_new_response_handler(GtkDialog* dialog, int response_id, void* pass) {
	GEN_FRAME_BEGIN(editor_prompt_new_response_handler);

	if(response_id == GTK_RESPONSE_OK) {
		// The drop down representing the type choice
		GtkComboBox* combo = NULL;
		GtkComboBox* template_combo = NULL;
		// The text field representing the name for the new file
		GtkEntry* field = NULL;
		// Gtk magic to get members of the dialog
		GList* content = gtk_container_get_children(GTK_CONTAINER(gtk_dialog_get_content_area(dialog)));
		GList* children = gtk_container_get_children(content->data);
		g_list_free(content);
		for(; children; children = children->next) {
			if(children->data == pass)
				combo = GTK_COMBO_BOX(children->data);
			else if(GTK_IS_COMBO_BOX(children->data))
				template_combo = GTK_COMBO_BOX(children->data);
			else if(GTK_IS_BOX(children->data)) {
				GList* filebox_children = gtk_container_get_children(children->data);
				for(; filebox_children; filebox_children = filebox_children->next)
					if(GTK_IS_ENTRY(filebox_children->data)) field = GTK_ENTRY(filebox_children->data);
				g_list_free(filebox_children);
			}
		}
		g_list_free(children);

		char* name;
		const char* str = gtk_entry_buffer_get_text(gtk_entry_get_buffer(field));
		(void) gstrndup(&name, str, strlen(str));

		// Creation actions from the drop down
		switch(gtk_combo_box_get_active(combo)) {
			case NEW_FILE: {
				glogf(DEBUG, "Creating new file %s...", name);
				struct stat buf;
				if(stat(name, &buf)) {
					editor_show_prompt("Error", GTK_WIDGET(gtk_label_new("File already exists")), 0, NULL, NULL, G_CALLBACK(editor_prompt_new_file_exists_response_handler), NULL);
					break;
				}
				else {
					(void) gen_path_create_file(name);
				}
				break;
			}
			case NEW_FOLDER: {
				glogf(DEBUG, "Creating new folder %s...", name);
				if(mkdir(name, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH))
					glogf(ERROR, "Failed to create folder %s: %s", name, strerror(errno));
				break;
			}
			case NEW_TEMPLATE: {
				glogf(DEBUG, "Creating new file %s from template %s...", name, (char*) file_template_names->members[gtk_combo_box_get_active(template_combo)]);

				(void) gen_path_create_file(name);
				gen_filesystem_handle_t handle;
				(void) galloc((void**) &handle.path, GEN_PATH_MAX, sizeof(char));
				(void) gen_handle_open(&handle, name);

				char* buffer;
				char* str1 = file_templates->members[gtk_combo_box_get_active(template_combo)];
				(void) gstrndup(&buffer, str1, strlen(str));
				size_t buffer_len = strlen(buffer);

				// Keep a reference to all `\$` to replace with `$` at the end
				vector_T* escaped_marks;
				(void) gzalloc((void**) &escaped_marks, 1, sizeof(vector_T));

				char* extless_name = get_extensionless_filename(name);
				char* EXTLESS_NAME = strmkupper(extless_name);

				char* path = name;
				char* PATH = strmkupper(path);

				char* parent = basename(dirname(name));
				char* PARENT = strmkupper(parent);

				char* project = basename(root->data);
				char* PROJECT = strmkupper(project);

				// Find the first `$` in the buffer
				char* next_marker = strchr(buffer, '$');
				do {
					if(!next_marker || (next_marker == buffer && buffer[0] != '$')) break;
					if(next_marker != buffer && next_marker[-1] == '\\') {
						next_marker[0] = '~';
						vector_append(escaped_marks, (void*) ((uintptr_t) next_marker - (uintptr_t) buffer));
						continue;
					}

					// Big 'ol WoT to replace tags with the proper data

					if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_name], template_tag_compare_lengths[TEMPLATE_TAG_name]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_name], extless_name);
					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_NAME], template_tag_compare_lengths[TEMPLATE_TAG_NAME]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_NAME], EXTLESS_NAME);

					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_path], template_tag_compare_lengths[TEMPLATE_TAG_path]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_path], path);
					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_PATH], template_tag_compare_lengths[TEMPLATE_TAG_PATH]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_PATH], PATH);

					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_parent], template_tag_compare_lengths[TEMPLATE_TAG_parent]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_parent], parent);
					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_PARENT], template_tag_compare_lengths[TEMPLATE_TAG_PARENT]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_PARENT], PARENT);

					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_project], template_tag_compare_lengths[TEMPLATE_TAG_project]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_project], project);
					else if(!strncmp(next_marker, template_tags[TEMPLATE_TAG_PROJECT], template_tag_compare_lengths[TEMPLATE_TAG_PROJECT]))
						_replace(&buffer, &buffer_len, next_marker, template_tag_compare_lengths[TEMPLATE_TAG_PROJECT], PROJECT);
				} while((next_marker = strchr(buffer, '$')));

				// Cleaning up all our garbage

				(void) gfree(extless_name);
				(void) gfree(EXTLESS_NAME);

				(void) gfree(PATH);

				(void) gfree(PARENT);

				(void) gfree(PROJECT);

				for(size_t i = 0; i < escaped_marks->n_members; i++) {
					buffer[(size_t) escaped_marks->members[i]] = '$';
				}
				(void) gfree(escaped_marks);

				(void) gen_file_write(&handle, strlen(buffer), (uint8_t*) buffer);
				(void) gen_handle_close(&handle);

				(void) gfree(buffer);
				break;
			}
		}
		(void) gfree(name);
	}

	gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * `G_CALLBACK` for selections in the file-type combo box in the new file prompt
 */
static void editor_promt_new_combo_box_item_selected(GtkComboBox* box, void* pass) {
	GEN_FRAME_BEGIN(editor_promt_new_combo_box_item_selected);

	(void) pass;

	if(gtk_combo_box_get_active(box) == NEW_TEMPLATE) {
		GtkBox* frame = GTK_BOX(gtk_widget_get_parent(GTK_WIDGET(box)));

		GtkListStore* new_store = gtk_list_store_new(1, G_TYPE_STRING);
		GtkTreeIter new_iter;
		for(size_t i = 0; i < file_template_names->n_members; i++) {
			gtk_list_store_append(new_store, &new_iter);
			gtk_list_store_set(new_store, &new_iter, 0, file_template_names->members[i], -1);
		}

		// Magic to setup the combo box
		GtkComboBox* combo = GTK_COMBO_BOX(gtk_combo_box_new_with_model(GTK_TREE_MODEL(new_store)));
		g_object_unref(new_store);
		GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
		gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, true);
		gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);
		gtk_combo_box_set_active(combo, 0);

		gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(combo));
		gtk_container_add(GTK_CONTAINER(frame), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
		gtk_widget_show_all(GTK_WIDGET(frame));
	}
}

/**
 * `G_CALLBACK` for selecting the file location in the new file prompt
 */
static void editor_promt_new_location_selected(GtkButton* button, void* pass) {
	GEN_FRAME_BEGIN(editor_promt_new_location_selected);

	(void) button;

	GtkEntryBuffer* buffer = pass;

	GtkFileChooserDialog* dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new("New File...", window, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL));
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if(result == GTK_RESPONSE_ACCEPT) gtk_entry_buffer_set_text(buffer, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)), -1);
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * Prompts the user to create a new file
 * Implicitly uses the current root node as the default location
 */
void editor_show_new_prompt(void) {
	GEN_FRAME_BEGIN(editor_show_new_prompt);

	// Create a list of file creation options
	GtkListStore* store = gtk_list_store_new(1, G_TYPE_STRING);
	GtkTreeIter iter;
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, "File", -1);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, "Folder", -1);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, "Template...", -1);

	// Magic to setup the combo box
	GtkComboBox* combo = GTK_COMBO_BOX(gtk_combo_box_new_with_model(GTK_TREE_MODEL(store)));
	g_object_unref(store);
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, true);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), renderer, "text", 0, NULL);
	gtk_combo_box_set_active(combo, 0);
	glean_signal_connect(G_OBJECT(combo), "changed", G_CALLBACK(editor_promt_new_combo_box_item_selected), NULL);

	// Creates the buffer/field for the file name entry
	// Concatonate the file name and the destination with a path delimiter (`/`) in between
	size_t needed = (size_t) snprintf(NULL, 0, "%s/%s", root->data, "filename") + 1ul;
	char* name;
	(void) galloc((void**) &name, needed, sizeof(char));
	sprintf(name, "%s/%s", root->data, "filename");

	GtkEntryBuffer* buffer = gtk_entry_buffer_new(name, -1);
	(void) gfree(name);
	GtkEntry* field = GTK_ENTRY(gtk_entry_new_with_buffer(buffer));

	GtkButton* button = GTK_BUTTON(gtk_button_new_from_icon_name("folder", GTK_ICON_SIZE_BUTTON));
	glean_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(editor_promt_new_location_selected), buffer);

	GtkBox* filename_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));
	gtk_box_pack_start(filename_box, GTK_WIDGET(field), true, false, 5);
	gtk_box_pack_start(filename_box, GTK_WIDGET(button), true, false, 5);

	// The container for the prompt area
	GtkBox* box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 5));

	gtk_box_pack_start(box, GTK_WIDGET(filename_box), true, false, 2);
	gtk_box_pack_start(box, GTK_WIDGET(combo), true, false, 10);

	// Commence the dialog!
	editor_show_prompt("New File...", GTK_WIDGET(box), 0, NULL, NULL, G_CALLBACK(editor_prompt_new_response_handler), combo);
}

/**
 * Shows a "Save As" prompt (`GtkFileChooserDialog`)
 */
void editor_show_save_as_prompt(editor_tab_T* tab) {
	GEN_FRAME_BEGIN(editor_show_save_as_prompt);

	GtkFileChooserDialog* dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new("Save as...", window, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL));
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if(result == GTK_RESPONSE_ACCEPT) editor_tab_save_as(tab, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * Shows an "Open" prompt (`GtkFileChooserDialog`)
 */
void editor_show_open_prompt(void) {
	GEN_FRAME_BEGIN(editor_show_open_prompt);

	GtkFileChooserDialog* dialog = GTK_FILE_CHOOSER_DIALOG(gtk_file_chooser_dialog_new("Open a file...", window, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL));
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if(result == GTK_RESPONSE_ACCEPT) {
		char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		char* last_path_segment = basename(filename);
		editor_tab_open_from_file_with_id(filename, last_path_segment, filename);
	}
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * Opens a URL in the default browser application
 * @param url the URL to open
 */
void editor_open_url(const char* url) {
	GEN_FRAME_BEGIN(editor_open_url);

	GError* error = NULL;
	if(!gtk_show_uri_on_window(window, url, GDK_CURRENT_TIME, &error))
		glogf(ERROR, "Failed to open URi %s: %s", url, error->message);
}

/**
 * `G_CALLBACK` for a keypress in the main window
 * Key combinations handled here
 */
static gboolean editor_window_key_pressed(GtkWidget* widget, GdkEventKey* event, void* pass) {
	GEN_FRAME_BEGIN(editor_window_key_pressed);

	(void) widget;
	(void) pass;

	// Get the modifier keys (`ctrl` `shift` etc.)
	GdkModifierType modifiers = gtk_accelerator_get_default_mod_mask();

	editor_tab_T* current_tab = editor_tab_get_active();

	/// File

	// `ctrl + n` -> new
	if(event->keyval == GDK_n && (event->state & modifiers) == GDK_CONTROL_MASK) {
		editor_show_new_prompt();
		return true;
	}

	// `ctrl + o` -> open
	if(event->keyval == GDK_o && (event->state & modifiers) == GDK_CONTROL_MASK) {
		editor_show_open_prompt();
		return true;
	}

	// `ctrl + s` -> save
	if(event->keyval == GDK_s && (event->state & modifiers) == GDK_CONTROL_MASK && current_tab) {
		editor_tab_save(current_tab);
		return true;
	}
	// `shift + ctrl + s` -> save as
	if(event->keyval == GDK_S && (event->state & modifiers) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) {
		editor_show_save_as_prompt(current_tab);
		return true;
	}

	/// Edit

	// `ctrl + z` -> undo
	if(event->keyval == GDK_z && (event->state & modifiers) == GDK_CONTROL_MASK && current_tab) {
		editor_tab_undo_stack_move_back(current_tab);
		return true;
	}

	// `shift + ctrl + z` -> redo
	if(event->keyval == GDK_Z && (event->state & modifiers) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK) && current_tab) {
		editor_tab_undo_stack_move_forward(current_tab);
		return true;
	}

	if(event->keyval == GDK_j && (event->state & modifiers) == GDK_CONTROL_MASK) {
		editor_tab_open(GTK_WIDGET(GTK_LABEL(gtk_label_new("Test text for a test tab!"))), "A unique identifier", "Test");
		return true;
	}

	return false;
}

/**
 * Helper function
 * Shows a save as prompt for the current tab
 */
void editor_tab_active_show_save_as_prompt(void) {
	GEN_FRAME_BEGIN(editor_tab_active_show_save_as_prompt);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) editor_show_save_as_prompt(tab);
}

/**
 * Helper function
 * Saves the current tab
 */
void editor_tab_active_save(void) {
	GEN_FRAME_BEGIN(editor_tab_active_save);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) editor_tab_save(tab);
}

/**
 * Helper function
 * Moves back in the current undo stack state (undo)
 */
void editor_tab_active_undo_stack_move_back(void) {
	GEN_FRAME_BEGIN(editor_tab_active_undo_stack_move_back);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) editor_tab_undo_stack_move_back(tab);
}

/**
 * Helper function
 * Moves forward in the current undo stack state (redo)
 */
void editor_tab_active_undo_stack_move_forward(void) {
	GEN_FRAME_BEGIN(editor_tab_active_undo_stack_move_forward);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) editor_tab_undo_stack_move_forward(tab);
}

/**
 * Helper function
 * Cuts selected text in the active tab
 */
void editor_tab_active_cut(void) {
	GEN_FRAME_BEGIN(editor_tab_active_cut);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) g_signal_emit_by_name(tab->tab_view, "cut-clipboard");
}

/**
 * Helper function
 * Copies selected text in the active tab
 */
void editor_tab_active_copy(void) {
	GEN_FRAME_BEGIN(editor_tab_active_copy);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) g_signal_emit_by_name(tab->tab_view, "copy-clipboard");
}

/**
 * Helper function
 * pastes copied/cut text into the active tab
 */
void editor_tab_active_paste(void) {
	GEN_FRAME_BEGIN(editor_tab_active_paste);

	editor_tab_T* tab = editor_tab_get_active();
	if(tab) g_signal_emit_by_name(tab->tab_view, "paste-clipboard");
}

/**
 * Shows some information about the application
 */
void editor_show_about_prompt(void) {
	GEN_FRAME_BEGIN(editor_show_about_prompt);

	GtkBuilder* builder = gtk_builder_new();
	GError* error = NULL;
	if(!gtk_builder_add_from_file(builder, "sandbox/glean/res/interface/about.glade", &error))
		glogf(FATAL, "Failed to load UI from file: %s", error->message);
	glean_signal_connect(gtk_builder_get_object(builder, "about_diag"), "response", G_CALLBACK(gtk_widget_destroy), NULL);
	g_object_unref(builder);
}

/**
 * Initializes the editor state
 * @param builder the `GtkBuilder` for the main application, used to extract the menubar and notebook
 */
void editor_init(GtkBuilder* builder) {
	GEN_FRAME_BEGIN(editor_init);

	(void) gzalloc((void**) &editor_tabs, 1, sizeof(vector_T));
	editor_notebook = GTK_NOTEBOOK(gtk_builder_get_object(builder, "editor_notebook"));
	window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
	glean_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(editor_window_key_pressed), NULL);

	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_new"), "activate", G_CALLBACK(editor_show_new_prompt), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_open"), "activate", G_CALLBACK(editor_show_open_prompt), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_save"), "activate", G_CALLBACK(editor_tab_active_save), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_save_as"), "activate", G_CALLBACK(editor_tab_active_show_save_as_prompt), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_exit"), "activate", G_CALLBACK(gtk_widget_destroy), window);

	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_undo"), "activate", G_CALLBACK(editor_tab_active_undo_stack_move_back), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_redo"), "activate", G_CALLBACK(editor_tab_active_undo_stack_move_forward), NULL);

	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_cut"), "activate", G_CALLBACK(editor_tab_active_cut), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_copy"), "activate", G_CALLBACK(editor_tab_active_copy), NULL);
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_paste"), "activate", G_CALLBACK(editor_tab_active_paste), NULL);

	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_documentation"), "activate", G_CALLBACK(editor_open_url), "https://th3t3chn0g1t.github.io/glean/index.html");
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_github"), "activate", G_CALLBACK(editor_open_url), "https://github.com/Th3T3chn0G1t/glean");
	glean_signal_connect_swapped(gtk_builder_get_object(builder, "menubar_about"), "activate", G_CALLBACK(editor_show_about_prompt), NULL);
}

/**
 * Cleans up the editor state
 */
void editor_cleanup(void) {
	GEN_FRAME_BEGIN(editor_cleanup);

	for(size_t i = 0; i < editor_tabs->n_members; i++) {
		editor_tab_close(editor_tabs->members[i]);
	}
	if(editor_tabs->members) (void) gfree(editor_tabs->members);
	if(editor_tabs) (void) gfree(editor_tabs);
}

/**
 * Saves the text buffer of a tab to disk
 * @param tab the tab to save
 * @param path the file to save the buffer to
 */
void editor_tab_save_as(editor_tab_T* tab, char* path) {
	GEN_FRAME_BEGIN(editor_tab_save_as);

	// You can't save non-editable/non-text tabs
	if(!GTK_IS_TEXT_VIEW(tab->tab_view)) return;

	// Begin blocking FS-triggered operations
	tab->internal_update = true;

	// Remove the edited indicator from the tab
	GtkTextView* text_view = GTK_TEXT_VIEW(tab->tab_view);
	// Get the contents of the text buffer and write it to disk
	GtkTextBuffer* text_buffer = gtk_text_view_get_buffer(text_view);

	(void) gen_path_create_file(path);
	gen_filesystem_handle_t handle;
	(void) gzalloc((void**) &handle.path, GEN_PATH_MAX, sizeof(char));
	(void) gen_handle_open(&handle, path);

	if(tab->edited) {
		tab->edited = false;
		GtkGrid* grid = GTK_GRID(gtk_notebook_get_tab_label(editor_notebook, gtk_widget_get_parent(GTK_WIDGET(text_view))));
		gtk_container_remove(GTK_CONTAINER(grid), gtk_grid_get_child_at(grid, 2, 0));
	}

	GtkTextIter start;
	gtk_text_buffer_get_start_iter(text_buffer, &start);
	GtkTextIter end;
	gtk_text_buffer_get_end_iter(text_buffer, &end);
	char* buffer = gtk_text_buffer_get_text(text_buffer, &start, &end, false);

	(void) gen_file_write(&handle, strlen(buffer), (uint8_t*) buffer);
	(void) gfree(buffer);

	// Don't forget to clean up
	(void) gen_handle_close(&handle);

	// Finish blocking FS operations
	tab->internal_update = false;
}

/**
 * Update the displayed contents of a tab from disk
 * @param tab the tab to update
 * @param path the file to update from
 */
void editor_tab_update_contents_from_file(editor_tab_T* tab, char* path) {
	GEN_FRAME_BEGIN(editor_tab_update_contents_from_file);

	// You can't update non-editable/non-text tabs
	if(!GTK_IS_TEXT_VIEW(tab->tab_view)) return;

	gen_filesystem_handle_t handle;
	(void) gzalloc((void**) &handle.path, GEN_PATH_MAX, sizeof(char));
	(void) gen_handle_open(&handle, path);
	size_t file_buffer_size;
	(void) gen_handle_size(&file_buffer_size, &handle);
	char* file_buffer;
	(void) galloc((void**) &file_buffer, file_buffer_size + 1, sizeof(char));
	(void) gen_file_read((uint8_t*) file_buffer, &handle, 0, file_buffer_size);
	file_buffer[file_buffer_size] = '\0';
	(void) gen_handle_close(&handle);

	tab->internal_update = true;

	// Set the buffer to the updated data
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(tab->tab_view)), file_buffer, (int) file_buffer_size);

	(void) gfree(file_buffer);
}

/**
 * Gets the index of a tab
 * @param tab the tab to find
 * @return the index of the tab, or `SIZE_MAX` if not found
 */
size_t editor_tab_find(editor_tab_T* tab) {
	GEN_FRAME_BEGIN(editor_tab_find);

	GtkWidget* page = gtk_widget_get_parent(tab->tab_view);
	while(!GTK_IS_SCROLLED_WINDOW(page))
		page = gtk_widget_get_parent(page);

	return (size_t) gtk_notebook_page_num(editor_notebook, page);
}

void editor_tab_close(editor_tab_T* tab) {
	GEN_FRAME_BEGIN(editor_tab_close);

	// Remove the file watcher
	if(GTK_IS_TEXT_VIEW(tab->tab_view))
		file_remove_inotify_handle(tab->wd);

	size_t tab_index = editor_tab_find(tab);
	if(tab_index == SIZE_MAX) glogf(FATAL, "Failed to find tab %s for closure", tab->name);

	// Free the tab from the state
	for(size_t i = 0; i < tab->undo_stack->n_members; i++) {
		(void) gfree(((undo_stack_node_T*) tab->undo_stack->members[i])->data);
		(void) gfree(tab->undo_stack->members[i]);
	}
	if(tab->undo_stack->members) (void) gfree(tab->undo_stack->members);
	if(tab->undo_stack) (void) gfree(tab->undo_stack);
	if(tab->full_identifier) (void) gfree(tab->full_identifier);
	if(tab->name) (void) gfree(tab->name);
	if(tab) (void) gfree(tab);

	vector_remove(editor_tabs, tab_index);

	gtk_notebook_remove_page(editor_notebook, (int) tab_index);
}

/**
 * `G_CALLBACK` for the close button of editor tabs
 */
static void editor_tab_close_button_clicked(GtkWidget* widget, void* pass) {
	GEN_FRAME_BEGIN(editor_tab_close_button_clicked);

	(void) widget;

	// Find the index of the tab to close
	size_t tab_index = 0;
	for(; tab_index < editor_tabs->n_members; tab_index++)
		if(!strcmp(((editor_tab_T*) editor_tabs->members[tab_index])->full_identifier, pass)) break;

	editor_tab_close(editor_tabs->members[tab_index]);
}

/**
 * Marks a tab as edited and shows an edited indicator
 */
void editor_tab_mark_edited(editor_tab_T* tab) {
	GEN_FRAME_BEGIN(editor_tab_mark_edited);

	// If the tab does not already have the edited indicator
	if(!tab->edited) {
		tab->edited = true;
		// Show an indicator on unsaved edited files
		GtkGrid* grid = GTK_GRID(gtk_notebook_get_tab_label(editor_notebook, gtk_widget_get_parent(GTK_WIDGET(tab->tab_view))));
		GtkImage* edited_icon = GTK_IMAGE(gtk_image_new_from_icon_name("emblem-new", GTK_ICON_SIZE_SMALL_TOOLBAR));
		gtk_grid_attach(grid, GTK_WIDGET(edited_icon), 2, 0, 1, 1);
		gtk_widget_show(GTK_WIDGET(edited_icon));
	}
}

/**
 * `G_CALLBACK` for edits made to an editor tab buffer
 */
static void editor_buffer_changed(GtkTextBuffer* buffer, void* pass) {
	GEN_FRAME_BEGIN(editor_buffer_changed);

	(void) buffer;
	(void) pass;

	editor_tab_T* tab = editor_tab_get_active();
	if(!tab) return;

	// Don't update edited status for internal updates
	if(tab->internal_update) {
		tab->internal_update = false;
		return;
	}

	// Get the current tab - you can't edit non-open tabs
	editor_tab_mark_edited(tab);
}

#ifndef MAX_RUNNING_ADDITION_SEQUENCE
#define MAX_RUNNING_ADDITION_SEQUENCE 10
#endif

/**
 * The length of the running sequence of added characters
 */
static size_t running_addition_sequence_len = 0;
/**
 * The position of the running sequence of added characters
 */
static size_t running_addition_sequence_pos = 0;
/**
 * The running sequence of added characters
 */
static char* running_addition_sequence = NULL;
/**
 * `G_CALLBACK` for additions in the editor tab buffer
 */
static void editor_buffer_addition(GtkTextBuffer* buffer, GtkTextIter* location, char* text, int len, void* pass) {
	GEN_FRAME_BEGIN(editor_buffer_addition);

	(void) buffer;
	(void) pass;

	editor_tab_T* tab = editor_tab_get_active();
	if(!tab) return;

	// Don't push back for internal updates
	if(tab->internal_update) {
		tab->internal_update = false;
		return;
	}

	if(len == 1) {
		if(!running_addition_sequence_len)
			running_addition_sequence_pos = (size_t) gtk_text_iter_get_offset(location);

		(void) grealloc((void**) &running_addition_sequence, ++running_addition_sequence_len + 1, sizeof(char)); // Catch the NULL-terminator
		running_addition_sequence[running_addition_sequence_len - 1] = *text;
		if(running_addition_sequence_len == MAX_RUNNING_ADDITION_SEQUENCE || *text == '\n' || *text == '\t' || *text == ' ') {
			// Pushes back a new undo stack node for the sequence
			undo_stack_node_T* seq_node;
			(void) galloc((void**) &seq_node, 1, sizeof(undo_stack_node_T));
			seq_node->position = running_addition_sequence_pos;
			seq_node->n_changed = (long) running_addition_sequence_len;
			(void) gstrndup(&seq_node->data, running_addition_sequence, strlen(running_addition_sequence));
			editor_tab_undo_stack_push(tab, seq_node);
			running_addition_sequence_len = 0;
			running_addition_sequence_pos = SIZE_MAX;
			(void) gfree(running_addition_sequence);
			running_addition_sequence = NULL;
		}
	}
	else {
		// Pushes back a new undo stack node for the sequence
		undo_stack_node_T* seq_node;
		(void) galloc((void**) &seq_node, 1, sizeof(undo_stack_node_T));
		seq_node->position = running_addition_sequence_pos;
		seq_node->n_changed = (long) running_addition_sequence_len;
		(void) gstrndup(&seq_node->data, running_addition_sequence, running_addition_sequence_len);
		editor_tab_undo_stack_push(tab, seq_node);
		running_addition_sequence_len = 0;
		running_addition_sequence_pos = SIZE_MAX;
		(void) gfree(running_addition_sequence);
		running_addition_sequence = NULL;

		// Pushes back a new undo stack node for the action
		undo_stack_node_T* node;
		(void) galloc((void**) &node, 1, sizeof(undo_stack_node_T));
		node->position = (size_t) gtk_text_iter_get_offset(location);
		node->n_changed = len;
		(void) gstrndup(&node->data, text, strlen(text));
		editor_tab_undo_stack_push(tab, node);
	}
}

/**
 * `G_CALLBACK` for deletions in the editor tab buffer
 */
static void editor_buffer_deletion(GtkTextBuffer* buffer, GtkTextIter* start, GtkTextIter* end, void* pass) {
	GEN_FRAME_BEGIN(editor_buffer_deletion);

	(void) pass;

	editor_tab_T* tab = editor_tab_get_active();
	if(!tab) return;

	// Don't push back for internal updates
	if(tab->internal_update) {
		tab->internal_update = false;
		return;
	}

	// Pushes back a new undo stack node
	size_t offset_start = (size_t) gtk_text_iter_get_offset(start);
	size_t offset_end = (size_t) gtk_text_iter_get_offset(end);
	size_t len = offset_end - offset_start;
	char* text = gtk_text_buffer_get_text(buffer, start, end, false);
	undo_stack_node_T* node;
	(void) galloc((void**) &node, 1, sizeof(undo_stack_node_T));
	node->position = offset_start;
	node->n_changed = -((long) len);
	(void) gstrndup(&node->data, text, strlen(text));
	editor_tab_undo_stack_push(tab, node);
}

/**
 * Adds another node the the undo stack of a tab
 * @param tab the tab whose undo stack should be modified
 * @param node the node to push onto the stack
 */
void editor_tab_undo_stack_push(editor_tab_T* tab, undo_stack_node_T* node) {
	GEN_FRAME_BEGIN(editor_tab_undo_stack_push);

	if(node->n_changed > 0)
		glogf(DEBUG, "Pushing back addition of %li characters: %s", node->n_changed, node->data);
	else if(node->n_changed < 0)
		glogf(DEBUG, "Pushing back removal of %li characters: %s", node->n_changed, node->data);

	if(tab->current_undo_stack_offset) {
		for(size_t i = tab->undo_stack->n_members - 1; i > (tab->undo_stack->n_members - tab->current_undo_stack_offset); i--) {
			(void) gfree(((undo_stack_node_T*) tab->undo_stack->members[i])->data);
			(void) gfree(tab->undo_stack->members[i]);
			vector_remove(tab->undo_stack, i);
		}

		tab->current_undo_stack_offset = 0;
	}
	vector_append(tab->undo_stack, node);
}

/**
 * Move state backwards in the undo stack of a tab
 * Effectively "undoes" the current action in the stack
 * @param tab the tab whose state should be moved
 */
void editor_tab_undo_stack_move_back(editor_tab_T* tab) {
	GEN_FRAME_BEGIN(editor_tab_undo_stack_move_back);

	// You can't undo on non-editable/non-text tabs
	if(!GTK_IS_TEXT_VIEW(tab->tab_view)) return;

	if(running_addition_sequence_len) {
		undo_stack_node_T* seq_node;
		(void) galloc((void**) &seq_node, 1, sizeof(undo_stack_node_T));
		seq_node->position = running_addition_sequence_pos;
		seq_node->n_changed = (long) running_addition_sequence_len;
		glogf(DEBUG, "Running addition sequence: %s", running_addition_sequence);
		(void) gstrndup(&seq_node->data, running_addition_sequence, strlen(running_addition_sequence));
		editor_tab_undo_stack_push(tab, seq_node);
		running_addition_sequence_len = 0;
		running_addition_sequence_pos = SIZE_MAX;
		(void) gfree(running_addition_sequence);
		running_addition_sequence = NULL;
	}

	// Don't go beyond the stack
	if(tab->undo_stack->n_members < 1) return;
	if((long) (tab->undo_stack->n_members - tab->current_undo_stack_offset) <= 0) return;

	// Don't push back another state for this
	tab->internal_update = true;

	// The current node, one deeper in the stack
	undo_stack_node_T* node = tab->undo_stack->members[tab->undo_stack->n_members - ++(tab->current_undo_stack_offset)];
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tab->tab_view));

	glogf(DEBUG, "Undoing to state at node head (aka. %zu) - %zu (%lu)", tab->undo_stack->n_members, tab->current_undo_stack_offset, tab->undo_stack->n_members - tab->current_undo_stack_offset);

	// Addition nodes
	if(node->n_changed > 0) {
		// Get the modified range
		GtkTextIter rm_start;
		gtk_text_buffer_get_iter_at_offset(buffer, &rm_start, (int) node->position);
		GtkTextIter rm_end;
		gtk_text_buffer_get_iter_at_offset(buffer, &rm_end, (int) node->position + (int) node->n_changed);

		char* data = gtk_text_buffer_get_text(buffer, &rm_start, &rm_end, false);
		(void) gstrndup(&data, node->data, strlen(node->data));

		glogf(DEBUG, "Undoing addition of %li characters at offset %zu from buffer start: %s", node->n_changed, node->position, data);
		gtk_text_buffer_delete(buffer, &rm_start, &rm_end);
	}
	// Removal nodes
	else if(node->n_changed < 0) {
		glogf(DEBUG, "Undoing removal of %li characters at offset %zu from buffer start: %s", node->n_changed, node->position, node->data);

		// Add the previously removed text
		GtkTextIter add_start;
		gtk_text_buffer_get_iter_at_offset(buffer, &add_start, (int) node->position);
		gtk_text_buffer_insert(buffer, &add_start, node->data, (int) node->n_changed);
	}

	editor_tab_mark_edited(tab);
}

/**
 * Move state forwards in the undo stack of a tab
 * Effectively "redoes" the next action in the stack
 * @param tab the tab whose state should be moved
 */
void editor_tab_undo_stack_move_forward(editor_tab_T* tab) {
	GEN_FRAME_BEGIN(editor_tab_undo_stack_move_forward);

	// You can't undo on non-editable/non-text tabs
	if(!GTK_IS_TEXT_VIEW(tab->tab_view)) return;

	// Don't go beyond the stack
	if(tab->undo_stack->n_members < 1) return;
	if((long) tab->current_undo_stack_offset <= 0) return;

	// Don't push back another state for this
	tab->internal_update = true;

	// The current node, one shallower in the stack
	undo_stack_node_T* node = tab->undo_stack->members[tab->undo_stack->n_members - tab->current_undo_stack_offset--];
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tab->tab_view));

	glogf(DEBUG, "Redoing to state at node head (aka. %zu) - %lu (%lu)", tab->undo_stack->n_members, tab->current_undo_stack_offset + 1, tab->undo_stack->n_members - (tab->current_undo_stack_offset + 1));

	// Addition nodes
	if(node->n_changed > 0) {
		glogf(DEBUG, "Redoing addition of %li characters at offset %zu from buffer start: %s", node->n_changed, node->position, node->data);

		// Add the previously removed (undone) text
		GtkTextIter add_start;
		gtk_text_buffer_get_iter_at_offset(buffer, &add_start, (int) node->position);
		gtk_text_buffer_insert(buffer, &add_start, node->data, (int) node->n_changed);
	}
	// Removal nodes
	else if(node->n_changed < 0) {
		// Get the modified range
		GtkTextIter rm_start;
		gtk_text_buffer_get_iter_at_offset(buffer, &rm_start, (int) node->position);
		GtkTextIter rm_end;
		gtk_text_buffer_get_iter_at_offset(buffer, &rm_end, (int) node->position + (int) node->n_changed);

		// Remove the text
		char* data = gtk_text_buffer_get_text(buffer, &rm_start, &rm_end, false);
		(void) gstrndup(&node->data, data, strlen(data));
		glogf(DEBUG, "Redoing removal of %li characters at offset %zu from buffer start: %s", node->n_changed, node->position, data);
		gtk_text_buffer_delete(buffer, &rm_start, &rm_end);
	}

	editor_tab_mark_edited(tab);
}

static char* preview_prefix = "(preview) ";
static void editor_prompt_file_update_response_handler(GtkDialog* dialog, int response_id, void* pass) {
	GEN_FRAME_BEGIN(editor_prompt_file_update_response_handler);

	editor_tab_T* tab = pass;

	switch(response_id) {
		/// View inbound changes
		case GTK_RESPONSE_YES: {
			// Prepend "(preview)" to id and name to distinguish from original file
			char* view_name;
			(void) gzalloc((void**) &view_name, strlen(preview_prefix) + strlen(tab->name) + 1, sizeof(char));
			strcat(view_name, preview_prefix);
			strcat(view_name, tab->name);

			char* view_id;
			(void) gzalloc((void**) &view_id, strlen(preview_prefix) + strlen(tab->full_identifier) + 1, sizeof(char));
			strcat(view_id, preview_prefix);
			strcat(view_id, tab->full_identifier);

			// Show the new file
			tab->preview = editor_tab_open_from_file_with_id(tab->full_identifier, view_name, view_id);

			(void) gfree(view_name);
			(void) gfree(view_id);

			break;
		}
		case GTK_RESPONSE_ACCEPT: {
			// Close the preview if open
			if(tab->preview)
				editor_tab_close(tab->preview);

			// Reopen the file, discarding changes
			char* path;
			(void) gstrndup(&path, tab->full_identifier, strlen(tab->full_identifier));
			char* name;
			(void) gstrndup(&name, tab->name, strlen(tab->name));
			char* id;
			(void) gstrndup(&id, tab->full_identifier, strlen(tab->full_identifier));
			editor_tab_close(tab);
			editor_tab_open_from_file_with_id(path, name, id);
			(void) gfree(path);
			(void) gfree(name);
			(void) gfree(id);

			gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
		}
		case GTK_RESPONSE_REJECT: {
			// Close the preview if open
			if(tab->preview)
				editor_tab_close(tab->preview);

			editor_tab_save(tab);

			gtk_widget_destroy(GTK_WIDGET(dialog));
			break;
		}
	}
}

/**
 * Callback for an inotify event on the underlying file for a tab
 */
static void editor_tab_file_updated(struct inotify_event* event, int main_comm_fd, void* pass) {
	GEN_FRAME_BEGIN(editor_tab_file_updated);

	editor_tab_T* tab = pass;

	// FS op blocks are unset by the caller
	if(tab->internal_update)
		return;

	glog(DEBUG, "Tab file on disk modified");
	if(event->mask & IN_MODIFY) {
		glogf(DEBUG, "Tab file contents updated on disk: %s", tab->name);
		gtk_notebook_set_current_page(editor_notebook, (int) editor_tab_find(tab));

		main_thread_message_T message;
		memset(&message, 0, sizeof(main_thread_message_T));
		message.type = PROMPT;
		message.title = "File contents updated";
		message.child = gtk_label_new("File has modified on disk");
		message.n_buttons = 3;
		message.button_labels[0] = "View";
		message.button_labels[1] = "Accept";
		message.button_labels[2] = "Reject";
		message.response_ids[0] = GTK_RESPONSE_YES;
		message.response_ids[1] = GTK_RESPONSE_ACCEPT;
		message.response_ids[2] = GTK_RESPONSE_REJECT;
		message.callback = G_CALLBACK(editor_prompt_file_update_response_handler);
		message.pass = tab;

		if(write(main_comm_fd, &message, sizeof(main_thread_message_T)) == -1)
			glogf(ERROR, "Failed to write message value to thread communication pipe: %s", strerror(errno));
	}
	else if(event->mask & (IN_DELETE_SELF | IN_MOVE_SELF)) {
		glogf(DEBUG, "Tab file contents deleted from disk: %s", tab->name);
	}
}

/**
 * `G_CALLBACK` for mouse button events
 * Tab view context menu handled here
 */
static bool editor_tab_view_button_pressed(GtkWidget* widget, GdkEvent* event, void* pass) {
	GEN_FRAME_BEGIN(editor_tab_view_button_pressed);

	(void) widget;
	(void) pass;

	GdkEventButton* button_press = (GdkEventButton*) event;
	if(button_press->button == GDK_BUTTON_SECONDARY) {
		GtkMenu* menu = GTK_MENU(gtk_menu_new());

		GtkMenuItem* undo = GTK_MENU_ITEM(gtk_menu_item_new_with_label("undo"));
		glean_signal_connect_swapped(G_OBJECT(undo), "activate", G_CALLBACK(editor_tab_active_undo_stack_move_back), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(undo), 0, 1, 1, 2);

		GtkMenuItem* redo = GTK_MENU_ITEM(gtk_menu_item_new_with_label("redo"));
		glean_signal_connect_swapped(G_OBJECT(redo), "activate", G_CALLBACK(editor_tab_active_undo_stack_move_forward), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(redo), 0, 1, 2, 3);

		gtk_menu_attach(menu, gtk_separator_menu_item_new(), 0, 1, 3, 4);

		GtkMenuItem* cut = GTK_MENU_ITEM(gtk_menu_item_new_with_label("cut"));
		glean_signal_connect_swapped(G_OBJECT(cut), "activate", G_CALLBACK(editor_tab_active_cut), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(cut), 0, 1, 4, 5);

		GtkMenuItem* copy = GTK_MENU_ITEM(gtk_menu_item_new_with_label("copy"));
		glean_signal_connect_swapped(G_OBJECT(copy), "activate", G_CALLBACK(editor_tab_active_copy), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(copy), 0, 1, 5, 6);

		GtkMenuItem* paste = GTK_MENU_ITEM(gtk_menu_item_new_with_label("paste"));
		glean_signal_connect_swapped(G_OBJECT(paste), "activate", G_CALLBACK(editor_tab_active_paste), NULL);
		gtk_menu_attach(menu, GTK_WIDGET(paste), 0, 1, 6, 7);

		gtk_widget_show_all(GTK_WIDGET(menu));
		gtk_menu_popup_at_pointer(menu, event);
		return true;
	}

	return false;
}

/**
 * Creates and shows a new base tab
 * @param child a widget containing the tab view
 * @param id a unique identifer for the created tab, used to prevent re-opening of existing tabs
 * @param name the display name for the tab label
 * @return a pointer to the created tab or an existing tab with the same ID if found, or `NULL` on error
 */
editor_tab_T* editor_tab_open(GtkWidget* child, char* id, char* name) {
	GEN_FRAME_BEGIN(editor_tab_open);

	// Do not open already open tabs
	for(size_t i = 0; i < editor_tabs->n_members; i++) {
		if(!strcmp(((editor_tab_T*) editor_tabs->members[i])->full_identifier, id)) {
			gtk_notebook_set_current_page(editor_notebook, (int) i);
			return editor_tab_get_active();
		}
	}

	// Otherwise, open the tab
	// Add the new tab and populate it's fields
	editor_tab_T* tab;
	(void) gzalloc((void**) &tab, 1, sizeof(editor_tab_T));
	(void) gstrndup(&tab->full_identifier, id, strlen(id));
	(void) gstrndup(&tab->name, name, strlen(name));
	tab->tab_view = child;
	vector_append(editor_tabs, tab);

	// Setup the tab label for the notebook to show
	GtkGrid* label = GTK_GRID(gtk_grid_new());
	GtkLabel* label_text = GTK_LABEL(gtk_label_new(tab->name));
	GtkButton* label_close_button = GTK_BUTTON(gtk_button_new_from_icon_name("window-close", 1));
	gtk_button_set_relief(GTK_BUTTON(label_close_button), GTK_RELIEF_NONE);
	gtk_grid_attach(label, GTK_WIDGET(label_text), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(label), GTK_WIDGET(label_close_button), 1, 0, 1, 1);
	gtk_widget_show_all(GTK_WIDGET(label));
	glean_signal_connect(G_OBJECT(label_close_button), "clicked", G_CALLBACK(editor_tab_close_button_clicked), tab->full_identifier);

	// The top level widget of all tabs is scrollable
	GtkScrolledWindow* scrolled_window = GTK_SCROLLED_WINDOW(gtk_scrolled_window_new(NULL, NULL));
	gtk_scrolled_window_set_policy(scrolled_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), child);

	// Add the new tab and show it
	int new_tab_index = gtk_notebook_append_page(editor_notebook, GTK_WIDGET(scrolled_window), GTK_WIDGET(label));
	gtk_widget_show_all(GTK_WIDGET(editor_notebook));

	gtk_notebook_set_current_page(editor_notebook, new_tab_index);

	return tab;
}

/**
 * Opens a new tab from a file
 * @param path the file to open
 * @param name the name to assign to the opened tab
 * @param id the id to assign to the opened tab
 */
editor_tab_T* editor_tab_open_from_file_with_id(char* path, char* name, char* id) {
	GEN_FRAME_BEGIN(editor_tab_open_from_file_with_id);

	// Setup the text view for the tab contents
	GtkTextBuffer* text_buffer = gtk_text_buffer_new(NULL);
	GtkTextView* text_view = GTK_TEXT_VIEW(gtk_text_view_new_with_buffer(text_buffer));

	size_t n_tabs = editor_tabs->n_members;
	editor_tab_T* tab = editor_tab_open(GTK_WIDGET(text_view), id, name);

	if(tab && n_tabs < editor_tabs->n_members) {
		(void) gzalloc((void**) &tab->undo_stack, 1, sizeof(vector_T));
		editor_tab_update_contents_from_file(tab, path);
		tab->internal_update = false;

		glean_signal_connect(G_OBJECT(text_buffer), "changed", G_CALLBACK(editor_buffer_changed), NULL);
		glean_signal_connect(G_OBJECT(text_buffer), "delete-range", G_CALLBACK(editor_buffer_deletion), NULL);
		glean_signal_connect(G_OBJECT(text_buffer), "insert-text", G_CALLBACK(editor_buffer_addition), NULL);
		glean_signal_connect(G_OBJECT(text_view), "button-press-event", G_CALLBACK(editor_tab_view_button_pressed), NULL);

		// Add an inotify listener for changes
		tab->wd = file_add_inotify_handle(path, IN_MODIFY | IN_MOVE_SELF | IN_DELETE_SELF, editor_tab_file_updated, tab);

		return tab;
	}
	else if(!tab) {
		glogf(ERROR, "Failed to open tab %s: %s from file %s", name, id, path);
	}
	g_object_unref(text_buffer);
	gtk_widget_destroy(GTK_WIDGET(text_view));

	return NULL;
}
