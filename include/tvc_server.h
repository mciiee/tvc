#ifndef TVC_SERVER_H
#define TVC_SERVER_H
#include <wayland-server-core.h>

struct tvc_server {
    struct wl_display *display;
    struct wlr_backend *backend;
    struct wlr_renderer *renderer;
    struct wlr_allocator *allocator;


    struct wlr_compositor *compositor;
    struct wlr_subcompositor *subcompositor;
    struct wlr_data_device_manager *dataDeviceManager;

    struct wlr_output_layout *outputLayout; 
    struct wl_list outputs;
	struct wl_listener newOutput;
};
#endif
