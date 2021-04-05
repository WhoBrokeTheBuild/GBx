#ifndef GBX_LOG_WINDOW_H
#define GBX_LOG_WINDOW_H

#include <GBx/GBx.h>
#include <GBx/Log.h>
#include <gtk/gtk.h>

#include <stdbool.h>

typedef struct _GBxLogWindow GBxLogWindow;

G_DECLARE_FINAL_TYPE(
    GBxLogWindow,
    gbx_log_window,
    GBX, LOG_WINDOW,
    GtkWindow
)

struct _GBxLogWindow
{
    GtkWindow GtkParent;

    GtkScrolledWindow * GtkScrolledWindow;

    GtkTextView * GtkTextView;

    GtkTextBuffer * GtkTextBuffer;
};

GtkWidget * gbx_log_window_new();

void gbx_log_window_destroy(GBxLogWindow * self);

gboolean gbx_log_window_delete(GBxLogWindow * self);

void gbx_log_window_add_entry(GBxLogWindow * self, clog_color_t color, const char * message);

#endif // GBX_LOG_WINDOW_H