#ifndef RDRAWING_H
#define RDRAWING_H

#include <raylib.h>

#define	ALICEBLUE              CLITERAL(Color){ 0xF0, 0xF8, 0xFF, 255 }
#define	ANTIQUEWHITE           CLITERAL(Color){ 0xFA, 0xEB, 0xD7, 255 }  
#define	AQUA                   CLITERAL(Color){ 0x00, 0xFF, 0xFF, 255 }  
#define	AQUAMARINE             CLITERAL(Color){ 0x7F, 0xFF, 0xD4, 255 }  
#define	AZURE                  CLITERAL(Color){ 0xF0, 0xFF, 0xFF, 255 }  
#define	BISQUE                 CLITERAL(Color){ 0xFF, 0xE4, 0xC4, 255 }  
#define	BLANCHEDALMOND         CLITERAL(Color){ 0xFF, 0xEB, 0xCD, 255 }  
#define	BLUEVIOLET             CLITERAL(Color){ 0x8A, 0x2B, 0xE2, 255 }  
#define	BURLYWOOD              CLITERAL(Color){ 0xDE, 0xB8, 0x87, 255 }  
#define	CADETBLUE              CLITERAL(Color){ 0x5F, 0x9E, 0xA0, 255 }  
#define	CHARTREUSE             CLITERAL(Color){ 0x7F, 0xFF, 0x00, 255 }  
#define	CHOCOLATE              CLITERAL(Color){ 0xD2, 0x69, 0x1E, 255 }  
#define	CORAL                  CLITERAL(Color){ 0xFF, 0x7F, 0x50, 255 }  
#define	CORNFLOWERBLUE         CLITERAL(Color){ 0x64, 0x95, 0xED, 255 }  
#define	CORNSILK               CLITERAL(Color){ 0xFF, 0xF8, 0xDC, 255 }  
#define	CRIMSON                CLITERAL(Color){ 0xDC, 0x14, 0x3C, 255 }  
#define	CYAN                   CLITERAL(Color){ 0x00, 0xFF, 0xFF, 255 }  
#define	DARKCYAN               CLITERAL(Color){ 0x00, 0x8B, 0x8B, 255 }  
#define	DARKGOLDENROD          CLITERAL(Color){ 0xB8, 0x86, 0x0B, 255 }  
#define	DARKKHAKI              CLITERAL(Color){ 0xBD, 0xB7, 0x6B, 255 }  
#define	DARKMAGENTA            CLITERAL(Color){ 0x8B, 0x00, 0x8B, 255 }  
#define	DARKOLIVEGREEN         CLITERAL(Color){ 0x55, 0x6B, 0x2F, 255 }  
#define	DARKORANGE             CLITERAL(Color){ 0xFF, 0x8C, 0x00, 255 }  
#define	DARKORCHID             CLITERAL(Color){ 0x99, 0x32, 0xCC, 255 }  
#define	DARKRED                CLITERAL(Color){ 0x8B, 0x00, 0x00, 255 }  
#define	DARKSALMON             CLITERAL(Color){ 0xE9, 0x96, 0x7A, 255 }  
#define	DARKSEAGREEN           CLITERAL(Color){ 0x8F, 0xBC, 0x8F, 255 }  
#define	DARKSLATEBLUE          CLITERAL(Color){ 0x48, 0x3D, 0x8B, 255 }  
#define	DARKSLATEGRAY          CLITERAL(Color){ 0x2F, 0x4F, 0x4F, 255 }  
#define	DARKTURQUOISE          CLITERAL(Color){ 0x00, 0xCE, 0xD1, 255 }  
#define	DARKVIOLET             CLITERAL(Color){ 0x94, 0x00, 0xD3, 255 }  
#define	DEEPPINK               CLITERAL(Color){ 0xFF, 0x14, 0x93, 255 }  
#define	DEEPSKYBLUE            CLITERAL(Color){ 0x00, 0xBF, 0xFF, 255 }  
#define	DIMGRAY                CLITERAL(Color){ 0x69, 0x69, 0x69, 255 }  
#define	DODGERBLUE             CLITERAL(Color){ 0x1E, 0x90, 0xFF, 255 }  
#define	FIREBRICK              CLITERAL(Color){ 0xB2, 0x22, 0x22, 255 }  
#define	FLORALWHITE            CLITERAL(Color){ 0xFF, 0xFA, 0xF0, 255 }  
#define	FORESTGREEN            CLITERAL(Color){ 0x22, 0x8B, 0x22, 255 }  
#define	FUCHSIA                CLITERAL(Color){ 0xFF, 0x00, 0xFF, 255 }  
#define	GAINSBORO              CLITERAL(Color){ 0xDC, 0xDC, 0xDC, 255 }  
#define	GHOSTWHITE             CLITERAL(Color){ 0xF8, 0xF8, 0xFF, 255 }  
#define	GOLDENROD              CLITERAL(Color){ 0xDA, 0xA5, 0x20, 255 }  
#define	GREENYELLOW            CLITERAL(Color){ 0xAD, 0xFF, 0x2F, 255 }  
#define	HONEYDEW               CLITERAL(Color){ 0xF0, 0xFF, 0xF0, 255 }  
#define	HOTPINK                CLITERAL(Color){ 0xFF, 0x69, 0xB4, 255 }  
#define	INDIANRED              CLITERAL(Color){ 0xCD, 0x5C, 0x5C, 255 }  
#define	INDIGO                 CLITERAL(Color){ 0x4B, 0x00, 0x82, 255 }  
#define	IVORY                  CLITERAL(Color){ 0xFF, 0xFF, 0xF0, 255 }  
#define	KHAKI                  CLITERAL(Color){ 0xF0, 0xE6, 0x8C, 255 }  
#define	LAVENDER               CLITERAL(Color){ 0xE6, 0xE6, 0xFA, 255 }  
#define	LAVENDERBLUSH          CLITERAL(Color){ 0xFF, 0xF0, 0xF5, 255 }  
#define	LAWNGREEN              CLITERAL(Color){ 0x7C, 0xFC, 0x00, 255 }  
#define	LEMONCHIFFON           CLITERAL(Color){ 0xFF, 0xFA, 0xCD, 255 }  
#define	LIGHTBLUE              CLITERAL(Color){ 0xAD, 0xD8, 0xE6, 255 }  
#define	LIGHTCORAL             CLITERAL(Color){ 0xF0, 0x80, 0x80, 255 }  
#define	LIGHTCYAN              CLITERAL(Color){ 0xE0, 0xFF, 0xFF, 255 }  
#define	LIGHTGOLDENRODYELLOW   CLITERAL(Color){ 0xFA, 0xFA, 0xD2, 255 }  
#define	LIGHTGREEN             CLITERAL(Color){ 0x90, 0xEE, 0x90, 255 }  
#define	LIGHTPINK              CLITERAL(Color){ 0xFF, 0xB6, 0xC1, 255 }  
#define	LIGHTSALMON            CLITERAL(Color){ 0xFF, 0xA0, 0x7A, 255 }  
#define	LIGHTSEAGREEN          CLITERAL(Color){ 0x20, 0xB2, 0xAA, 255 }  
#define	LIGHTSKYBLUE           CLITERAL(Color){ 0x87, 0xCE, 0xFA, 255 }  
#define	LIGHTSLATEGRAY         CLITERAL(Color){ 0x77, 0x88, 0x99, 255 }  
#define	LIGHTSTEELBLUE         CLITERAL(Color){ 0xB0, 0xC4, 0xDE, 255 }  
#define	LIGHTYELLOW            CLITERAL(Color){ 0xFF, 0xFF, 0xE0, 255 }  
#define	LIGHTRED               CLITERAL(Color){ 0xFC, 0x54, 0x54, 255 }  
#define	LIGHTMAGENTA           CLITERAL(Color){ 0xFC, 0x54, 0xFC, 255 }  
#define	LIMEGREEN              CLITERAL(Color){ 0x32, 0xCD, 0x32, 255 }  
#define	LINEN                  CLITERAL(Color){ 0xFA, 0xF0, 0xE6, 255 }  
#define	MEDIUMAQUAMARINE       CLITERAL(Color){ 0x66, 0xCD, 0xAA, 255 }  
#define	MEDIUMBLUE             CLITERAL(Color){ 0x00, 0x00, 0xCD, 255 }  
#define	MEDIUMORCHID           CLITERAL(Color){ 0xBA, 0x55, 0xD3, 255 }  
#define	MEDIUMPURPLE           CLITERAL(Color){ 0x93, 0x70, 0xDB, 255 }  
#define	MEDIUMSEAGREEN         CLITERAL(Color){ 0x3C, 0xB3, 0x71, 255 }  
#define	MEDIUMSLATEBLUE        CLITERAL(Color){ 0x7B, 0x68, 0xEE, 255 }  
#define	MEDIUMSPRINGGREEN      CLITERAL(Color){ 0x00, 0xFA, 0x9A, 255 }  
#define	MEDIUMTURQUOISE        CLITERAL(Color){ 0x48, 0xD1, 0xCC, 255 }  
#define	MEDIUMVIOLETRED        CLITERAL(Color){ 0xC7, 0x15, 0x85, 255 }  
#define	MIDNIGHTBLUE           CLITERAL(Color){ 0x19, 0x19, 0x70, 255 }  
#define	MINTCREAM              CLITERAL(Color){ 0xF5, 0xFF, 0xFA, 255 }  
#define	MISTYROSE              CLITERAL(Color){ 0xFF, 0xE4, 0xE1, 255 }  
#define	MOCCASIN               CLITERAL(Color){ 0xFF, 0xE4, 0xB5, 255 }  
#define	NAVAJOWHITE            CLITERAL(Color){ 0xFF, 0xDE, 0xAD, 255 }  
#define	NAVY                   CLITERAL(Color){ 0x00, 0x00, 0x80, 255 }  
#define	OLDLACE                CLITERAL(Color){ 0xFD, 0xF5, 0xE6, 255 }  
#define	OLIVE                  CLITERAL(Color){ 0x80, 0x80, 0x00, 255 }  
#define	OLIVEDRAB              CLITERAL(Color){ 0x6B, 0x8E, 0x23, 255 }  
#define	ORANGERED              CLITERAL(Color){ 0xFF, 0x45, 0x00, 255 }  
#define	ORCHID                 CLITERAL(Color){ 0xDA, 0x70, 0xD6, 255 }  
#define	PALEGOLDENROD          CLITERAL(Color){ 0xEE, 0xE8, 0xAA, 255 }  
#define	PALEGREEN              CLITERAL(Color){ 0x98, 0xFB, 0x98, 255 }  
#define	PALETURQUOISE          CLITERAL(Color){ 0xAF, 0xEE, 0xEE, 255 }  
#define	PALEVIOLETRED          CLITERAL(Color){ 0xDB, 0x70, 0x93, 255 }  
#define	PAPAYAWHIP             CLITERAL(Color){ 0xFF, 0xEF, 0xD5, 255 }  
#define	PEACHPUFF              CLITERAL(Color){ 0xFF, 0xDA, 0xB9, 255 }  
#define	PERU                   CLITERAL(Color){ 0xCD, 0x85, 0x3F, 255 }  
#define	PLUM                   CLITERAL(Color){ 0xDD, 0xA0, 0xDD, 255 }  
#define	POWDERBLUE             CLITERAL(Color){ 0xB0, 0xE0, 0xE6, 255 }  
#define	ROSYBROWN              CLITERAL(Color){ 0xBC, 0x8F, 0x8F, 255 }  
#define	ROYALBLUE              CLITERAL(Color){ 0x41, 0x69, 0xE1, 255 }  
#define	SADDLEBROWN            CLITERAL(Color){ 0x8B, 0x45, 0x13, 255 }  
#define	SALMON                 CLITERAL(Color){ 0xFA, 0x80, 0x72, 255 }  
#define	SANDYBROWN             CLITERAL(Color){ 0xF4, 0xA4, 0x60, 255 }  
#define	SEAGREEN               CLITERAL(Color){ 0x2E, 0x8B, 0x57, 255 }  
#define	SEASHELL               CLITERAL(Color){ 0xFF, 0xF5, 0xEE, 255 }  
#define	SIENNA                 CLITERAL(Color){ 0xA0, 0x52, 0x2D, 255 }  
#define	SILVER                 CLITERAL(Color){ 0xC0, 0xC0, 0xC0, 255 }  
#define	SLATEBLUE              CLITERAL(Color){ 0x6A, 0x5A, 0xCD, 255 }  
#define	SLATEGRAY              CLITERAL(Color){ 0x70, 0x80, 0x90, 255 }  
#define	SNOW                   CLITERAL(Color){ 0xFF, 0xFA, 0xFA, 255 }  
#define	SPRINGGREEN            CLITERAL(Color){ 0x00, 0xFF, 0x7F, 255 }  
#define	STEELBLUE              CLITERAL(Color){ 0x46, 0x82, 0xB4, 255 }  
#define	TAN                    CLITERAL(Color){ 0xD2, 0xB4, 0x8C, 255 }  
#define	TEAL                   CLITERAL(Color){ 0x00, 0x80, 0x80, 255 }  
#define	THISTLE                CLITERAL(Color){ 0xD8, 0xBF, 0xD8, 255 }  
#define	TOMATO                 CLITERAL(Color){ 0xFF, 0x63, 0x47, 255 }  
#define	TURQUOISE              CLITERAL(Color){ 0x40, 0xE0, 0xD0, 255 }  
#define	WHEAT                  CLITERAL(Color){ 0xF5, 0xDE, 0xB3, 255 }  
#define	WHITESMOKE             CLITERAL(Color){ 0xF5, 0xF5, 0xF5, 255 }  
#define	YELLOWGREEN            CLITERAL(Color){ 0x9A, 0xCD, 0x32, 255 }  

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
	void ImageDrawPointEx(Image* dst,int x, int y, int pointSize, Color color);
	void ImageDrawLineEx(Image* dst,int x0, int y0, int x1, int y1, int lineWidth, Color color);
	void ImageDrawEllipseEx(Image* dst, int cx, int cy, int radiusX, int radiusY, int lineWidth, Color color);
	void ImageDrawCircleEx(Image* dst, int cx, int cy, int radius, int lineWidth, Color color);
	void ImageFillEllipseEx(Image* dst, int cx, int cy, int radiusX, int radiusY, Color fillColor);
	void ImageFillCircleEx(Image* dst, int cx, int cy, int radius, Color fillColor);
	void ImageFloodFill(Image* dst,int x, int y, Color borderColor, Color fillColor);
	void ImageFillPolygonEx(Image* dst,int* vertice_x,  int * vertice_y, int num_vertice, Color fillColor);
	void ImageDrawPolygonEx(Image* dst,int* vertice_x,  int * vertice_y, int num_vertice, int lineWidth, Color color);
	void ImageDrawPolylineEx(Image* dst,int* points_x,  int * points_y, int num_vertice, int lineWidth, Color color);
	void ImageFillTriangleEx(Image* dst, int x0, int y0, int x1, int y1, int x2, int y2, Color fillColor);
	
#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif
