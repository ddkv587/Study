#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wayland-client.h>

struct tagWLBody
{
	struct wl_display 		*display;
	struct wl_registry 		*registry;
	struct wl_shell		 	*shell;
	struct wl_compositor 	*compositor;

	tagWLBody() {
		display 	= NULL;
		registry 	= NULL;
		shell 		= NULL;
		compositor	= NULL;
	}
} *g_pWLDataBody;


void global_registry_handler(void *data,
		       struct wl_registry *registry,
		       uint32_t name,
		       const char *interface,
		       uint32_t version)
{
	printf("get event: %s, id: %d\n", interface, name);

	if ( strcmp(interface, "wl_compositor") == 0 ) {
		global_compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1);
	} else if( strcmp(interface, "wl_shell") == 0 ) {
		global_shell = wl_registry_bind(registry, name, &wl_shell_interface, 1);	
	}
}

void global_registry_remove(void *data,
			      struct wl_registry *wl_registry,
			      uint32_t name)
{

}

static const struct wl_registry_listener register_listener = {
	.global 		= global_registry_handler,
	.global_remove 	= global_registry_remove 
};

int main(int argc, const char *argv[])
{
	g_pWLDataBody->display = wl_display_connect(NULL);
	assert( NULL == g_pWLDataBody->display );

	g_pWLDataBody->registry = wl_display_get_registry(g_pWLDataBody->display);
	wl_registry_add_listener(g_pWLDataBody->registry, &register_listener, NULL);
	
	wl_display_dispatch(g_pWLDataBody->display);
	wl_display_roundtrip(g_pWLDataBody->display);

	assert( NULL == g_pWLDataBody->shell );
	assert( NULL == g_pWLDataBody->compositor );
	
	wl_surface* surface = wl_compositor_create_surface(g_pWLDataBody->compositor);
	wl_shell_surface* shell_surface = wl_shell_get_shell_surface(g_pWLDataBody->shell, surface); 
	



	wl_display_disconnect(g_pWLDataBody->display);
	return 0;
}
