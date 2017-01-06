/** ------------------------------------------------------------------------
 *  File: ascii_art.h
 *  Decription:
 * ------------------------------------------------------------------------ */
#ifndef ASCII_ART_H
#define ASCII_ART_H

/**
 *  Input mode for the supplied image.
 */
typedef enum _ascii_art_colorspace
{
    ASCII_ART_YUV420,
    ASCII_ART_YUV422,
    ASCII_ART_RGB8,
    ASCII_ART_RGBA8,
    ASCII_ART_Y,
    ASCII_ART_U,
    ASCII_ART_V,
}
ascii_art_colorspace;

/**
 *  Supported image types used as input data.
 */
typedef enum _ascii_art_image_type
{
    ASCII_ART_JPEG,
    ASCII_ART_PNG,
    ASCII_ART_GIF,
    ASCII_ART_RAW,
}
ascii_art_image_type;


/**
 *  Init image to ascii art converter.
 *  Returns non-zero on failure.
 */
int ascii_art_init (int /* w */, int /* h */) ;

/**
 *  Deinitialize ascii art converter. Freeing necessary memmory.
 *  Returns non-zero code on error.
 */
int ascii_art_deinit () ;

/**
 *  Convert an image to ascii art.
 *
 *  image parameter points to image data of image_type type and
 *  dimensions image_width x image_height.
 *
 *  Result will be stored to destination, which is allocated dynamically,
 *  and later has to be freed by the caller.
 *
 *  Returns non-zero error code on failure.
 */
int ascii_art_convert (unsigned char*       /* image_buffer */,
                       ascii_art_image_type /* image_type */,
                       int                  /* image_buf_size */,
                       int                  /* image_width */,
                       int                  /* image_height */,
                       unsigned char**      /* destination */) ;


#endif
