#include "event_callbacks.h"
#include "tvc_server.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wayland-util.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_output_layout.h>
#include <wlr/types/wlr_scene.h>

static void output_request_state(struct wl_listener *listener, void *data) {

}

static void output_destroy(struct wl_listener *listener, void *data) {

}

static void output_frame(struct wl_listener *listener, void *data) {
    struct tvc_output *output = wl_container_of(listener, output, frame);
    struct wlr_scene *scene = output->server->scene;

    struct wlr_scene_output *sceneOutput = wlr_scene_get_scene_output(scene, output->wlrOutput);

    wlr_scene_output_commit(sceneOutput, nullptr);

    struct timespec now;
    // TODO: Fix time
    //clock_gettime(CLOCK_MONOTONIC, &now);
    wlr_scene_output_send_frame_done(sceneOutput, &now);
}

// TODO: Handle new outputs
void server_new_output(struct wl_listener *listener, void *data){
    struct tvc_server *server = wl_container_of(listener, server, newOutput);
	struct wlr_output *wlrOutput = data;
    
    wlr_output_init_render(wlrOutput, server->allocator, server->renderer);
    struct wlr_output_state *state = &(struct wlr_output_state){};
    wlr_output_state_init(state);
    wlr_output_state_set_enabled(state, true);

    struct wlr_output_mode *mode = wlr_output_preferred_mode(wlrOutput);
    if (mode != nullptr) {
        wlr_output_state_set_mode(state, mode);
    }

    wlr_output_commit_state(wlrOutput, state);
    wlr_output_state_finish(state);

	/* Allocates and configures our state for this output */
    struct tvc_output *output = calloc(1, sizeof(*output));
    output->wlrOutput = wlrOutput;
    output->server = server;

    output->frame.notify = output_frame;
    wl_signal_add(&wlrOutput->events.frame, &output->frame);

    output->requestState.notify = output_request_state;
    wl_signal_add(&wlrOutput->events.request_state, &output->requestState);

    output->destroy.notify = output_destroy;
    wl_signal_add(&wlrOutput->events.destroy, &output->destroy);

    wl_list_insert(&server->outputs, &output->link);

    // Adds this to the output layout. 
    // Outputs are arranged from left-to-right in order they appeear
    // The output layout utility automatically adds a wl_output global to the display, 
	// which Wayland clients can see to find out information about th output,
    // such as DPI, scale factor, manufacturer, etc.
    // TODO: Add user configuration for output arrangement
    struct wlr_output_layout_output *lOutput = wlr_output_layout_add_auto(server->outputLayout, wlrOutput);
    struct wlr_scene_output *sceneOutput = wlr_scene_output_create(server->scene, wlrOutput);
    wlr_scene_output_layout_add_output(server->sceneLayout, lOutput, sceneOutput);
}

