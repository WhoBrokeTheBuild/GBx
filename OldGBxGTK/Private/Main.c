#include <GBx/GBx.h>
#include <GBx/Log.h>
#include <GBx/Cartridge.h>
#include <GBx/Bootstrap.h>

#include <cflags.h>
#include <portaudio.h>

#include <stdio.h>

#include "GBxAppWindow.h"
#include "Resource.h"

#include <math.h>

float _table[200];
unsigned long _index;

static int audio_callback(
    const void * inputBuffer,
    void * outputBuffer, 
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags flags,
    void * userData
)
{
    GBx * ctx = (GBx *)userData;

    float * output = (float *)outputBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; ++i) {
        *output++ = _table[_index]; // Left
        _index = (_index + 1) % 200;
        *output++ = _table[_index]; // Right
        _index = (_index + 1) % 200;
    }

    return paContinue;
}

void add_log_file(const char * filename)
{
    clog_add_file(filename, false);
}

int main(int argc, char ** argv)
{
    cflags_t * flags = NULL;
    
    GError * error = NULL;
    GtkApplication * app = NULL;
    GBxAppWindow * window = NULL;

    PaStreamParameters paParam;
    PaStream * paStream;
    PaError paErr;

    GBx * gbx = NULL;
    
    flags = cflags_init();

    bool help = false;
    cflags_add_bool(flags, 'h', "help", &help,
        "Display this help and exit");

    int scale = 2;
    cflags_add_int(flags, 's', "scale", &scale,
        "Set the initial window scale, default is 2x");

    bool fullscreen = false;
    cflags_add_bool(flags, 'f', "fullscreen", &fullscreen,
        "Start fullscreen");

    const char * bootstrap = NULL;
    cflags_add_string(flags, 'b', "bootstrap", &bootstrap,
        "Load a custom Bootstrap/BIOS ROM");

    bool checksum = false;
    cflags_add_bool(flags, 'c', "checksum", &checksum,
        "Compute and compare the checksum from the Catridge Header");

    bool verbose = false;
    cflags_add_bool(flags, 'v', "verbose", &verbose,
        "Enables verbose output");

    cflags_add_string_callback(flags, '\0', "log-file", add_log_file,
        "Log output to file");

    cflags_parse(flags, argc, argv);

    if (help) {
        cflags_print_usage(flags, "[OPTION]... [ROM_FILENAME]",
            "A Toy GameBoy Emulator",
            "Additional information about this program can be found at:\n"
            "  http://github.com/WhoBrokeTheBuild/GBx.git");
        
        cflags_free(flags);
        return 0;
    }

    GBx_Config gbxConfig = {
    };

    if (!GBx_Init(&gbx, &gbxConfig)) {
        fprintf(stderr, "Failed to initialize GBx\n");
        goto cleanup;
    }

    app = gtk_application_new("com.stephenlw.gbx", G_APPLICATION_FLAGS_NONE);
    if (!app) {
        fprintf(stderr, "Failed to create GTK Application\n");
        goto cleanup;
    }

    if (!g_application_register(G_APPLICATION(app), NULL, &error)) {
        fprintf(stderr, "Failed to register GTK Application\n");
        goto cleanup;
    }

    window = GBX_APP_WINDOW(gbx_app_window_new(gbx));

    gbx_app_window_set_scale(window, scale);

    if (fullscreen) {
        gbx_app_window_toggle_fullscreen(window);
    }

    if (bootstrap) {
        if (!GBx_Bootstrap_Load(gbx, bootstrap)) {
            fprintf(stderr, "Failed to load Bootstrap/BIOS ROM '%s'\n", bootstrap);
            goto cleanup;
        }
    }

    if (flags->argc > 1) {
        if (!GBx_Cartridge_Load(gbx, flags->argv[1])) {
            fprintf(stderr, "Failed to load Cartridge ROM '%s'\n", flags->argv[1]);
            goto cleanup;
        }

        window->IsRunning = true;
    }

    GBx_Reset(gbx);

    
    // for (unsigned long i = 0; i < 200; ++i) {
    //     _table[i] = sinf(((float)i / 200.0f) * M_PI * 2.0f);
    //     _table[i] *= 0.1f;
    // }
    // _index = 0;

    // paErr = Pa_Initialize();
    // if (paErr != paNoError) {
    //     goto cleanup;
    // }

    // for (PaDeviceIndex i = 0, end = Pa_GetDeviceCount(); i != end; ++i) {
    //     PaDeviceInfo const * info = Pa_GetDeviceInfo(i);
    //     if (!info) {
    //         continue;
    //     }

    //     if (strcmp(info->name, "pulse") == 0) {
    //         paParam.device = i;
    //     }
    // }

    // if (paParam.device == paNoDevice) {
    //     fprintf(stderr, "Failed to find audio output device\n");
    //     goto cleanup;
    // }

    // paParam.channelCount = 2;
    // paParam.sampleFormat = paFloat32;
    // paParam.suggestedLatency = Pa_GetDeviceInfo(paParam.device)->defaultLowOutputLatency;
    // paParam.hostApiSpecificStreamInfo = NULL;

    // paErr = Pa_OpenStream(
    //     &paStream,
    //     NULL, // input
    //     &paParam,
    //     44100,
    //     200,
    //     paClipOff,
    //     audio_callback,
    //     gbx
    // );
    // if (paErr != paNoError) {
    //     goto cleanup;
    // }

    // Pa_StartStream(paStream);



    gbx_app_window_run(window);

cleanup:

    if (paErr != paNoError) {
        fprintf(stderr, "PortAudio Error: %s\n", Pa_GetErrorText(paErr));
    }

    Pa_Terminate();

    if (error) {
        g_error_free(error);
    }

    if (app) {
        g_object_unref(app);
    }

    GBx_Term(&gbx);

    cflags_free(flags);

    return 0;
}