#include <stdio.h>
#include <stdlib.h>

void int_to_char_array(char* mem_ptr, int offset, int number) {
    mem_ptr[offset + 3] = (number >> 24) & 0xFF;
    mem_ptr[offset + 2] = (number >> 16) & 0xFF;
    mem_ptr[offset + 1] = (number >> 8) & 0xFF;
    mem_ptr[offset + 0] = number & 0xFF;
}

void pixel_to_char_array(char* mem_ptr, int offset, short r, short g, short b) {
    mem_ptr[offset + 2] = r;
    mem_ptr[offset + 1] = g;
    mem_ptr[offset + 0] = b;
}

void short_to_char_array(char* mem_ptr, int offset, short number) {
    mem_ptr[offset + 1] = (number >> 8) & 0xFF;
    mem_ptr[offset + 0] = number & 0xFF;
}

//size_t write_int_to_file(FILE* file, int number) {
//    return fwrite(int_to_char_array(number), 1, 4, file);
//}
//
//size_t write_short_to_file(FILE* file, short number) {
//    return fwrite(short_to_char_array(number), 1, 2, file);
//}

int main() {
    FILE* file = fopen("../image.bmp", "wb");
    if (file == NULL) return 1;

    // Telling the application what this file is
//    fprintf(file, "BM");
//    fprintf(file, "%ho", 10);
//    fwrite("bruh", 1, 4, file);
//    fwrite(int_to_char_array(10), 1, 4, file);

    int pixels_size = (3 * 2) + 2 + (3 * 2) + 2;

    // FILE HEADER //
    int header_size = 14 + 40;
    char* header = malloc(header_size);

    // Header field
    header[0] = 'B';
    header[1] = 'M';

    // BMP file size in bytes
    int_to_char_array(header, 2, header_size + pixels_size);

    // Reserved (Filler)
    short_to_char_array(header, 6, 0);
    short_to_char_array(header, 8, 0);

    // Offset of the actual bitmap image data
    int_to_char_array(header, 10, 54);


    // BITMAP INFO HEADER //

    // Header size in bytes. We're using BITMAPINFOHEADER so its 40.
    int_to_char_array(header, 14, 40);

    // Width and Height
    int_to_char_array(header, 18, 2);
    int_to_char_array(header, 22, 2);

    // Color planes...?
    short_to_char_array(header, 26, 1);

    // Color depth (lets go with general 24-bit)
    short_to_char_array(header, 28, 24);

    // Compression method
    int_to_char_array(header, 30, 0);

    // Image size...? But since we're going for uncompressed, we can leave this as 0?
    int_to_char_array(header, 34, 0);

    // Horizontal resolution  (pixels per meter??)
    int_to_char_array(header, 38, 2835);

    // Vertical resolution
    int_to_char_array(header, 42, 2835);

    // Number of colors (0 for default)
    int_to_char_array(header, 46, 0);

    // "Important" colors. Mostly ignored by software, so leaving this as 0
    int_to_char_array(header, 50, 0);

//     */
    fwrite(header, 1, header_size, file);
    char* pixels = malloc(pixels_size);
    pixel_to_char_array(pixels, 0, 0, 0, 255);
    pixel_to_char_array(pixels, 3, 255, 255, 255);
    pixel_to_char_array(pixels, 6 + 2, 255, 0, 0);
    pixel_to_char_array(pixels, 9 + 2, 0, 255, 0);

    fwrite(pixels, 1, pixels_size, file);
    fclose(file);
    free(header);
    free(pixels);

    printf("Image created successfully!!");
    return 0;
}
