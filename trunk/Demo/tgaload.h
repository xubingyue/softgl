#pragma once

#include "..\inc\SoftGL.h"
/*  Definitions for bitflags  */
typedef unsigned int tgaFLAG;

#define TGA_DEFAULT            0x0000000000000000   /* In case we don't want any parameters */
#define TGA_FREE               0x0000000000000001   /* Bit flag 0 */
#define TGA_NO_PASS            0x0000000000000010   /* Bit flag 1 */
#define TGA_ALPHA              0x0000000000000100   /* Bit flag 2 */
#define TGA_LUMINANCE          0x0000000000001000   /* Bit flag 3 */
#define TGA_NO_MIPMAPS         0x0000000000010000   /* Bit flag 4 */
#define TGA_LOW_QUALITY        0x0000000000100000   /* Bit flag 5 */

#define GL_PROXY_TEXTURE_1D				0x8063
#define GL_PROXY_TEXTURE_2D				0x8064
#define GL_TEXTURE_PRIORITY				0x8066
#define GL_TEXTURE_RESIDENT				0x8067
#define GL_TEXTURE_BINDING_1D			0x8068
#define GL_TEXTURE_BINDING_2D			0x8069
#define GL_TEXTURE_INTERNAL_FORMAT		0x1003
#define GL_ALPHA4						0x803B
#define GL_ALPHA8						0x803C
#define GL_ALPHA12						0x803D
#define GL_ALPHA16						0x803E
#define GL_LUMINANCE4					0x803F
#define GL_LUMINANCE8					0x8040
#define GL_LUMINANCE12					0x8041
#define GL_LUMINANCE16					0x8042
#define GL_LUMINANCE4_ALPHA4			0x8043
#define GL_LUMINANCE6_ALPHA2			0x8044
#define GL_LUMINANCE8_ALPHA8			0x8045
#define GL_LUMINANCE12_ALPHA4			0x8046
#define GL_LUMINANCE12_ALPHA12			0x8047
#define GL_LUMINANCE16_ALPHA16			0x8048
#define GL_INTENSITY					0x8049
#define GL_INTENSITY4					0x804A
#define GL_INTENSITY8					0x804B
#define GL_INTENSITY12					0x804C
#define GL_INTENSITY16					0x804D
#define GL_R3_G3_B2						0x2A10
#define GL_RGB4							0x804F
#define GL_RGB5							0x8050
#define GL_RGB8							0x8051
#define GL_RGB10						0x8052
#define GL_RGB12						0x8053
#define GL_RGB16						0x8054
#define GL_RGBA2						0x8055
#define GL_RGBA4						0x8056
#define GL_RGB5_A1						0x8057
#define GL_RGBA8						0x8058
#define GL_RGB10_A2						0x8059
#define GL_RGBA12						0x805A
#define GL_RGBA16						0x805B


#define GL_FRONT_LEFT				0x0400
#define GL_FRONT_RIGHT				0x0401
#define GL_BACK_LEFT				0x0402
#define GL_BACK_RIGHT				0x0403
#define GL_AUX0					0x0409
#define GL_AUX1					0x040A
#define GL_AUX2					0x040B
#define GL_AUX3					0x040C
#define GL_COLOR_INDEX				0x1900
#define GL_LUMINANCE_ALPHA			0x190A
#define GL_ALPHA_BITS				0x0D55
#define GL_RED_BITS				0x0D52
#define GL_GREEN_BITS				0x0D53
#define GL_BLUE_BITS				0x0D54
#define GL_INDEX_BITS				0x0D51
#define GL_SUBPIXEL_BITS			0x0D50
#define GL_AUX_BUFFERS				0x0C00
#define GL_READ_BUFFER				0x0C02
#define GL_DRAW_BUFFER				0x0C01
#define GL_DOUBLEBUFFER				0x0C32
#define GL_STEREO				0x0C33
#define GL_BITMAP				0x1A00
#define GL_COLOR				0x1800
#define GL_DEPTH				0x1801
#define GL_STENCIL				0x1802
#define GL_DITHER				0x0BD0


/*
** GL_ARB_texture_compression
**
** Support:
**  GeForce
**  Radeon
**  ????   <- Any suggestions?
*/


typedef struct {
   unsigned char id_length;
   unsigned char colour_map_type;
   unsigned char image_type;

   // colourmap spec.  5 bytes
   short int     colour_map_first_entry;  // Ignore
   short int     colour_map_length;       // Usually 256
   unsigned char colour_map_entry_size;   // Usually 24-bit

   // image spec.  10 bytes
   short int     x_origin;  // Ignore
   short int     y_origin;  // Ignore
   short int     width;
   short int     height;
   unsigned char pixel_depth;       // Usually 24 or 32
   unsigned char image_descriptor;  // Ignore

   // Added for 'compeletness' :)
   int   components;
   int   bytes;

   GLenum tgaColourType;

} tgaHeader_t;


typedef struct {
   tgaHeader_t          info;
   unsigned char        *data;      /* Image data */
} image_t;


/* 'Public' functions */
void   tgaLoad        ( char *file_name, image_t *p, tgaFLAG mode );

void tgaFree ( image_t *p );
