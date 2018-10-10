#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "ImageDecode.h"

extern int errno;
BOOLEAN loadBMP( const CHAR *strPath, UINT *width, UINT *height, BYTE **buffer )
{
    int fd = 0;
    if ( ( fd = open( strPath, O_RDONLY ) ) == -1 ) {
        return FALSE;
    }
    struct tagBitMapFileHeader bitMapFileHeader;
	struct tagBitMapInfoHeader bitMapInfoHeader;

    if ( SIZE_BITMAP_FILEHEADER != read( fd, &bitMapFileHeader, SIZE_BITMAP_FILEHEADER ) ) {
        goto err_file;
    }

    if (bitMapFileHeader.ucType[0] != 'B' && bitMapFileHeader.ucType[1] != 'M') {
	    goto err_file;
	}

    if ( SIZE_BITMAP_INFOHEADER != read( fd, &bitMapInfoHeader, SIZE_BITMAP_INFOHEADER ) ) {
        goto err_file;
    }

    if ( bitMapInfoHeader.lWidth == 0 || bitMapInfoHeader.lHeight == 0 \
		    || (bitMapInfoHeader.usBitCount != 8 && bitMapInfoHeader.usBitCount != 24 && bitMapInfoHeader.usBitCount != 32 ) ) {
        goto err_file;
	}

    INT			    iStride	        = 0;
	INT			    iColorMapSize   = 0;
    BYTE		    *pbtSrcDIB      = NULL;		
    struct tagPixel *pPixelColorMap = NULL;
	
    if (bitMapInfoHeader.usBitCount == 8) {
        iColorMapSize = bitMapFileHeader.ulOffBits - SIZE_BITMAP_FILEHEADER - SIZE_BITMAP_INFOHEADER;
        if (iColorMapSize > 0) {
            pPixelColorMap = (struct tagPixel*)malloc(iColorMapSize);
            if ( iColorMapSize != read( fd,  (BYTE*)pPixelColorMap, iColorMapSize ) ) {
                free( pPixelColorMap );
                goto err_file;
            }
        } else {
            goto err_file;
        }
    } else {
        iColorMapSize = bitMapFileHeader.ulOffBits - SIZE_BITMAP_FILEHEADER - SIZE_BITMAP_INFOHEADER;
        if (iColorMapSize > 0) {
           if ( iColorMapSize != lseek( fd, iColorMapSize, SEEK_CUR ) ) {
               //printf("lseek error\n");
               ;
           }
        }
    }

    *buffer = (BYTE*)malloc( bitMapInfoHeader.lWidth * bitMapInfoHeader.lHeight * 4 );   
    if ( *buffer == NULL ) goto err_PixelColorMap;

    iStride = ( bitMapInfoHeader.lWidth * bitMapInfoHeader.usBitCount + 31 ) / 32 * 4;
    UINT uiSize = (UINT)( iStride * bitMapInfoHeader.lHeight );
    pbtSrcDIB = (BYTE*)malloc( uiSize );

    if ( pbtSrcDIB == NULL ) goto err_buffer;
    if ( uiSize != read( fd, pbtSrcDIB, uiSize ) ) goto err_srcdib;
    
    BYTE*   pbtPixelDstCur  = *buffer;
	BYTE*   pbtPixelSrcCur  = NULL;
    switch (bitMapInfoHeader.usBitCount ) {
        case 8:
            {
                UINT i, j;
                for ( i = 0; i < bitMapInfoHeader.lHeight; ++i )
				{
					pbtPixelSrcCur = pbtSrcDIB + iStride * ( bitMapInfoHeader.lHeight - 1 - i );
					
					for ( j = 0; j < bitMapInfoHeader.lWidth; ++j )
					{
						*pbtPixelDstCur++ = pPixelColorMap[*pbtPixelSrcCur].btB;
						*pbtPixelDstCur++ = pPixelColorMap[*pbtPixelSrcCur].btG;
						*pbtPixelDstCur++ = pPixelColorMap[*pbtPixelSrcCur].btR;
						*pbtPixelDstCur++ = 255;				// alpha

						pbtPixelSrcCur++;
					}
				}
            }
            break;
        case 24:
            {
                UINT i, j;
                for ( i = 0; i < bitMapInfoHeader.lHeight; ++i )
                {
                    pbtPixelSrcCur = pbtSrcDIB + iStride * ( bitMapInfoHeader.lHeight - 1 - i );
                    for ( j = 0; j < bitMapInfoHeader.lWidth; ++j )
                    {
                        *pbtPixelDstCur++ = pbtPixelSrcCur[0];	// blue
                        *pbtPixelDstCur++ = pbtPixelSrcCur[1];	// green
                        *pbtPixelDstCur++ = pbtPixelSrcCur[2];	// red
                        *pbtPixelDstCur++ = 255;				// alpha

                        pbtPixelSrcCur += 3;
                    }
                }
            }
            break;
        case 32:
            {
                UINT i, j;
                for ( i = 0; i < bitMapInfoHeader.lHeight; ++i )
                {
                    pbtPixelSrcCur = pbtSrcDIB + iStride * ( bitMapInfoHeader.lHeight - 1 - i );
                    for ( j = 0; j < bitMapInfoHeader.lWidth; ++j )
                    {
                        *pbtPixelDstCur++ = pbtPixelSrcCur[0];	// blue
                        *pbtPixelDstCur++ = pbtPixelSrcCur[1];	// green
                        *pbtPixelDstCur++ = pbtPixelSrcCur[2];	// red
                        *pbtPixelDstCur++ = pbtPixelSrcCur[3];				// alpha

                        pbtPixelSrcCur += 4;
                    }
                }
            }
            break;
    }
    if ( pPixelColorMap != NULL ) free( pPixelColorMap );
    if ( pbtSrcDIB != NULL )  free( pbtSrcDIB );   

    *width  = bitMapInfoHeader.lWidth;
    *height = bitMapInfoHeader.lHeight;

    return TRUE;

err_srcdib:
    if ( pbtSrcDIB != NULL )  free( pbtSrcDIB );   

err_buffer:
    if ( *buffer != NULL ) free( *buffer );   

err_PixelColorMap:
    if ( pPixelColorMap != NULL ) free( pPixelColorMap );

err_file:
    close(fd);

    return FALSE;
}