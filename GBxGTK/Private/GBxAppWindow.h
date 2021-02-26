#ifndef GBX_APP_WINDOW_H
#define GBX_APP_WINDOW_H

#include <GBx/GBx.h>
#include <gtk/gtk.h>

#include "GBxLogWindow.h"

#include <stdbool.h>

typedef struct _GBxAppWindow GBxAppWindow;

G_DECLARE_FINAL_TYPE(
    GBxAppWindow,
    gbx_app_window,
    GBX, APP_WINDOW,
    GtkApplicationWindow
)

struct _GBxAppWindow
{
    GtkApplicationWindow GtkParent;

    bool IsOpen;
    bool IsRunning;
    bool IsFullscreen;

    GBx * GBx;

    GBxLogWindow * LogWindow;

    GtkVolumeButton * GtkVolume;
    GtkGLArea * GtkGLArea;

    unsigned GLShader;
    unsigned GLTexture;
    unsigned GLVertexBuffer;
    unsigned GLVertexArray;
};

GtkWidget * gbx_app_window_new(GBx * ctx);

void gbx_app_window_run(GBxAppWindow * self);

void gbx_app_window_destroy(GBxAppWindow * self);

void gbx_app_window_gl_init(GBxAppWindow * self);
void gbx_app_window_gl_term(GBxAppWindow * self);
void gbx_app_window_gl_configure(GBxAppWindow * self, GdkEventConfigure * event);
void gbx_app_window_gl_render(GBxAppWindow * self);

void gbx_app_window_reset(GBxAppWindow * self);

void gbx_app_window_open_rom(GBxAppWindow * self);

void gbx_app_window_play_pause(GBxAppWindow * self);

void gbx_app_window_show_debug_window(GBxAppWindow * self);
void gbx_app_window_show_log_window(GBxAppWindow * self);
void gbx_app_window_show_about(GBxAppWindow * self);

void gbx_app_window_on_key_release(GBxAppWindow * self, GdkEventKey * event);

void gbx_app_window_toggle_fullscreen(GBxAppWindow * self);

void gbx_app_window_change_volume(GBxAppWindow * self, double value);

void gbx_app_window_set_scale(GBxAppWindow * self, int scale);

void gbx_app_window_zoom_1(GBxAppWindow * self);
void gbx_app_window_zoom_2(GBxAppWindow * self);
void gbx_app_window_zoom_3(GBxAppWindow * self);
void gbx_app_window_zoom_4(GBxAppWindow * self);
void gbx_app_window_zoom_5(GBxAppWindow * self);
void gbx_app_window_zoom_6(GBxAppWindow * self);

#endif // GBX_APP_WINDOW_H