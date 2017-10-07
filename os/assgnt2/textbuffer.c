#include <gtk/gtk.h>

typedef GtkWidget gw;

int main(int argc, char *argv[])
{

	GtkWidget *window;
	GtkWidget *view;
	gw *vbox;

	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	GtkTextIter iter;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	gtk_window_set_title(GTK_WINDOW(window), "GtkTextView");

	vbox = gtk_vbox_new(FALSE, 0);
	view = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(vbox), view, TRUE, TRUE, 0);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

	gtk
}