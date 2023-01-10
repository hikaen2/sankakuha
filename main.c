// gcc main.c -lm
#include <stdio.h>
#include <stdint.h>
#include <math.h>

size_t fwrite_int16(int16_t i, FILE* f)
{
    return fwrite(&i, sizeof(int16_t), 1, f);
}

size_t fwrite_int32(int32_t i, FILE* f)
{
    return fwrite(&i, sizeof(int32_t), 1, f);
}

double triangle(double x)
{
    x = fmod(x + 3.0, 4.0);
    if (0.0 <= x && x < 2.0) {
        return -x + 1.0;
    } else {
        return x - 3.0;
    }
}

double triangle2(double x)
{
    int t[] = { 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };
    x = fmod(x, 4.0);
    return t[(int)floor(x * 8.0)] / 16.0;
}

double circle(double x)
{
    x = fmod(x, 4.0);
    if (0.0 <= x && x < 2.0) {
        return sqrt(1.0 - (x - 1.0) * (x - 1.0));
    } else {
        return -sqrt(1.0 - (x - 3.0) * (x - 3.0));
    }
}

int main(void)
{
    FILE* file = fopen("out.wav", "w");

    for (int i = 0; i < 100; i++) {
        fwrite("\0", 1, 1, file);
    }

    // 440hz
    // １秒間に440周?  880π        880π/48000sample
    // for (int i = 0; i < 48000 * 10; i++) {
    //     double f = sin((440.0 / 48000.0) * 2 * M_PI * i);
    //     int16_t i = (int16_t)round(f * 10000.0);
    //     fwrite_int16(i, file);
    // }

    // for (int i = 0; i < 48000 * 10; i++) {
    //     double f = triangle2((440.0 / 48000.0) * 4.0 * i);
    //     int16_t i = (int16_t)round(f * 10000.0);
    //     fwrite_int16(i, file);
    // }

    for (int i = 0; i < 48000 * 10; i++) {
        double f = triangle2((32.7 / 48000.0) * 4.0 * i);
        int16_t i = (int16_t)round(f * 10000.0);
        fwrite_int16(i, file);
    }

    int32_t size = ftell(file);
    rewind(file);
    fwrite("RIFF", 4, 1, file);
    fwrite_int32(size - 8, file);
    fwrite("WAVE", 4, 1, file);
    fwrite("fmt ", 4, 1, file);
    fwrite_int32(16, file); // format size
    fwrite_int16(1, file); // data format
    fwrite_int16(1, file); // nChannels
    fwrite_int32(48000, file); // sampling rate
    fwrite_int32(48000 * 2, file); // byes / sec
    fwrite_int16(2, file); // block境界
    fwrite_int16(16, file); // nBits
    fwrite("data", 4, 1, file);
    fwrite_int32(size - 44, file); // 4 + 4 + 4 + 4 + 4 +  2 + 2 + 4 + 4 +  2 + 2 + 4
    fclose(file);
    return 0;
}
