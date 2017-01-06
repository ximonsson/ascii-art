#include "ascii_art.h"
#include "ascii_art_fonts.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FACE_SIZE   12
#define LINE_SPACE   1.5

/*
static unsigned char test_image[7 * 18] =
{
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0,
    0, 1, 0, 1, 1, 0, 0,
    0, 1, 0, 0, 0, 1, 0,
    0, 1, 0, 1, 0, 1, 0,
    0, 1, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 1, 0, 0, 0, //______________baseline_________________
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
};
*/

/**
 *  Load an image from disk.
 */
static int load_image (const char* path, unsigned char** buf, int* size)
{
    // open file
    FILE* fp = fopen (path, "rb");
    if (fp == 0)
        return 1;

    // get image size
    fseek (fp, 0, SEEK_END);
    *size = ftell (fp);
    *buf  = malloc (*size);

    // read file contents
    fseek (fp, 0, SEEK_SET);
    if (fread (*buf, 1, *size, fp) != *size)
        return 1;

    return 0;
}


int main (int argc, char** argv)
{
    // check enough args
    if (argc < 2)
    {
        fprintf (stderr, "not enough arguments\n");
        return 1;
    }

    // load library
    int ret = ascii_art_init (FACE_SIZE, FACE_SIZE);
    if (ret != 0)
        return ret;

    // print some debug info
    ascii_art_print_face_information ();
    printf ("\n");

    // load image from disk
    int image_buf_size;
    unsigned char* image_buf;
    if (load_image (argv[1], &image_buf, &image_buf_size) != 0)
    {
        fprintf (stderr, "error loading image file\n");
        return 1;
    }

    // convert
    unsigned char* img;
    ascii_art_convert (image_buf, ASCII_ART_JPEG, image_buf_size, 7 * 3, 18 * 3, &img);
    printf ("%s\n", img);

    // unload library
    free (image_buf);
    free (img);
    ret = ascii_art_deinit ();
    return ret;
}
