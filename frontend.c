#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

typedef struct {
  GtkWidget *entry_topic;
  GtkWidget *entry_goal;
  GtkWidget *result_label;
} AppWidgets;

static void start_session(GtkWidget *button, gpointer user_data)
{
  AppWidgets *app = (AppWidgets *)user_data;

  const char *text_topic = gtk_entry_get_text(GTK_ENTRY(app->entry_topic));
  const char *text_goal = gtk_entry_get_text(GTK_ENTRY(app->entry_goal));

  char msg[256];
  snprintf(msg, sizeof(msg), "Topic: %s\nGoal: %s", text_topic, text_goal);
  gtk_label_set_text(GTK_LABEL(app->result_label), msg);
}

// Starting Application
static void start_application(GtkApplication *app, gpointer user_data)
{
  AppWidgets *widgets = g_new0(AppWidgets, 1);
  
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Session Tracker");
  gtk_window_set_default_size(GTK_WINDOW(window), 320, 160);
  gtk_container_set_border_width(GTK_CONTAINER(window), 12);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // Adds a label 
  GtkWidget *topic_prompt = gtk_label_new("What is the topic of this session?");
  gtk_box_pack_start(GTK_BOX(vbox), topic_prompt, FALSE, FALSE, 0);

  // Adds a entry
  widgets->entry_topic = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_topic), "Enter Topic");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_topic, FALSE, FALSE, 0);

  // Adds a label
  GtkWidget *goal_prompt = gtk_label_new("What is the goal of this session?");
  gtk_box_pack_start(GTK_BOX(vbox), goal_prompt, FALSE, FALSE, 0);

  // Adds a entry
  widgets->entry_goal = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->entry_topic), "Enter Goal");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->entry_goal, FALSE, FALSE, 0);

  // Adds a button
  GtkWidget *start_button = gtk_button_new_with_label("Start Session");
  gtk_box_pack_start(GTK_BOX(vbox), start_button, FALSE, FALSE, 0);

  widgets->result_label = gtk_label_new("");
  gtk_box_pack_start(GTK_BOX(vbox), widgets->result_label, FALSE, FALSE, 0);
  
  g_signal_connect(start_button, "clicked", G_CALLBACK(start_session), widgets);

  g_signal_connect_swapped(window, "destroy", G_CALLBACK(g_free), widgets);

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
