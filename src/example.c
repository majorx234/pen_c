#include <stdio.h>
#include "olive.c"
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

#define COLS 8
#define ROWS 6
#define CELL_WIDTH (WIDTH/COLS)
#define CELL_HEIGHT (HEIGHT/ROWS)

#define BACKGROUND_COLOR 0xFF202020
#define FOREGROUND_COLOR 0xFF2020FF

void olivec_fill_circle(uint32_t *pixels, size_t pixels_width, size_t pixels_height, int cx, int cy, size_t r, uint32_t color)
{
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cx + r;
    for (int y = y1; y <= y2; ++y) {
        if (0 <= y && y < (int) pixels_height) {
            for (int x = x1; x <= x2; ++x) {
                if (0 <= x && x < (int) pixels_width) {
                    int dx = x - cx;
                    int dy = y - cy;
                    if (dx*dx + dy*dy <= r*r) {
                        pixels[y*pixels_width + x] = color;
                    }
                }
            }
        }
    }
}

static uint32_t pixels[HEIGHT*WIDTH];

bool checker_example(void)
{
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    //checkboard
    for (int y =0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            uint32_t color = BACKGROUND_COLOR;
            if ((x + y)%2 == 0) {
                color = 0xFF2020FF;
            }
            olivec_fill_rect(pixels, WIDTH, HEIGHT, x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, color);
        }
    }

    const char *file_path = "../checker.ppm";
    Errno err = olivec_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }

    return true;
}

bool circle_example(void)
{
    olivec_fill(pixels, WIDTH, HEIGHT, BACKGROUND_COLOR);

    olivec_fill_circle(pixels, WIDTH, HEIGHT, WIDTH/2, HEIGHT/2, 100, FOREGROUND_COLOR);

    const char *file_path = "../circle.ppm";
    Errno err = olivec_save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
    if (err) {
        fprintf(stderr, "ERROR: could not save file %s: %s\n", file_path, strerror(errno));
        return false;
    }

    return true;
}

int main(void)
{
    if (!checker_example()) return -1;
    if (!circle_example()) return -1;
    return 0;
}
