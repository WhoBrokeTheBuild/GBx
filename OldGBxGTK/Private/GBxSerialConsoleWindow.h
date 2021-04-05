#ifndef GBX_SERIAL_CONSOLE_WINDOW_H
#define GBX_SERIAL_CONSOLE_WINDOW_H

#include <GBx/GBx.h>
#include <gtk/gtk.h>

#include <stdbool.h>

typedef struct _GBxSerialConsoleWindow GBxSerialConsoleWindow;

G_DECLARE_FINAL_TYPE(
    GBxSerialConsoleWindow,
    gbx_serial_console_window,
    GBX, SERIAL_CONSOLE_WINDOW,
    GtkWindow
)

struct _GBxSerialConsoleWindow
{
    GtkWindow GtkParent;

    GBx * GBx;

    GtkCheckButton * GtkCheckButtonAutoScroll;

    GtkScrolledWindow * GtkScrolledWindowText;

    GtkTextView * GtkTextViewText;

    GtkTextBuffer * GtkTextBufferText;

    GtkScrolledWindow * GtkScrolledWindowHex;

    GtkTextView * GtkTextViewHex;
    
    GtkTextBuffer * GtkTextBufferHex;
};

GtkWidget * gbx_serial_console_window_new(GBx * ctx);

void gbx_serial_console_window_destroy(GBxSerialConsoleWindow * self);

gboolean gbx_serial_console_window_delete(GBxSerialConsoleWindow * self);

void gbx_serial_window_clear(GBxSerialConsoleWindow * self);

void gbx_serial_console_window_add_entry(GBxSerialConsoleWindow * self, uint8_t data);

#endif // GBX_SERIAL_CONSOLE_WINDOW_H