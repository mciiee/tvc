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

    struct wlr_scene *scene;
    struct wlr_scene_output_layout *sceneLayout;
};

struct tvc_output {
	struct wl_list link;
	struct tvc_server *server;
	struct wlr_output *wlrOutput;
	struct wl_listener frame;
	struct wl_listener requestState;
	struct wl_listener destroy;
};
#endif
