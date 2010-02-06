#include <gtk/gtk.h>
#include "main.h"
#include "pixmap.h"
#include "hackmain.xpm"
#include "hackerzone.xpm"

GdkPixmap *hackmain_xpm;
GdkBitmap *hackmainmask_xpm;
GdkPixmap *hackerzone_xpm;
GdkBitmap *hackerzonemask_xpm;

int get_pixmap (char **data, GdkPixmap **pix, GdkPixmap **mas)
{
	*pix = gdk_pixmap_create_from_xpm_d (topwindow->window, mas, NULL, data);
	return (*pix!=NULL);
}

void create_pixmaps()
{
	get_pixmap (XPM_HACKMAIN,&hackmain_xpm,&hackmainmask_xpm);	
	get_pixmap (XPM_HACKERZONE,&hackerzone_xpm,&hackerzonemask_xpm);
}
