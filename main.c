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

int main(void)
{
    FILE* file = fopen("out.wav", "w");

    fwrite("RIFF", 4, 1, file);
    fwrite_int32(0, file); // wip
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
    fwrite_int32(0, file);

    // 440hz
    // １秒間に440周?  880π        880π/48000sample
    for (int i = 0; i < 48000 * 10; i++) {
        double r = 880.0 * M_PI * (1.0 / 48000.0) * i;
        double f = sin(r);
        int16_t i = (int16_t)round(f * 10000.0);
        //printf("%d\n", i);
        fwrite_int16(i, file);
    }

    int32_t size = ftell(file);
    fseek(file, 4, SEEK_SET);
    fwrite_int32(size - 8, file);
    fseek(file, 4 + 4 + 4 + 2 + 2 + 4 + 4 + 2 + 2 + 4, SEEK_CUR);
    fwrite_int32(size - -126, file);

    fclose(file);    
    return 0;
}
