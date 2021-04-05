#include "GBxSerialConsoleWindow.h"

#include <GBx/Serial.h>

#include "Resource.h"

G_DEFINE_TYPE(GBxSerialConsoleWindow, gbx_serial_console_window, GTK_TYPE_WINDOW)

void gbx_serial_console_window_init(GBxSerialConsoleWindow * self)
{
    gtk_widget_init_template(GTK_WIDGET(self));
}

void gbx_serial_console_window_class_init(GBxSerialConsoleWindowClass * klass)
{
    GtkWidgetClass * wc = GTK_WIDGET_CLASS(klass);

    GError * error = NULL;
    GBytes * data = g_resource_lookup_data(gbx_get_resource(), "/GBx/GBxSerialConsoleWindow.glade", 0, &error);
    if (!data) {
        fprintf(stderr, "failed to load /GBx/GBxSerialConsoleWindow.glade: %s\n", error->message);
        g_error_free(error);
    }

    gtk_widget_class_set_template(wc, data);

    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkCheckButtonAutoScroll);
    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkScrolledWindowText);
    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkTextViewText);
    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkTextBufferText);
    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkScrolledWindowHex);
    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkTextViewHex);
    gtk_widget_class_bind_template_child(wc, GBxSerialConsoleWindow, GtkTextBufferHex);

    gtk_widget_class_bind_template_callback(wc, gbx_serial_console_window_destroy);
    gtk_widget_class_bind_template_callback(wc, gbx_serial_console_window_delete);
    gtk_widget_class_bind_template_callback(wc, gbx_serial_window_clear);
}

GtkWidget * gbx_serial_console_window_new(GBx * ctx)
{
    GBxSerialConsoleWindow * window;
    window = GBX_SERIAL_CONSOLE_WINDOW(g_object_new(gbx_serial_console_window_get_type(), NULL));
    window->GBx = ctx;

    GBx_Serial_SetCallbacks(ctx, (GBx_SerialReadCallback)gbx_serial_console_window_add_entry, NULL, window);

    return GTK_WIDGET(window);
}

void gbx_serial_console_window_destroy(GBxSerialConsoleWindow * self)
{
    GBx_Serial_SetCallbacks(self->GBx, NULL, NULL, NULL);
}

gboolean gbx_serial_console_window_delete(GBxSerialConsoleWindow * self)
{
    gtk_widget_hide(GTK_WIDGET(self));
    return true;
}

void gbx_serial_window_clear(GBxSerialConsoleWindow * self)
{

}

void gbx_serial_console_window_add_entry(GBxSerialConsoleWindow * self, uint8_t data)
{
    GtkTextIter iter;
    GtkAdjustment * vadjust;
    char buffer[4];

    bool autoScroll = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self->GtkCheckButtonAutoScroll));

    snprintf(buffer, sizeof(buffer), "%02X ", data);

    gtk_text_buffer_get_end_iter(self->GtkTextBufferHex, &iter);
    gtk_text_buffer_insert(self->GtkTextBufferHex, &iter, buffer, 3);

    if (autoScroll) {
        vadjust = gtk_scrolled_window_get_vadjustment(self->GtkScrolledWindowHex);
        gtk_adjustment_set_value(vadjust, gtk_adjustment_get_upper(vadjust));
    }

    buffer[0] = data;
    gtk_text_buffer_get_end_iter(self->GtkTextBufferText, &iter);
    gtk_text_buffer_insert(self->GtkTextBufferText, &iter, buffer, 1);

    if (autoScroll) {
        vadjust = gtk_scrolled_window_get_vadjustment(self->GtkScrolledWindowText);
        gtk_adjustment_set_value(vadjust, gtk_adjustment_get_upper(vadjust));
    }
}