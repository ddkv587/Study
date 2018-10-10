#ifndef __RENDERENGINEH__
#define __RENDERENGINEH__

#include <screen/screen.h>
#include "Typedef.h"

struct tagRectangle
{
    INT     iX;
    INT     iY;
    UINT    uiWidth;
    UINT    uiHeight;
};

struct tagScreenContext
{
    
    screen_context_t	    screen_ctx;	// connection to screen windowing system
    screen_display_t        *pScreen_dispaly;

    INT                     iDisplayID;
    CHAR                    strGroupName[128];
};

struct tagWindowContext
{ 
    screen_window_t		        screen_win;	// native handle for our window
	screen_buffer_t		        pBuf;
	BYTE*				        pbtBufData;
    
    struct tagRectangle         position; 
    struct tagWindowContext     *pNext;
    CHAR                        strName[128];
};

extern BOOLEAN  initializeRenderEngine( struct tagScreenContext *pScreenContext, struct tagWindowContext *pWindowContext );
extern void     unInitializeRenderEngine( struct tagScreenContext *pScreenContext, struct tagWindowContext *pWindowContext );
extern BOOLEAN  createWindow( struct tagScreenContext *pScreenContext, struct tagWindowContext *pWindowContext, INT type );
extern BOOLEAN  destroyWindow( struct tagWindowContext *pWindowContext );
extern BOOLEAN  updateWindow( struct tagWindowContext *pWindowContext, INT x1, INT y1, INT x2, INT y2, INT flag );
extern BOOLEAN  setVisible( struct tagWindowContext *pWindowContext, BOOLEAN isVisible );
extern BOOLEAN  setPosition( struct tagWindowContext *pWindowContext, INT x, INT y );
extern BOOLEAN  setSize( struct tagWindowContext *pWindowContext, INT w, INT h );			
extern BOOLEAN  setAlpha( struct tagWindowContext *pWindowContext, INT alpha);	
extern BOOLEAN  setName( struct tagWindowContext *pWindowContext, CHAR *strName);			
#endif