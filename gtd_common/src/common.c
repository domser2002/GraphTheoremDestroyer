#include "common.h"

void *gtd_malloc(size_t size)
{
    void *ptr = malloc(size);
    GTD_LOG("malloc %p",ptr);
    return ptr;
}

void *gtd_calloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb,size);
    GTD_LOG("calloc %p",ptr);
    return ptr;
}

void *gtd_realloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    GTD_LOG("realloc %p",ptr);
    return ptr;
}

void gtd_free(void *ptr)
{
    GTD_LOG("free %p",ptr);
    return free(ptr);
}

void printProgressBar(int progress, int total) {
    int barWidth = 70; // Width of the progress bar
    float percentage = (float)progress / total;

    printf("\033[0G["); // Move cursor to the beginning of the line and print the start of the bar
    int pos = barWidth * percentage;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) 
            printf("=");
        else 
            printf(" ");
    }
    printf("] %.2f%%", (double)percentage * 100); // Display percentage
    fflush(stdout); // Flush the output buffer to update the display
}