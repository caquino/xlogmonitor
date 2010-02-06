#include <gtk/gtk.h>
#include "pixmap.h"
GtkWidget *create_about (GtkWidget *parent)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *pixmap;
	GtkWidget *separator;
	GtkWidget *button;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect_object (GTK_OBJECT (window), "destroy",
					GTK_SIGNAL_FUNC (gtk_widget_destroy),
					GTK_OBJECT (window));
	gtk_container_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_title (GTK_WINDOW (window), "About...");
	
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show (vbox);
	
	pixmap = gtk_pixmap_new (hackerzone_xpm, hackerzonemask_xpm);
	gtk_box_pack_start (GTK_BOX (vbox), pixmap, FALSE, FALSE, 0);
	gtk_widget_show (pixmap);
	
	separator = gtk_hseparator_new ();
	gtk_box_pack_start (GTK_BOX (vbox), separator, FALSE, FALSE, 10);
	gtk_widget_show (separator);
	
	label = gtk_label_new ("Copyright (c) 1998 ChaosMaker");
	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
	gtk_widget_show (label);

	label = gtk_label_new ("chaosmaker@webcom.com");
	gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
	gtk_widget_show (label);

	button = gtk_button_new_with_label ("OK");
	gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 5);
	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
					GTK_SIGNAL_FUNC (gtk_widget_destroy),
					GTK_OBJECT (window));
	gtk_widget_show (button);
	
	gtk_widget_show (window);
	return 0;
}
