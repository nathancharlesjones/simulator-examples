#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

gboolean windowDelete(__attribute__((unused)) GtkWidget *widget, 
                      __attribute__((unused)) GdkEvent  *event,
                      __attribute__((unused)) gpointer   data)
{
    //g_print("%s called.\n",__FUNCTION__);
    return FALSE;    // Returning TRUE stops the window being deleted.
                    // Returning FALSE allows deletion.   
}

int main(int argc, char **argv)
{
    GtkWidget *window;
    GtkBuilder *builder;
    GError *error = NULL;

    gtk_init(&argc,&argv);

    builder = gtk_builder_new ();
    if( gtk_builder_add_from_file (builder,"gtk_layout.glade" ,&error) == 0)
    {
	printf("gtk_builder_add_from_file FAILED %s\n",error->message);
	return EXIT_FAILURE;
    }
    
    window = GTK_WIDGET(gtk_builder_get_object(builder,"mainWindow"));
    gtk_builder_connect_signals(builder,NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return EXIT_SUCCESS;
}
