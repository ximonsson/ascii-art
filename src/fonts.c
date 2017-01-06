#include <stdio.h>
#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define WHITESPACE      "."
#define LINE_HEIGHT     1.5

static FT_Library   library;    // FreeType library
static FT_Face      face;       // FreeType face to use
static int          face_size;
static int          block_width,
                    block_height;

/**
 *  Load a glyph from input character.
 *  Glyph is loaded to, and can be accessed through, the current loaded face.
 *  Returns non-zero if a glyph could not be found.
 */
static int load_glyph (unsigned int character)
{
    // get character index in glyph map
    FT_UInt index = FT_Get_Char_Index (face, character);
    if (index == 0)
    {
        // fprintf (stderr, "could not get glyph index\n");
        return 1;
    }

    // load the glyph from current face
    FT_Error error = FT_Load_Glyph (face, index, FT_LOAD_DEFAULT);
    if (error)
    {
        fprintf (stderr, "Error loading glyph (%d)\n", error);
        return 1;
    }

    // make sure to convert to a monochone bitmap
	// TODO load anti aliased instead of monochrome
    error = FT_Render_Glyph (face->glyph, FT_RENDER_MODE_MONO);
    if (error)
    {
        fprintf (stderr, "Could not render glyph to monochrome bitmap (%d)\n", error);
        return 1;
    }

    return 0;
}

/**
 *  Return glyph luminance at coordinates.
 */
static inline unsigned char at (int x, int y)
{
    FT_GlyphSlot glyph = face->glyph;

    // outside glyph bitmap on x-axis
    if (x < glyph->bitmap_left || x >= glyph->bitmap.width + glyph->bitmap_left)
        return 0;

    // outside glyph bitmap on y-axis
    if (y < face->size->metrics.y_ppem - glyph->bitmap_top || y >= ((face->size->metrics.y_ppem - glyph->bitmap_top) + glyph->bitmap.rows))
        return 0;

    // inside bitmap
    // @TODO: hardcoded to monochrome version at the moment
    y -= face->size->metrics.y_ppem - glyph->bitmap_top;
    x -= glyph->bitmap_left;
    int byte = glyph->bitmap.buffer[y * glyph->bitmap.pitch + x / 8];

    if ((byte << (x & 7)) & 0x80)
        return 255;
    else
        return 0;
}

/**
 *  Performs block matching algorithm and returns a score for the difference of the
 *  supplied block and the currently loaded glyph.
 *  The higher score, the more they are different.
 */
static unsigned int match_block (unsigned char* block)
{
    int diff = 0;
    for (int y = 0; y < block_height; y ++)
    {
        for (int x = 0; x < block_width; x ++)
        {
            diff += abs (block[y * block_width + x] - at (x, y));
        }
    }
    diff /= sqrt (block_height * block_width);
    return diff;
}


int ascii_art_load_fonts (int dimension)
{
    face_size = dimension;

    // init library
    FT_Error error = FT_Init_FreeType (&library);
    if (error)
    {
        fprintf (stderr, "error init ft library (%d)\n", error);
        return 1;
    }

    // @TODO load terminal font
    error = FT_New_Face (library, "font.ttf", 0, &face);
    if (error == FT_Err_Unknown_File_Format)
    {
        fprintf (stderr, "unknown file format\n");
        return 1;
    }
    else if (error)
    {
        fprintf (stderr, "error creating new face (%d)\n", error);
        return 1;
    }

    // @TODO set depending on terminal font size
    error = FT_Set_Pixel_Sizes (face, 0, dimension);
    if (error)
    {
        fprintf (stderr, "Error setting pixel size (%d)\n", error);
        return 1;
    }

    // calculate block size
    // block_width  = face->glyph->advance.x >> 6;
    block_width  = 7; // @TODO: remove hardcoded
    block_height = dimension * 1.5;

    return 0;
}


int ascii_art_unload_fonts ()
{
    FT_Error error = FT_Done_FreeType (library);
    if (error)
    {
        fprintf (stderr, "Error destroying library (%d)\n", error);
        return 1;
    }
    return 0;
}


unsigned char ascii_art_find_best_glyph (unsigned char* block)
{
    unsigned char best_match = ' ';
    unsigned int  best_score = 1 << 31;

    // loop over all standard ASCII characters
    for (unsigned char c = 0; c < 255; c ++)
    {
        // printf (" > testing: '%c' (%d) \n", c, c);
        if (load_glyph (c) != 0)
            continue;

        int score = match_block (block);
        if (score < best_score)
        {
            // printf (" > [new best] '%c': score: %d\n", c, score);
            best_score = score;
            best_match = c;
        }
    }

    return best_match;
}


int ascii_art_print_glyph (unsigned int character)
{
    if (load_glyph (character) != 0)
        return 1;

    FT_Bitmap bitmap = face->glyph->bitmap;
    printf (
        "'%c' [%d x %d (%d x %d)] (grays: %d, pitch: %d, pixel mode: %d)\n",
        (char) character, bitmap.width, bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top,
        bitmap.num_grays, bitmap.pitch, bitmap.pixel_mode
    );

    for (int y = 0; y < block_height; y ++)
    {
        for (int x = 0; x < block_width; x ++)
        {
            unsigned char v = at (x, y);
            if (v == 0)
                printf (WHITESPACE);
            else
                printf ("@");
        }
        printf ("\n");
    }

    printf ("advance: %ld x %ld\n", face->glyph->advance.x >> 6, face->glyph->advance.y >> 6);
    return 0;
}


void ascii_art_print_face_information ()
{
    printf ("Family Name        %s\n",  face->family_name);
    printf ("Style Name         %s\n",  face->style_name);
    printf ("Face Size          %d x %d pixels per EM\n",  face->size->metrics.x_ppem, face->size->metrics.y_ppem);
    printf ("Glyphs             %ld\n", face->num_glyphs);

    if (face->available_sizes)
    {
        printf ("Available Sizes:\n");
        for (int i = 0; i < face->num_fixed_sizes; i ++)
        {
            FT_Bitmap_Size size = face->available_sizes[i];
            printf ("  [%d x %d]", size.width, size.height);
        }
        printf ("\n");
    }
}


void ascii_art_get_block_dimensions (int* width, int* height)
{
    *width  = block_width;
    *height = block_height;
}
