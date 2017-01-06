#include "ascii_art.h"
#include "ascii_art_fonts.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 *  Decode JPEG image data from buffer to destination.
 *  The destination needs to manually be freed later.
 *  Returns non-zero on failure.
 */
static int decode_jpeg (unsigned char* image_buf, int buf_size, unsigned char** dest)
{
    return 0;
}

/**
 *  Init image to ascii art converter.
 *  Returns non-zero on failure.
 */
int ascii_art_init (int w, int h)
{
    return ascii_art_load_fonts (w);
}

/**
 *  Deinitialize ascii art converter. Freeing necessary memmory.
 *  Returns non-zero code on error.
 */
int ascii_art_deinit ()
{
    return ascii_art_unload_fonts ();
}


int ascii_art_convert (unsigned char*       image_buffer,
                       ascii_art_image_type image_type,
                       int                  image_buf_size,
                       int                  image_width,
                       int                  image_height,
                       unsigned char**      ascii_image)
{
    int bw, bh; // block width and height
    unsigned char* block;
    // int w = image_width;
    unsigned char* p;

    // setup buffers
    ascii_art_get_block_dimensions (&bw, &bh);
    block = malloc (bw * bh);

    *ascii_image = malloc (image_width / bw * image_height / bh + image_height / bh + 1);
    memset (*ascii_image, 0, image_width / bw * image_height / bh + image_height / bh + 1);
    p = *ascii_image;

    unsigned char* raw_data;

    // decode image if needed
    switch (image_type)
    {
        case ASCII_ART_JPEG:
            decode_jpeg (image_buffer, image_buf_size, &raw_data);
            // break;

        case ASCII_ART_GIF:
        case ASCII_ART_PNG:
            return 1;

        case ASCII_ART_RAW:
        default:
            // no decoding needed
            raw_data = image_buffer;
            break;
    }

    // match blocks in image to characters
    for (int y = 0; y < image_height; y += bh)
    {
        for (int x = 0; x < image_width; x += bw)
        {
            // copy block
            memset (block, 0, bw * bh);
            // copy dimensions
            int cy = image_height - y < bh ? image_height - y : bh;
            int cw = image_width  - x < bw ? image_width  - x : bw;
            for (int yy = 0; yy < cy; yy ++)
            {
                memcpy (&block[yy * bw], &image_buffer[yy * image_width + x], cw);
            }
            *p = ascii_art_find_best_glyph (block);
            p ++;
        }
        *p = '\n'; p ++;
    }

    free (block);
    free (raw_data);
    return 0;
}
