#include <stdlib.h>

#include <wlr/backend.h>
#include "wayland-server-core.h"

int main(int argc, char *argv[]){
    auto eventLoop = wl_event_loop_create();
    auto backend = wlr_backend_autocreate(eventLoop, nullptr);
    (void)backend;
    return EXIT_SUCCESS;
}
