/** -------------------------------------------------------------------------
 *  File: ascii_art_fonts.h
 *  Description:
 *  ------------------------------------------------------------------------ */
#ifndef ASCII_ART_FONTS_H
#define ASCII_ART_FONTS_H

/**
 *  Init the ascii art applications font library,
 *  using the supplied dimensions for character face size.
 */
int ascii_art_load_fonts (int /* face_dimension */) ;

/**
 *  Deinitiliaze font library used.
 */
int ascii_art_unload_fonts () ;

/**
 *  Performs block matching algorithm to find the best suiting glyph
 *  compared to the supplied block at input.
 *  Returns which ascii character to use.
 */
unsigned char ascii_art_find_best_glyph (unsigned char* /* buffer */) ;

/**
 *  Dump face information to stdout.
 */
void ascii_art_print_face_information () ;

/**
 *  Debugging function to print the bitmap of a character to stdout.
 */
int ascii_art_print_glyph (unsigned int /* character */) ;

/**
 *  Get block dimensions calculated after the size of the face.
 *  Width depends of face advance value, and height is set to linespace (x1.5 of face size).
 */
void ascii_art_get_block_dimensions (int* /* width */, int* /* height */) ;

#endif
