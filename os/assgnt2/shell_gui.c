#include <gtk/gtk.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

GtkWidget *window, *txtInput;

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int min_cursor_offset = 0;
char buf[256];
char output[256];
void gtk_text_buffer_append_prompt(GtkTextBuffer *buffer, int len);
void gtk_text_buffer_append_output(GtkTextBuffer *buffer, char *text, int len);
static gboolean txtinput_on_key_press(GtkWidget *widget,GdkEventKey *event);

void destroy(GtkWidget *window);
gboolean delete_event(GtkWidget*, GdkEvent*);

char **parse(char *string)
{
	int buff = 82;
	int pos = 0;
	char **arg = malloc(buff * sizeof(char *));
	char *input;

	if (!arg) {
		fprintf(stderr, "Error\n");
		exit(EXIT_FAILURE);
	}

	input = strtok(string, " \t\r\n\a\v\f");
	while (input != NULL) {
		arg[pos++] = input;
		if (pos >= buff) {
			buff += 80;
			arg = realloc(arg, buff * sizeof(char *));
			if (!arg) {
				fprintf(stderr, "Error\n");
				exit(EXIT_FAILURE);
			}
		}

		input = strtok(NULL, " \t\r\n\a\v\f");
	}
	arg[pos] = NULL;
	return arg;
}

int main(int argc, char *argv[])
{
	GtkWidget *scrollwin;
	GtkTextBuffer *buffer;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	buffer = gtk_text_buffer_new(NULL);
	txtInput = gtk_text_view_new_with_buffer(buffer);
	getcwd(buf, sizeof(buf));
	strcat(buf, "$ ");
	printf("%s\n", buf);
	gtk_text_buffer_append_prompt(GTK_TEXT_BUFFER(buffer), -1);
	scrollwin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrollwin), txtInput);
	gtk_container_add(GTK_CONTAINER(window), scrollwin);
	gtk_widget_set_size_request(window,600,400);
    gtk_window_set_title(GTK_WINDOW(window),"Terminal");
    g_signal_connect(G_OBJECT(txtInput),"key-press-event",G_CALLBACK(txtinput_on_key_press),NULL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(destroy),NULL);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(delete_event),NULL);
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
	// gtk_text_buffer_create_tag(buffer, "bold",  "weight", PANGO_WEIGHT_BOLD, NULL);
}

void gtk_text_buffer_append_prompt(GtkTextBuffer *buffer, int len)
{
	getcwd(buf, sizeof(buf));
	strcat(buf, "$ ");
	// printf("%s\n", buf);
	gtk_text_buffer_insert_at_cursor(buffer, buf, len);
	GtkTextIter iter;
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
	min_cursor_offset = gtk_text_iter_get_offset(&iter);
}

static gboolean txtinput_on_key_press(GtkWidget *widget, GdkEventKey *event)
{
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
	GtkTextIter current_iter, start_iter;
	gtk_text_buffer_get_iter_at_mark(buffer, &current_iter, gtk_text_buffer_get_insert(buffer));
	int current_offset = gtk_text_iter_get_offset(&current_iter);
	if(current_offset < min_cursor_offset)
		return TRUE;

	gtk_text_buffer_get_iter_at_offset(buffer, &start_iter, min_cursor_offset);

	char *text, *text2;

	asprintf(&text2, "");

	switch(event->keyval)
	{
		case 65293:
			text = gtk_text_buffer_get_text(buffer, &start_iter, &current_iter, FALSE);
			asprintf(&text2, "%s\n", text);

			gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
			break;

		case 65288: //for Backspace key
            if(current_offset == min_cursor_offset)
                return TRUE;
            break;

      //   case GDK_c:
	     //  	if (event->state & GDK_CONTROL_MASK)
		    // {
		    //   printf("key pressed: %s\n", "ctrl + c");
		    //   gtk_main_quit();
		    // }
		    // break;

	}
	if(strcmp(text2, "") == 0)
		return FALSE;

	char **arg = parse(text2);
	if(strcmp(arg[0], "exit") == 0)
	{
		gtk_main_quit();
		return TRUE;
	}
	int link[2];
	pid_t pid;
	char foo[4096];
	if (pipe(link)==-1)
	  die("pipe");
	if ((pid = fork()) == -1)
	  die("fork");
	if(pid == 0) {
	  dup2 (link[1], STDOUT_FILENO);
	  close(link[0]);
	  close(link[1]);
	  // char *myargs[3];
	  // myargs[0] = strdup("wc"); // program: "wc" (word count)
	  // myargs[1] = strdup("shell_gui.c"); // argument: file to count
	  // myargs[2] = NULL; // marks end of array
	  execvp(arg[0], arg);
	  die("execl");
	} else {
	  close(link[1]);
	  int nbytes = read(link[0], foo, sizeof(foo));
	  // printf("Output: (%.*s)\n", nbytes, foo);
	  char *result;
	  sprintf(result, "%.*s", nbytes, foo);
	  // printf("OUTPUT: %s\n",result);
	  gtk_text_buffer_append_output(GTK_TEXT_BUFFER(buffer), result, -1);
	  close(link[0]);
	  wait(NULL);
	}
	gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
	gtk_text_buffer_append_prompt(GTK_TEXT_BUFFER(buffer), -1);

	free(text2);
	return TRUE;
}

void gtk_text_buffer_append_output(GtkTextBuffer *buffer, char *text, int len)
{
	gtk_text_buffer_insert_at_cursor(buffer, text, len);
	GtkTextIter iter;
	gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
	min_cursor_offset = gtk_text_iter_get_offset(&iter);
}

void destroy(GtkWidget *window)
{
    gtk_main_quit();
}
 
gboolean delete_event(GtkWidget *window, GdkEvent *event)
{
    return FALSE;
}   