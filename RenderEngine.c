#include <string.h>
#include <stdlib.h>
#include "RenderEngine.h"

//#ifdef OS_QNX
	BOOLEAN initializeRenderEngine(struct tagScreenContext *pScreenContext, struct tagWindowContext *pWindowContext)
	{
		if ( NULL == pWindowContext || NULL == pScreenContext ) return FALSE;

		// create context
		if ( screen_create_context( &pScreenContext->screen_ctx, SCREEN_APPLICATION_CONTEXT ) != 0 ) return FALSE;
		
		// create main screen
		if ( !createWindow( pScreenContext, pWindowContext, SCREEN_APPLICATION_WINDOW ) ) goto err_context;
		//create window group
		if ( screen_create_window_group( pWindowContext->screen_win, pScreenContext->strGroupName ) != 0 ) goto err_win;
		//set screen name
		CHAR acName[] = "HMI";
		if ( screen_set_window_property_cv(pWindowContext->screen_win, SCREEN_PROPERTY_ID_STRING, strlen(acName), acName) != 0 ) goto err_win;
		
		// attach screen to display
		INT iDisplayCount = 0;
		screen_get_context_property_iv( pScreenContext->screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &iDisplayCount );
		if ( iDisplayCount == 0 || pScreenContext->iDisplayID > iDisplayCount ) goto err_context;
		pScreenContext->pScreen_dispaly = calloc( iDisplayCount, sizeof(screen_display_t) );
		screen_get_context_property_pv( pScreenContext->screen_ctx,  SCREEN_PROPERTY_DISPLAYS, (void **)pScreenContext->pScreen_dispaly);
		if ( screen_set_window_property_pv( pWindowContext->screen_win, SCREEN_PROPERTY_DISPLAY, (void **)&pScreenContext->pScreen_dispaly[pScreenContext->iDisplayID]) != 0 ) goto err_win;
		
		// hide screen 
		INT iVisible = 0;
		if ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_VISIBLE, &iVisible ) != 0 ) goto err_win;
		
		printf("initialize success!! \n");
		return TRUE;

err_win:
		screen_destroy_window(pWindowContext->screen_win);		
			
err_context:
		screen_destroy_context(pScreenContext->screen_ctx);
		
		free(pScreenContext->pScreen_dispaly);
		pScreenContext->pScreen_dispaly = NULL;
		printf("initialize error!! \n");
		
		return FALSE;
	}

	void unInitializeRenderEngine( struct tagScreenContext *pScreenContext, struct tagWindowContext *pWindowContext )
	{
		// must destroy all sub window before
		if ( NULL == pWindowContext || NULL == pScreenContext ) return;

		screen_destroy_window(pWindowContext->screen_win);	
		screen_destroy_context(pScreenContext->screen_ctx);
		free(pScreenContext->pScreen_dispaly);
	}

	//type: SCREEN_APPLICATION_WINDOW, SCREEN_CHILD_WINDOW or SCREEN_EMBEDDED_WINDOW
	BOOLEAN createWindow( struct tagScreenContext *pScreenContext, struct tagWindowContext *pWindowContext, INT type )
	{
		if ( NULL == pScreenContext || NULL == pWindowContext ) return FALSE;

		// create window and join window group if child
		if ( screen_create_window_type(&pWindowContext->screen_win, pScreenContext->screen_ctx, type) != 0 ) return FALSE;
		if ( type == SCREEN_CHILD_WINDOW || type == SCREEN_EMBEDDED_WINDOW ) {
			if ( screen_join_window_group(pWindowContext->screen_win, pScreenContext->strGroupName) != 0 ) 
				goto err_win;
		}
		
		// set window usage
		INT usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
		if ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_USAGE, &usage ) != 0 ) goto err_win;
		
		// set window transparency
		int transparency = SCREEN_TRANSPARENCY_SOURCE_OVER;
		if ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_TRANSPARENCY, &transparency ) != 0 ) goto err_win;
    	
		// set window name
		if ( screen_set_window_property_cv( pWindowContext->screen_win, SCREEN_PROPERTY_ID_STRING, strlen( pWindowContext->strName ), pWindowContext->strName ) != 0 ) goto err_win;
		
		// create window buffer and get buffer ptr
		if ( screen_create_window_buffers( pWindowContext->screen_win, 1 ) != 0 ) goto err_win;
		screen_get_window_property_pv( pWindowContext->screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&(pWindowContext->pBuf) );
		screen_get_buffer_property_pv( pWindowContext->pBuf, SCREEN_PROPERTY_POINTER, (void **)&(pWindowContext->pbtBufData) );

		// set buffer format, not work!! BGRA8888 is default
		INT format = SCREEN_FORMAT_RGBA8888;
		if ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_FORMAT, &format ) != 0 ) goto err_buffer;

		// set position
		setPosition( pWindowContext, pWindowContext->position.iX, pWindowContext->position.iY );

		// set size
		setSize( pWindowContext, pWindowContext->position.uiWidth, pWindowContext->position.uiHeight);

		printf("createWindow success!! \n");
		return TRUE;

err_buffer:
		//screen_destroy_buffer( pWindowContext->pBuf );

err_win:
		screen_destroy_window( pWindowContext->screen_win );
		printf("createWindow error!! \n");	
		return FALSE;
	}

	BOOLEAN destroyWindow( struct tagWindowContext *pWindowContext )
	{
		if ( NULL == pWindowContext ) return FALSE;
		if ( 0 != screen_leave_window_group( pWindowContext->screen_win ) ) return FALSE;

		screen_destroy_window( pWindowContext->screen_win );
		return TRUE;
	}

	BOOLEAN updateWindow( struct tagWindowContext *pWindowContext, INT x1, INT y1, INT x2, INT y2, INT flag )
	{
		if ( NULL == pWindowContext ) return FALSE;
		INT rect[] = { x1, y1, x2, y2 };
		return ( 0 == screen_post_window( pWindowContext->screen_win, pWindowContext->pBuf, 1, rect, 0 ) ); 
	}

	BOOLEAN setVisible( struct tagWindowContext *pWindowContext, BOOLEAN isVisible )
	{
		if ( NULL == pWindowContext ) return FALSE;
		INT iValue = 0;
		if ( isVisible ) iValue = 1;
		return ( 0 == screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_VISIBLE, &iValue ) );
	}

	BOOLEAN setPosition( struct tagWindowContext *pWindowContext, INT x, INT y )
	{
		if ( NULL == pWindowContext ) return FALSE;
		INT posi[] = { x, y };
		return ( 0 == screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_POSITION, posi ) );
	}

	BOOLEAN setSize( struct tagWindowContext *pWindowContext, INT w, INT h )
	{
		if ( NULL == pWindowContext ) return FALSE;
		INT size[] = { w, h };
		if ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_SIZE, size ) != 0 ) {
			printf("set SCREEN_PROPERTY_SIZE error\n");
			return FALSE;
		}

		if ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_BUFFER_SIZE, size ) != 0 ) {
			printf("set SCREEN_PROPERTY_BUFFER_SIZE error\n");
			return FALSE;
		}

		screen_get_window_property_pv( pWindowContext->screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&(pWindowContext->pBuf) );
		screen_get_buffer_property_pv( pWindowContext->pBuf, SCREEN_PROPERTY_POINTER, (void **)&(pWindowContext->pbtBufData) );

		return TRUE;
	}

	BOOLEAN setAlpha( struct tagWindowContext *pWindowContext, INT alpha)
	{
		// 0 - 255
		if ( NULL == pWindowContext || alpha < 0 || alpha > 255 ) return FALSE;
		
		return ( screen_set_window_property_iv( pWindowContext->screen_win, SCREEN_PROPERTY_GLOBAL_ALPHA, &alpha ) == 0 );
	}

	BOOLEAN setName( struct tagWindowContext *pWindowContext, CHAR *strName )
	{
		if ( NULL == pWindowContext || NULL == strName ) return FALSE;

		return ( screen_set_window_property_cv( pWindowContext->screen_win,  SCREEN_PROPERTY_ID_STRING, strlen( strName ), strName ) == 0 );		
	}

//#endif