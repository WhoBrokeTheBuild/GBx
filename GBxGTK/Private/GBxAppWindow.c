#include "GBxAppWindow.h"

#include <GBx/Cartridge.h>
#include <GBx/APU.h>
#include <GBx/PPU.h>
#include <GBx/Log.h>

#include "Resource.h"
#include <epoxy/gl.h>

#include <assert.h>

const char * VERTEX_SHADER =
    "#version 330 core                      \n"
    "in vec4 inPosition;                    \n"
    "in vec2 inUV;                          \n"
    "out vec2 vUV;                          \n"
    "void main() {                          \n"
    "  gl_Position = inPosition;            \n"
    "  vUV = inUV;                          \n"
    "}                                      \n";

const char * FRAGMENT_SHADER = 
    "#version 330 core                      \n"
    "in vec2 vUV;                           \n"
    "uniform sampler2D uTexture;            \n"
    "out vec4 oColor;                       \n"
    "void main() {                          \n"
    "  oColor = texture2D(uTexture, vUV);   \n"
    "}                                      \n";

const float VERTICES[] = {
    -1.0f,  1.0f,  0.0f,  1.0f,     0.0f, 0.0f,
     1.0f,  1.0f,  0.0f,  1.0f,     1.0f, 0.0f,
     1.0f, -1.0f,  0.0f,  1.0f,     1.0f, 1.0f,
     1.0f, -1.0f,  0.0f,  1.0f,     1.0f, 1.0f,
    -1.0f, -1.0f,  0.0f,  1.0f,     0.0f, 1.0f,
    -1.0f,  1.0f,  0.0f,  1.0f,     0.0f, 0.0f,
};

#define VERTEX_COUNT 6

G_DEFINE_TYPE(GBxAppWindow, gbx_app_window, GTK_TYPE_APPLICATION_WINDOW)

void gbx_app_window_init(GBxAppWindow * self)
{
    gtk_widget_init_template(GTK_WIDGET(self));

    self->IsOpen = true;
    self->IsRunning = true;
    self->IsFullscreen = false;

    self->LogWindow = GBX_LOG_WINDOW(gbx_log_window_new(self->GBx));
}

void gbx_app_window_class_init(GBxAppWindowClass * klass)
{
    GtkWidgetClass * wc = GTK_WIDGET_CLASS(klass);

    GError * error = NULL;
    GBytes * data = g_resource_lookup_data(gbx_get_resource(), "/GBx/GBxAppWindow.glade", 0, &error);
    if (!data) {
        fprintf(stderr, "failed to load /GBx/GBxAppWindow.glade: %s\n", error->message);
        g_error_free(error);
    }

    gtk_widget_class_set_template(wc, data);

    gtk_widget_class_bind_template_child(wc, GBxAppWindow, GtkGLArea);

    gtk_widget_class_bind_template_callback(wc, gbx_app_window_destroy);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_gl_init);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_gl_term);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_gl_configure);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_gl_render);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_reset);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_open_rom);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_play_pause);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_show_debug_window);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_show_log_window);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_show_about);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_on_key_release);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_toggle_fullscreen);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_change_volume);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_zoom_1);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_zoom_2);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_zoom_3);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_zoom_4);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_zoom_5);
    gtk_widget_class_bind_template_callback(wc, gbx_app_window_zoom_6);
}

GtkWidget * gbx_app_window_new(GBx * ctx)
{
    GBxAppWindow * window;
    window = GBX_APP_WINDOW(g_object_new(gbx_app_window_get_type(), NULL));
    window->GBx = ctx;
    
    return GTK_WIDGET(window);
}

void gbx_app_window_run(GBxAppWindow * self)
{
    gtk_widget_show_all(GTK_WIDGET(self));

    while (self->IsOpen) {
        while (g_main_context_iteration(NULL, false)) { }

        if (self->IsRunning) {
            // if (self->debugger) {
            //     nesx_debugger_frame(self->debugger);
            // }
            // else {
                GBx_Frame(self->GBx);
            // }
        }

        if (self->GtkGLArea) {
            gtk_gl_area_queue_render(self->GtkGLArea);
        }
    }
}

void gbx_app_window_destroy(GBxAppWindow * self)
{
    self->IsOpen = false;
    self->IsRunning = false;

    gtk_widget_destroy(GTK_WIDGET(self->LogWindow));
    self->LogWindow = NULL;
}

void gbx_app_window_gl_init(GBxAppWindow * self)
{
    gtk_gl_area_make_current(self->GtkGLArea);

    GError * error = gtk_gl_area_get_error(self->GtkGLArea);
    if (error != NULL) {
        fprintf(stderr, "failed to create GtkGLArea: %s\n", error->message);
        return;
    }

    GBxLogInfo("OpenGL Version: %s", glGetString(GL_VERSION));

    GBxLogInfo("OpenGL Renderer: %s", glGetString(GL_RENDERER));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glGenTextures(1, &self->GLTexture);
    glBindTexture(GL_TEXTURE_2D, self->GLTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Sanity check for using GL_RGB
    static_assert(
        GBX_SCREEN_COMPONENTS == 3,
        "GBX_SCREEN_COMPONENTS != 3"
    );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GBX_SCREEN_WIDTH, GBX_SCREEN_HEIGHT,
        0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &VERTEX_SHADER, NULL);
    glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    self->GLShader = glCreateProgram();

    glAttachShader(self->GLShader, vertexShader);
    glAttachShader(self->GLShader, fragmentShader);

    glLinkProgram(self->GLShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(self->GLShader);

    glGenVertexArrays(1, &self->GLVertexArray);
    glBindVertexArray(self->GLVertexArray);

    glGenBuffers(1, &self->GLVertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, self->GLVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    // vec4 inPosition
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 
        sizeof(float) * 6, NULL);

    // vec2 inUV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
        sizeof(float) * 6, (void *)(sizeof(float) * 4));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    // TODO: Error checking
}

void gbx_app_window_gl_term(GBxAppWindow * self)
{
    gtk_gl_area_make_current(self->GtkGLArea);

    GError * error = gtk_gl_area_get_error(self->GtkGLArea);
    if (error != NULL) {
        fprintf(stderr, "failed to create GtkGLArea: %s\n", error->message);
        return;
    }

    glUseProgram(0);
    glDeleteProgram(self->GLShader);
    self->GLShader = 0;

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &self->GLTexture);
    self->GLTexture = 0;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &self->GLVertexArray);
    self->GLVertexArray = 0;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &self->GLVertexBuffer);
    self->GLVertexBuffer = 0;
}

void gbx_app_window_gl_configure(GBxAppWindow * self, GdkEventConfigure * event)
{
    glViewport(0, 0, event->width, event->height);
}

void gbx_app_window_gl_render(GBxAppWindow * self)
{
    glClear(GL_COLOR_BUFFER_BIT);

    GBx_PPU * ppu = GBx_GetPPU(self->GBx);

    glBindTexture(GL_TEXTURE_2D, self->GLTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
        GBX_SCREEN_WIDTH, GBX_SCREEN_HEIGHT,
        GL_RGB, GL_UNSIGNED_BYTE, ppu->Pixels);

    glUseProgram(self->GLShader);
    glBindVertexArray(self->GLVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
}

void gbx_app_window_reset(GBxAppWindow * self)
{

}

void gbx_app_window_open_rom(GBxAppWindow * self)
{
    GtkWidget * dialog = gtk_file_chooser_dialog_new(
        "Open ROM",
        GTK_WINDOW(self),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Open", GTK_RESPONSE_ACCEPT,
        NULL
    );

    // TODO: gtk_file_chooser_add_shortcut_folder

    GtkFileFilter * filterROMs = gtk_file_filter_new();
    gtk_file_filter_set_name(filterROMs, "GB/SGB/GBC ROMs (*.gb, *.gbc)");
    gtk_file_filter_add_pattern(filterROMs, "*.gb");
    gtk_file_filter_add_pattern(filterROMs, "*.gbc");
    gtk_file_filter_add_mime_type(filterROMs, "application/x-gameboy-rom");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filterROMs);

    GtkFileFilter * filterAll = gtk_file_filter_new();
    gtk_file_filter_set_name(filterAll, "All Files");
    gtk_file_filter_add_pattern(filterAll, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filterAll);

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        const char * filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (!GBx_Cartridge_Load(self->GBx, filename)) {
            gtk_widget_hide(dialog);

            GtkWidget * errorDialog = gtk_message_dialog_new(
                GTK_WINDOW(self),
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_CLOSE,
                "Unable to open ROM %s", filename
            );

            gtk_dialog_run(GTK_DIALOG(errorDialog));
            gtk_widget_destroy(errorDialog);
        }
    }

    gtk_widget_destroy(dialog);
}

void gbx_app_window_play_pause(GBxAppWindow * self)
{

}

void gbx_app_window_show_debug_window(GBxAppWindow * self)
{

}

void gbx_app_window_show_log_window(GBxAppWindow * self)
{
    gint x, y, w, h;
    gtk_window_get_position(GTK_WINDOW(self), &x, &y);
    gtk_window_get_size(GTK_WINDOW(self), &w, &h);

    gtk_window_move(GTK_WINDOW(self->LogWindow), x + w, y);

    gtk_widget_show_all(GTK_WIDGET(self->LogWindow));
    // gtk_window_present(GTK_WINDOW(self->LogWindow));
}

void gbx_app_window_show_about(GBxAppWindow * self)
{
    static const char * authors[] = {
        "Stephen L.W.",
        "Dillon B.",
        "Benjamin C.",
        NULL,
    };

    static const char * website = "https://github.com/WhoBrokeTheBuild/GBx";

    gtk_show_about_dialog(
        GTK_WINDOW(self),
        "name", "GBx",
        "version", GBX_VERSION_STRING,
        "authors", authors,
        "website", website,
        "website-label", website, 
        "license-type", GTK_LICENSE_MIT_X11,
        "logo-icon-name", "application-x-executable",
        NULL);
}

void gbx_app_window_on_key_release(GBxAppWindow * self, GdkEventKey * event)
{
    // Escape, Exit Fullscreen
    if (self->IsFullscreen && event->keyval == GDK_KEY_Escape) {
        gbx_app_window_toggle_fullscreen(self);
    }
}

void gbx_app_window_toggle_fullscreen(GBxAppWindow * self)
{
    if (self->IsFullscreen) {
        gtk_window_unfullscreen(GTK_WINDOW(self));
        self->IsFullscreen = false;
    }
    else {
        gtk_window_fullscreen(GTK_WINDOW(self));
        self->IsFullscreen = true;
    }
}

void gbx_app_window_change_volume(GBxAppWindow * self, double value)
{
    // TODO: Move
    // GBxAPU_t * apu = GBx_APU(ctx);
    // gtk_scale_button_set_value(GTK_SCALE_BUTTON(self->GtkVolume), apu->Volume);

    GBx_APU * apu = GBx_GetAPU(self->GBx);
    apu->Volume = value;
}

void gbx_app_window_set_scale(GBxAppWindow * self, int scale)
{
    gtk_widget_set_size_request(
        GTK_WIDGET(self->GtkGLArea),
        GBX_SCREEN_WIDTH * scale,
        GBX_SCREEN_HEIGHT * scale);

    // Scale back down to requested size
    gtk_window_resize(GTK_WINDOW(self), 1, 1);
}

void gbx_app_window_zoom_1(GBxAppWindow * self)
{
    gbx_app_window_set_scale(self, 1);
}

void gbx_app_window_zoom_2(GBxAppWindow * self)
{
    gbx_app_window_set_scale(self, 2);
}

void gbx_app_window_zoom_3(GBxAppWindow * self)
{
    gbx_app_window_set_scale(self, 3);
}

void gbx_app_window_zoom_4(GBxAppWindow * self)
{
    gbx_app_window_set_scale(self, 4);
}

void gbx_app_window_zoom_5(GBxAppWindow * self)
{
    gbx_app_window_set_scale(self, 5);
}

void gbx_app_window_zoom_6(GBxAppWindow * self)
{
    gbx_app_window_set_scale(self, 6);
}