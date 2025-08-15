#include <stdio.h>
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 202311L
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include <wayland-util.h>
#include <wayland-server-core.h>

#include <wlr/backend.h>
#include <wlr/util/log.h>
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_scene.h>

#include "tvc_server.h"
#include "event_callbacks.h"
#include "utils.h"


struct tvc_server server = {0};

void tvc_terminate(int exit_code){
    wl_display_terminate(server.display);
}

static bool parseCliOptions(int argc, char **argv, CliOptions *options){
    constexpr char USAGE_TEMPLATE[] = "Usage: %s [-c config path] [-s startup command]\n";
    constexpr char SHORTOPS[] = "c:s:h";
    int ch = getopt(argc, argv, SHORTOPS);
    while (ch != -1) {
        switch (ch) {
            case 's':
                options->startupCmd = optarg;
                break;
            case 'c':
                options->configPath = optarg;
                break;
            default:
                printf(USAGE_TEMPLATE, argv[0]);
                return false;
        }
        ch = getopt(argc, argv, SHORTOPS);
    }
    if (optind < argc) {
        printf(USAGE_TEMPLATE, argv[0]);
        return false;
    }
    return true;
}

int main(int argc, char *argv[]){
    wlr_log_init(WLR_DEBUG, nullptr);
    
    CliOptions options;

    bool success = parseCliOptions(argc, argv, &options);
    if (!success) {
        return EXIT_FAILURE;
    }

    
    server.display = wl_display_create();
    server.backend = wlr_backend_autocreate(wl_display_get_event_loop(server.display), nullptr);

    if (server.backend == nullptr) {
        wlr_log(WLR_ERROR, "[wlroots] Failed to create wlr_backend");
        return EXIT_FAILURE;
    }
    
    server.renderer = wlr_renderer_autocreate(server.backend);
    bool err = wlr_renderer_init_wl_display(server.renderer, server.display);
    if (!err) {
        wlr_log(WLR_ERROR, "[wlroots] Failed to init wlr_renderer with wl_display");
        return EXIT_SUCCESS;
    }
    
    server.allocator = wlr_allocator_autocreate(server.backend, server.renderer);

    if (server.allocator == nullptr) {
        wlr_log(WLR_ERROR, "[wlroots] Failed to create wlr_allocator");
        return EXIT_FAILURE;
    }

    server.compositor = wlr_compositor_create(server.display, 5, server.renderer);
    server.subcompositor = wlr_subcompositor_create(server.display);
    server.dataDeviceManager = wlr_data_device_manager_create(server.display);
    
    server.outputLayout = wlr_output_layout_create(server.display);
    
    
    
	wl_list_init(&server.outputs);
	server.newOutput.notify = server_new_output;

        

    return EXIT_SUCCESS;
}
