#include <Cube_GTK.h>
/* Surface to store current scribbles */
static cairo_surface_t* surface = NULL;

static void clear_surface(void)
{
    cairo_t* cr;
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_destroy(cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean configure_event_cb(GtkWidget* widget, GdkEventConfigure* event, gpointer data)
{
    if( surface ) cairo_surface_destroy( surface );
    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget), CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));
    /* Initialize the surface to white */
    clear_surface();
    /* We've handled the configure event, no need for further processing. */
    return TRUE;
}

/* Redraw the screen from the surface. Note that the ::draw
 * signal receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static gboolean draw_cb(GtkWidget* widget, cairo_t* cr, gpointer data)
{
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    return FALSE;
}

/* Draw a rectangle on the surface at the given position */
void draw_brush(GtkWidget* widget, gdouble x, gdouble y)
{
    cairo_t* cr;
    /* Paint to the surface, where we store our state */
    cr = cairo_create(surface);
    cairo_rectangle(cr, x - 0.5, y - 0.5, 1, 1);
    cairo_fill(cr);
    cairo_destroy(cr);

    /* Now invalidate the affected region of the drawing area. */
    gtk_widget_queue_draw_area(widget, x - 0.5, y - 0.5, 1, 1);
}

/* Handle button press events by either drawing a rectangle
 * or clearing the surface, depending on which button was pressed.
 * The ::button-press signal handler receives a GdkEventButton
 * struct which contains this information.
 */

static gboolean button_press_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
  /* paranoia check, in case we haven't gotten a configure event */
    if ( NULL == surface ) return FALSE;
    if ( GDK_BUTTON_PRIMARY == event->button )
    {
        type_point point;
        point.y = event->x-DEFAULT_Y_OFFSET;
        point.z = event->y-DEFAULT_Z_OFFSET;
        point.x = 0 - DEFAULT_X_OFFSET;
        create_scene(point);
        create_primitive(CUBE, point);
        alt_angle_scene(15, AXIS_X);
        alt_angle_scene(30, AXIS_Y);
        mod_current_scene(1);
    }
    if ( GDK_BUTTON_SECONDARY == event->button )
    {
        mod_current_scene(-1);
        type_point point;
        point.y = event->x-DEFAULT_Y_OFFSET;
        point.z = event->y-DEFAULT_Z_OFFSET;
        point.x = 0-DEFAULT_X_OFFSET;
        create_primitive(CUBE, point);
        alt_angle_scene(15, AXIS_X);
        alt_angle_scene(30, AXIS_Y);
        mod_current_scene(1);
    }

    /* We've handled the event, stop processing */
    return TRUE;
}

/* Handle motion events by continuing to draw if button 1 is
 * still held down. The ::motion-notify signal handler receives
 * a GdkEventMotion struct which contains this information.
 */
static gboolean motion_notify_event_cb(GtkWidget* widget, GdkEventMotion* event, gpointer data)
{
    /* paranoia check, in case we haven't gotten a configure event */
    if ( surface == NULL ) return FALSE;
    /* We've handled it, stop processing */
    return TRUE;
}

static gboolean fake_event_cb(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
    /* paranoia check, in case we haven't gotten a configure event */
    if ( surface == NULL ) return FALSE;

    clear_surface();
    alt_all_scenes(2, AXIS_Z);
    draw_all(widget);
    gtk_widget_queue_draw(widget);
    usleep(10000);

    return TRUE;
}

static void
close_window (void)
{
    free_all();
    if(surface) cairo_surface_destroy (surface);
}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget* window;
    GtkWidget* frame;
    GtkWidget* drawing_area;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Drawing Area");

    g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);

    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(window), frame);

    drawing_area = gtk_drawing_area_new();
    /* set a minimum size */
    gtk_widget_set_size_request(drawing_area, DEFAULT_Y, DEFAULT_Z);

    gtk_container_add(GTK_CONTAINER(frame), drawing_area);

    /* Signals used to handle the backing surface */
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_cb), NULL);
    g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configure_event_cb), NULL);

    /* Event signals */
    g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(motion_notify_event_cb), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(button_press_event_cb), NULL);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(fake_event_cb), NULL);

    /* Ask to receive events the drawing area doesn't normally
    * subscribe to. In particular, we need to ask for the
    * button press and motion notify events that want to handle.
    */

    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

    gtk_widget_show_all(window);
}

int main(int argc, char** argv)
{
  GtkApplication* app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
