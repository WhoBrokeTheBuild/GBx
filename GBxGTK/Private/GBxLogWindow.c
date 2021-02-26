#include "GBxLogWindow.h"

#include "Resource.h"

G_DEFINE_TYPE(GBxLogWindow, gbx_log_window, GTK_TYPE_WINDOW)

void _gbx_log_window_callback(clog_color_t color, const char * message, void * userData)
{
    GBxLogWindow * self = GBX_LOG_WINDOW(userData);
    gbx_log_window_add_entry(self, color, message);
}

void gbx_log_window_init(GBxLogWindow * self)
{
    gtk_widget_init_template(GTK_WIDGET(self));

    clog_add_callback(_gbx_log_window_callback, self);
}

void gbx_log_window_class_init(GBxLogWindowClass * klass)
{
    GtkWidgetClass * wc = GTK_WIDGET_CLASS(klass);

    GError * error = NULL;
    GBytes * data = g_resource_lookup_data(gbx_get_resource(), "/GBx/GBxLogWindow.glade", 0, &error);
    if (!data) {
        fprintf(stderr, "failed to load /GBx/GBxLogWindow.glade: %s\n", error->message);
        g_error_free(error);
    }

    gtk_widget_class_set_template(wc, data);

    gtk_widget_class_bind_template_child(wc, GBxLogWindow, GtkScrolledWindow);
    gtk_widget_class_bind_template_child(wc, GBxLogWindow, GtkTextView);
    gtk_widget_class_bind_template_child(wc, GBxLogWindow, GtkTextBuffer);

    gtk_widget_class_bind_template_callback(wc, gbx_log_window_destroy);
    gtk_widget_class_bind_template_callback(wc, gbx_log_window_delete);
}

GtkWidget * gbx_log_window_new()
{
    GBxLogWindow * window;
    window = GBX_LOG_WINDOW(g_object_new(gbx_log_window_get_type(), NULL));
    return GTK_WIDGET(window);
}

void gbx_log_window_destroy(GBxLogWindow * self)
{
    clog_remove_callback(_gbx_log_window_callback, self);
}

gboolean gbx_log_window_delete(GBxLogWindow * self)
{
    gtk_widget_hide(GTK_WIDGET(self));
    return true;
}

void gbx_log_window_add_entry(GBxLogWindow * self, clog_color_t color, const char * message)
{
    GtkTextIter iter;

    const char * colorName = NULL;
    switch (color) {
    case CLOG_COLOR_BLACK:
        colorName = "black";
        break;
    case CLOG_COLOR_RED:
        colorName = "red";
        break;
    case CLOG_COLOR_GREEN:
        colorName = "green";
        break;
    case CLOG_COLOR_YELLOW:
        colorName = "orange";
        break;
    case CLOG_COLOR_BLUE:
        colorName = "blue";
        break;
    case CLOG_COLOR_MAGENTA:
        colorName = "magenta";
        break;
    case CLOG_COLOR_CYAN:
        colorName = "cyan";
        break;
    case CLOG_COLOR_WHITE:
        colorName = "white";
        break;
    default:
        break;
    }

    gtk_text_buffer_get_end_iter(self->GtkTextBuffer, &iter);

    if (colorName) {
        char buffer[CLOG_MAX_LOG_MESSAGE_LENGTH + 64]; // Leave room for <span></span>

        snprintf(buffer, sizeof(buffer), "<span color=\"%s\">%s</span>", colorName, message);
        gtk_text_buffer_insert_markup(self->GtkTextBuffer, &iter, buffer, -1);
    }
    else {
        gtk_text_buffer_insert_markup(self->GtkTextBuffer, &iter, message, -1);
    }

    GtkAdjustment * vadjust = gtk_scrolled_window_get_vadjustment(self->GtkScrolledWindow);
    gtk_adjustment_set_value(vadjust, gtk_adjustment_get_upper(vadjust));
}