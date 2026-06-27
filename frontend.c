#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

#include "session.h"

typedef struct {
  GtkWidget *entry_topic;
  GtkWidget *entry_goal;
  GtkWidget *entry_session_length;
  GtkWidget *result_label;
} AppWidgets;

static void start_session(GtkButton *button, gpointer user_data)
{
  printf("Initiating Session");
  AppWidgets *widgets = user_data;
  const char *topic = gtk_entry_get_text(GTK_ENTRY(widgets->entry_topic));
  const char *goal  = gtk_entry_get_text(GTK_ENTRY(widgets->entry_goal));

  char msg[256];
  g_snprintf(msg, sizeof(msg), "Session Started\ntopic: %s\ngoal: %s", topic, goal);
  gtk_label_set_text(GTK_LABEL(widgets->result_label), msg);
  g_print("set label to: %s\n", msg);
  g_print("ptr: %p\n", (void*)widgets->result_label);
  g_free(widgets);
}

static void insert_session(GtkButton *button, gpointer user_data)
{
  AppWidgets *widgets = user_data;
  const char *topic = gtk_entry_get_text(GTK_ENTRY(widgets->entry_topic));
  const char *goal  = gtk_entry_get_text(GTK_ENTRY(widgets->entry_goal));
  const char *session_length = gtk_entry_get_text(GTK_ENTRY(widgets->entry_session_length));

  char msg[256];
  g_snprintf(msg, sizeof(msg), "Inserted Session\ntopic: %s\ngoal: %s\nSession Length: %s", topic, goal, session_length);
  gtk_label_set_text(GTK_LABEL(widgets->result_label), msg);

  // Please segfault stop hurting me
  g_free(widgets);
}

// Creating the Automatic Tab
static void automatic_tab(GtkWidget *window, GtkNotebook *notebook)
{
  AppWidgets *widgets = g_new0(AppWidgets, 1);
  GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *heading = gtk_label_new("Automatic Insert");
  GtkWidget *entry = gtk_entry_new();

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(page), vbox);
  
  GtkWidget *topic_prompt = gtk_label_new("What is the topic of this session?");
  gtk_box_pack_start(GTK_BOX(vbox), topic_prompt, FALSE, FALSE, 0);

  widgets->entry_topic = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_topic), "Enter Topic");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_topic, FALSE, FALSE, 0);
  
  GtkWidget *goal_prompt = gtk_label_new("What is the goal of this session?");
  gtk_box_pack_start(GTK_BOX(vbox), goal_prompt, FALSE, FALSE, 0);
  
  widgets->entry_goal = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_goal), "Enter Goal");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_goal, FALSE, FALSE, 0);
  
  // Creating a start button to the session
  GtkWidget *start_button = gtk_button_new_with_label("Start Session");
  gtk_box_pack_start(GTK_BOX(vbox), start_button, FALSE, FALSE, 0);
  
  widgets->result_label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->result_label, FALSE, FALSE, 0);
  
  // Connecting the start_button to start_session
  // Once start_button is clicked invoke start_session
  /* g_object_set_data_full(G_OBJECT(window), "auto_widgets", widgets, g_free); */

  g_signal_connect_swapped(start_button, "destroy", G_CALLBACK(g_free), widgets);
  g_signal_connect(start_button, "clicked", G_CALLBACK(start_session), widgets);
  
  int index = gtk_notebook_append_page(notebook, page, gtk_label_new("Automatic"));
  gtk_widget_show_all(page);
}

static void manual_tab(GtkWidget *window, GtkNotebook *notebook)
{
  AppWidgets *widgets = g_new0(AppWidgets, 1);
  GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *heading = gtk_label_new("Manual Insert");
  GtkWidget *entry = gtk_entry_new();

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(page), vbox);
  
  GtkWidget *topic_prompt = gtk_label_new("What is the topic of this session?");
  gtk_box_pack_start(GTK_BOX(vbox), topic_prompt, FALSE, FALSE, 0);

  widgets->entry_topic = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_topic), "Enter Topic");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_topic, FALSE, FALSE, 0);
  
  GtkWidget *goal_prompt = gtk_label_new("What is the goal of this session?");
  gtk_box_pack_start(GTK_BOX(vbox), goal_prompt, FALSE, FALSE, 0);

  widgets->entry_goal = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_goal), "Enter Goal");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_goal, FALSE, FALSE, 0);

  GtkWidget *session_length_prompt = gtk_label_new("What was the session length? (HH:MM:SS)");
  gtk_box_pack_start(GTK_BOX(vbox), session_length_prompt, FALSE, FALSE, 0);

  widgets->entry_session_length = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_session_length), "Enter Session Length");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_session_length, FALSE, FALSE, 0);

  GtkWidget *insert_button = gtk_button_new_with_label("Insert Session");
  gtk_box_pack_start(GTK_BOX(vbox), insert_button, FALSE, FALSE, 0);

  widgets->result_label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->result_label, FALSE, FALSE, 0);

  g_signal_connect_swapped(insert_button, "destroy", G_CALLBACK(g_free), widgets);
  g_signal_connect(insert_button, "clicked", G_CALLBACK(insert_session), widgets);
  
  int index = gtk_notebook_append_page(notebook, page, gtk_label_new("Manual"));
  gtk_widget_show_all(page);
}

// Application Start Up
static void start_application(GtkApplication *app, gpointer user_data)
{
  AppWidgets *widgets = g_new0(AppWidgets, 1);
  
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Session Tracker");
  gtk_window_set_default_size(GTK_WINDOW(window), 320, 160);
  gtk_container_set_border_width(GTK_CONTAINER(window), 12);

  g_object_set_data_full(G_OBJECT(window), "widgets", widgets, g_free);
  
  GtkWidget *notebook = gtk_notebook_new();

  // Loading Tabs
  automatic_tab(window, GTK_NOTEBOOK(notebook));
  
  manual_tab(window, GTK_NOTEBOOK(notebook));
  
  gtk_container_add(GTK_CONTAINER(window), notebook);

  // Displaying Automatic first
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
  
  gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("com.example.timer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(start_application), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
