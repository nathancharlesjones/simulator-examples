#include <time.h>
#include <gtk/gtk.h>

// Returns elapsed system time in milliseconds.
//
uint32_t getMillis(void)
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (uint32_t)((time.tv_sec*1000) + (time.tv_nsec/1000000));
}

// Read the current x/y/z values (in units of g) from the attached accelerometer.
// Values should not include acceleration due to gravity.
//
void readAccel_gs(double* x, double* y, double* z)
{
    printf("%s", gtk_editable_get_chars());
}

// Set the motor speed from 0 to MAX using double value from 0 to 1.0.
//
void setMotorSpeed(double speed)
{
    //asdf
}

// Set the LED brightness using double value from 0 to 1.0.
//
void setLED(double brightness)
{
    //adsf
}

// Display a NUL-terminated string to the user
//
void display(const char * /* msg */)
{
    //asdf
}

gboolean windowDelete(__attribute__((unused)) GtkWidget *widget, 
                      __attribute__((unused)) GdkEvent  *event,
                      __attribute__((unused)) gpointer   data)
{
    //g_print("%s called.\n",__FUNCTION__);
    gtk_main_quit();
    return FALSE;    // Returning TRUE stops the window being deleted.
                    // Returning FALSE allows deletion.   
}

gboolean application(gpointer user_data)
{
    runTheApplication();
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
    g_idle_add(application, NULL);
    
    gtk_widget_show_all(window);
    gtk_main();

    return EXIT_SUCCESS;
}
