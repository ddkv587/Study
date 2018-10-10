#ifndef __IMAGEDECODEH__
#define __IMAGEDECODEH__

#include "Typedef.h"

#pragma pack(push, bfhead, 1)
struct tagBitMapFileHeader {
    BYTE   ucType[2];
    ULONG  ulSize;
    USHORT usReserved1;
    USHORT usReserved2;
    ULONG  ulOffBits;
};

struct tagBitMapInfoHeader {
    ULONG  usSize;
    LONG   lWidth;
    LONG   lHeight;
    USHORT usPlanes;
    USHORT usBitCount;
    ULONG  blCompression;
    ULONG  blSizeImage;
    LONG   lXPelsPerMeter;
    LONG   lYPelsPerMeter;
    ULONG  lClrUsed;
    ULONG  lClrImportant;
};

struct tagPixel {
    BYTE	btB;
    BYTE	btG;
    BYTE	btR;
    BYTE	btA;
};
#pragma pack(pop, bfhead)

const static INT SIZE_BITMAP_FILEHEADER = sizeof(struct tagBitMapFileHeader);
const static INT SIZE_BITMAP_INFOHEADER = sizeof(struct tagBitMapInfoHeader);

extern BOOLEAN loadBMP( const CHAR *strPath, UINT *width, UINT *height, BYTE **buffer );

#endif