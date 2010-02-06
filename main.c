#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "pixmap.h"
#include "about.h"
#include "readrc.h"

GtkWidget *topwindow;
GtkWidget *messtext;
GtkWidget *systext;
GtkWidget *icmptext;
GtkWidget *progressbar;
GdkFont *font;
FILE *log1,*log2,*log3,*fp_meminfo;

gint update_textbox_messages (gpointer data)
{
	static char temp2[128],temp3[128];
       	fgets(temp3,128,log1);
	if(!strcmp(temp2,temp3))
		return 1;
	strcpy(temp2,temp3);
	gtk_text_insert (GTK_TEXT (messtext), font, NULL, NULL,temp2 , -1);
	
	return 1;
	
}
gint update_textbox_syslog (gpointer data)
{
	static char temp2[128],temp3[128];
       	fgets(temp3,128,log2);
	if(!strcmp(temp2,temp3))
		return 1;
	strcpy(temp2,temp3);

	gtk_text_insert (GTK_TEXT (systext), font, NULL, NULL,temp2 , -1);
	return 1;
	
}

gint update_textbox_icmplog (gpointer data)
{
	static char temp2[128],temp3[128];
       	fgets(temp3,128,log3);
	if(!strcmp(temp2,temp3))
		return 1;
	strcpy(temp2,temp3);
	gtk_text_insert (GTK_TEXT (icmptext), font, NULL, NULL,temp2 , -1);
	return 1;
	
}

gint update_meminfo(gpointer data) {

	char	temp[128];
	unsigned long total, used, free, shared, buffers, cached, swtotal, swused;
	int porcmem;//,porcswap;
	freopen("/proc/meminfo", "r", fp_meminfo);
	while (fgets(temp, 128, fp_meminfo)) {
		if (strstr(temp, "Mem:")) {
			sscanf(temp, "Mem: %ld %ld %ld %ld %ld %ld", &total, &used, &free, &shared, &buffers, &cached);
		}
		if (strstr(temp, "Swap:")) {
			sscanf(temp, "Swap: %ld %ld", &swtotal, &swused);
			break;
		}
	}
	porcmem =((swtotal - swused) /1000000) + ((total - used) /100000);
	gtk_progress_bar_update (GTK_PROGRESS_BAR (progressbar),(gfloat) porcmem/100);
	return 1;
}


int main (int argc, char *argv[])
{

	GtkWidget *mainbox;
	GtkWidget *pixmap;
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *firstbox;
	GtkWidget *secondbox;
	GtkWidget *hbox1;
	GtkWidget *vbox1;
	GtkWidget *separator;
	GtkWidget *textbox;
	GtkWidget *vsb;
	GtkWidget *table;
	GtkAdjustment *vadj;




	gtk_init (&argc, &argv);
	
	read_conf();
	topwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT (topwindow), "destroy",
	                   GTK_SIGNAL_FUNC (gtk_exit), NULL);
	gtk_container_border_width (GTK_CONTAINER (topwindow), 1);
	gtk_window_set_title (GTK_WINDOW (topwindow), "X Log Monitor V0.1b");
	gtk_widget_show (topwindow);	
	
	mainbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (topwindow), mainbox);
	gtk_widget_show (mainbox);	

	firstbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (mainbox), firstbox, FALSE, FALSE, 1);
	gtk_widget_show (firstbox);
	
	secondbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (firstbox), secondbox, FALSE, FALSE, 1);
	gtk_widget_show (secondbox);
	


	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (secondbox), hbox1, FALSE, FALSE, 1);
	gtk_widget_show (hbox1);
	
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (firstbox), vbox1, FALSE, FALSE, 1);
	gtk_widget_show (vbox1);	
	
	create_pixmaps();
	pixmap = gtk_pixmap_new (hackmain_xpm, hackmainmask_xpm);
	gtk_box_pack_start (GTK_BOX (vbox1), pixmap, FALSE, FALSE, 1);
	gtk_widget_show (pixmap);

        separator = gtk_hseparator_new ();
        gtk_box_pack_start (GTK_BOX (vbox1), separator, FALSE, FALSE, 1);
        gtk_widget_show (separator);

	label = gtk_label_new ("Memory");
	gtk_box_pack_start (GTK_BOX (hbox1), label, FALSE, FALSE, 1);
	gtk_widget_show (label);	
		
	progressbar = gtk_progress_bar_new();
	gtk_container_add (GTK_CONTAINER (hbox1), progressbar);
	gtk_progress_bar_update (GTK_PROGRESS_BAR (progressbar), (gfloat) 0.0);
	gtk_widget_show (progressbar);

	button = gtk_button_new_with_label ("Exit");
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
				GTK_SIGNAL_FUNC (gtk_exit), NULL);
	gtk_box_pack_start (GTK_BOX (vbox1), button, FALSE, FALSE, 1);
	gtk_widget_show (button);
	button = gtk_button_new_with_label ("About");
	gtk_signal_connect (GTK_OBJECT (button), "clicked",
				GTK_SIGNAL_FUNC (create_about), NULL);
	gtk_box_pack_start (GTK_BOX (vbox1), button, FALSE, FALSE, 1);
	gtk_widget_show (button); 

	font = gdk_font_load ("-*-helvetica-medium-r-normal-*-*-100-*-*-*-*-*-*");


	textbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (secondbox), textbox, FALSE, FALSE, 0);
	gtk_widget_show (textbox);
	vadj = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	vsb = gtk_vscrollbar_new (vadj);
	
	messtext = gtk_text_new (NULL, vadj);
	gtk_text_set_editable (GTK_TEXT (messtext), FALSE);
	gtk_widget_set_sensitive (messtext, FALSE);
	gtk_widget_set_usize (messtext, 350, 12);
	
	table = gtk_table_new (1, 2, FALSE);
	gtk_table_set_col_spacing (GTK_TABLE (table), 0, 2);
	gtk_box_pack_start (GTK_BOX (textbox), table, TRUE, TRUE, 0);

	gtk_table_attach (GTK_TABLE (table), vsb, 1, 2, 0, 1,
				0, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
	gtk_table_attach (GTK_TABLE (table), messtext, 0, 1, 0, 1,
				GTK_EXPAND | GTK_SHRINK | GTK_FILL,
				GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
	
	gtk_widget_realize (messtext);

	gtk_widget_show (vsb);
	gtk_widget_show (messtext);
	gtk_widget_show (table);
	
	textbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (secondbox), textbox, FALSE, FALSE, 0);
	gtk_widget_show (textbox);
	vadj = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	vsb = gtk_vscrollbar_new (vadj);
	
	systext = gtk_text_new (NULL, vadj);
	gtk_text_set_editable (GTK_TEXT (systext), FALSE);
	gtk_widget_set_sensitive (systext, FALSE);
	gtk_widget_set_usize (systext, 350,12);
	
	table = gtk_table_new (1, 2, FALSE);
	gtk_table_set_col_spacing (GTK_TABLE (table), 0, 2);
	gtk_box_pack_start (GTK_BOX (textbox), table, TRUE, TRUE, 0);

	gtk_table_attach (GTK_TABLE (table), vsb, 1, 2, 0, 1,
				0, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
	gtk_table_attach (GTK_TABLE (table), systext, 0, 1, 0, 1,
				GTK_EXPAND | GTK_SHRINK | GTK_FILL,
				GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
	
	gtk_widget_realize (systext);

	gtk_widget_show (vsb);
	gtk_widget_show (systext);
	gtk_widget_show (table);
	
	textbox = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (secondbox), textbox, FALSE, FALSE, 0);
	gtk_widget_show (textbox);
	vadj = GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	vsb = gtk_vscrollbar_new (vadj);
	
	icmptext = gtk_text_new (NULL, vadj);
	gtk_text_set_editable (GTK_TEXT (icmptext), FALSE);
	gtk_widget_set_sensitive (icmptext, FALSE);
	gtk_widget_set_usize (icmptext, 350, 12);
	
	table = gtk_table_new (1, 2, FALSE);
	gtk_table_set_col_spacing (GTK_TABLE (table), 0, 2);
	gtk_box_pack_start (GTK_BOX (textbox), table, TRUE, TRUE, 0);

	gtk_table_attach (GTK_TABLE (table), vsb, 1, 2, 0, 1,
				0, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
	gtk_table_attach (GTK_TABLE (table), icmptext, 0, 1, 0, 1,
				GTK_EXPAND | GTK_SHRINK | GTK_FILL,
				GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
	
	gtk_widget_realize (icmptext);

	gtk_widget_show (vsb);
	gtk_widget_show (icmptext);
	gtk_widget_show (table);
	

	if( access(xlogmonitor_log1,0) == -1) {
		gtk_text_insert (GTK_TEXT (messtext), font, NULL, NULL,"File Messages not found\n" , -1);
           	}
        if((log1 = fopen(xlogmonitor_log1,"r"))==NULL) {
	 	gtk_text_insert (GTK_TEXT (messtext), font, NULL, NULL,"Error open Messages\n" , -1);
	 }

	if( access(xlogmonitor_log2,0) == -1) {
           	gtk_text_insert (GTK_TEXT (systext), font, NULL, NULL,"File Syslog not found\n" , -1);
           	}
        if((log2 = fopen(xlogmonitor_log2,"r"))==NULL) {
		gtk_text_insert (GTK_TEXT (systext), font, NULL, NULL,"Error open Syslog\n" , -1);
	 }

	if( access(xlogmonitor_log3,0) == -1) {
		gtk_text_insert (GTK_TEXT (icmptext), font, NULL, NULL,"file Icmplog not found\n" , -1);
      	}

	fp_meminfo=fopen("/proc/meminfo", "r");
        if((log3 = fopen(xlogmonitor_log3,"r"))==NULL) {
	 	gtk_text_insert (GTK_TEXT (icmptext), font, NULL, NULL,"Error open Icmplog\n" , -1);
	 }
	if(fseek(log1,1,SEEK_END)!=0){
		gtk_text_insert (GTK_TEXT (messtext), font, NULL, NULL,"Error finding the end of file \n" , -1);
	}
	if(fseek(log2,1,SEEK_END)!=0){
		gtk_text_insert (GTK_TEXT (systext), font, NULL, NULL,"Error finding the end of file \n" , -1); 
	}
	if(fseek(log3,1,SEEK_END)!=0){
		gtk_text_insert (GTK_TEXT (icmptext), font, NULL, NULL,"Error finding the end of file \n" , -1);
	}

	gtk_timeout_add (10, update_textbox_syslog, NULL);
	gtk_timeout_add (100, update_meminfo, NULL);
	gtk_timeout_add (10, update_textbox_messages, NULL);
	gtk_timeout_add (10, update_textbox_icmplog, NULL);

	gtk_main();
	return 0;
}



