#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <wayland-client.h>
#include "MacroDefine.h"
#include "Memory.h"

struct tagWLBody
{
	struct wl_display 		*display;
	struct wl_registry 		*registry;
	struct wl_shell		 	*shell;
	struct wl_shm			*shm;
	struct wl_surface		*surface;
	struct wl_buffer		*buffer;
	struct wl_callback		*frame_callback;
	struct wl_compositor 	*compositor;

	tagWLBody() 
		: display(NULL)
		, registry(NULL)
		, shell(NULL)
		, shm(NULL)
		, buffer(NULL)
		, frame_callback(NULL)
		, compositor(NULL)
	{}
} *g_pWLDataBody;

void global_registry_handler(void *data,
		       struct wl_registry *registry,
		       uint32_t name,
		       const char *interface,
		       uint32_t version)
{
	printf("get event: %s, id: %d\n", interface, name);
	
	assert(NULL != g_pWLDataBody);

	if ( strcmp(interface, "wl_compositor") == 0 ) {
		g_pWLDataBody->compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
	} else if( strcmp(interface, "wl_shell") == 0 ) {
		g_pWLDataBody->shell = (struct wl_shell*)wl_registry_bind(registry, name, &wl_shell_interface, 1);	
	} else if( strcmp(interface, "wl_shm") == 0 ) {
		g_pWLDataBody->shm = (struct wl_shm*)wl_registry_bind(registry, name, &wl_shm_interface, 1);
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

void shell_surface_ping(void *data,
		     			struct wl_shell_surface *wl_shell_surface,
		     			uint32_t serial)
{
	
}

void shell_surface_configure(void *data,
			  struct wl_shell_surface *wl_shell_surface,
			  uint32_t edges,
			  int32_t width,
			  int32_t height)
{

}

void shell_surface_popup_done(void *data,
			   struct wl_shell_surface *wl_shell_surface)
{

}

struct wl_shell_surface_listener shell_surface_listener = {
	/**
	 * ping client
	 * @param serial serial number of the ping
	 */
	.ping = shell_surface_ping,
	/**
	 * suggest resize
	 * @param edges how the surface was resized
	 * @param width new width of the surface
	 * @param height new height of the surface
	 */
	.configure = shell_surface_configure,
	/**
	 * popup interaction is done
	 */
	.popup_done = shell_surface_popup_done,
};

void shm_format(void *data,
		       struct wl_shm *wl_shm,
		       uint32_t format)
{

}

struct wl_shm_listener shm_listener = {
	.format = shm_format,
};

int ht = 0;

void frame_done(void *data,
		     struct wl_callback *wl_callback,
		     uint32_t callback_data)
{
	wl_callback_destroy(g_pWLDataBody->frame_callback);
	if ( ht == 0 ) ht = HEIGHT;
	wl_surface_damage(g_pWLDataBody->surface, g_pWLDataBody->buffer, 0, 0, DEF_WIDTH, ht--);
	
	paint_pixels();
	frame_callback

}

struct wl_callback_listener frame_listener = {
	/**
	 * done event
	 *
	 * Notify the client when the related request is done.
	 * @param callback_data request-specific data for the callback
	 */
	.done = frame_done;
};

int main(int argc, const char *argv[])
{
	g_pWLDataBody = (struct tagWLBody*)malloc(sizeof(tagWLBody));
	assert(NULL != g_pWLDataBody);

	g_pWLDataBody->display = wl_display_connect(NULL);
	assert( NULL != g_pWLDataBody->display );

	g_pWLDataBody->registry = wl_display_get_registry(g_pWLDataBody->display);
	wl_registry_add_listener(g_pWLDataBody->registry, &register_listener, NULL);
	
	wl_display_dispatch(g_pWLDataBody->display);
	wl_display_roundtrip(g_pWLDataBody->display);

	assert( NULL != g_pWLDataBody->shell );
	assert( NULL != g_pWLDataBody->compositor );
	assert( NULL != g_pWLDataBody->shm );

	wl_shm_add_listener(g_pWLDataBody->shm, &shm_listener, NULL);
	
	g_pWLDataBody->surface = wl_compositor_create_surface(g_pWLDataBody->compositor);
	assert(NULL != g_pWLDataBody->surface);
	wl_shell_surface* shell_surface = wl_shell_get_shell_surface(g_pWLDataBody->shell, g_pWLDataBody->surface); 
	assert(NULL != shell_surface);

	wl_shell_surface_set_toplevel(shell_surface);
	wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);

	const Memory::tagMemoryNode* pNodeBuff = Memory::getInstance()->createMemory( DEF_WIDTH * DEF_HEIGHT * 4 );
	assert(NULL != pNodeBuff);

	struct wl_shm_pool* pool = wl_shm_create_pool(g_pWLDataBody->shm, pNodeBuff->fd, pNodeBuff->size);
	assert(NULL != pool);

	g_pWLDataBody->buffer = wl_shm_pool_create_buffer(pool, 0, DEF_WIDTH, DEF_HEIGHT, DEF_WIDTH * 4, WL_SHM_FORMAT_XRGB8888);
	wl_shm_pool_destroy(pool);

	wl_surface_attach(surface, g_pWLDataBody->buffer, 0, 0);
	wl_surface_commit(surface);

	g_pWLDataBody->frame_callback =  wl_surface_frame(surface);
	assert(NULL != g_pWLDataBody->frame_callback);

	wl_callback_add_listener(frame_callback, &frame_listener, NULL);

	uint32_t *pixel = (uint32_t*)pNodeBuff->pData;
	for ( int index = 0; index < DEF_WIDTH * DEF_HEIGHT; ++index ) {
		*pixel++ = 0xffff;
	}

	wl_display_disconnect(g_pWLDataBody->display);
	return 0;
}
