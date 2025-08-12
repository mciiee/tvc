#include "wayland-util.h"
#include <stdlib.h>

#include <wayland-server-core.h>

#include <wlr/backend.h>
#include <wlr/util/log.h>
#include <wlr/render/allocator.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_subcompositor.h>
#include <wlr/types/wlr_data_device.h>
#include <wlr/types/wlr_output_layout.h>

#include "tvc_server.h"
#include "event_callbacks.h"

struct tvc_server server = {0};

void tvc_terminate(int exit_code){
    wl_display_terminate(server.display);
}


int main(int argc, char *argv[]){
    wlr_log_init(WLR_DEBUG, nullptr);

    
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

    
	//wl_list_init(&server.outputs);
	//    server.newOutput.notify = server_new_output;
        

    return EXIT_SUCCESS;
}
