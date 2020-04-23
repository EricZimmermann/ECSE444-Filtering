#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

// inplace swap 2d element - O(1) space 
void swap2d(float **input, y_in, x_in, y_out, x_out){
    temp = input[y_in][x_in];
    input[y_in][x_in] = input[y_out][x_out];
    input[y_out][x_out] = temp;
}

// inplace swap 1d element - O(1) space
void swap1d(float *input, loc_in, loc_out){
    temp = input[loc_in];
    input[loc_in] = input[loc_out];
    input[loc_out] = temp;
}
             
// Shift a spectra to be zero fq centered
// Unshift a spectra to be non-zero shifted centered
// Diagonalized quadrant swap
void shift(float **input, int size){    
    uint16_t quad = size >> 1;
    for(uint16_t y = 0; y < quad; ++y){
        for(uint16_t x = 0; x < quad; ++x){
            swap2d(input, y, x, y + quad, x + quad);
            swap2d(input, y + quad, x, y, x + quad);
        }        
    }
}

// Used as helper for logAction
int logDirectoryExists()
{
    struct stat stats;

    stat("./logs/", &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode)) return 1;

    return 0;
}

// Called by the method of each action that requires logging
void logAction(short actionToLog) {

    if (!logDirectoryExists()) {
        system("mkdir ./logs/");
    }

    char logFileName[] = "logs/logs.txt";
    char *logText = "";

    switch(actionToLog) {
        case 1:
            logText = "Successfully computed FFT with naive DFT.\n";
            break;
        case 2:
            logText = "Successfully computed IFFT with naive DFT.\n";
            break;
        case 3:
            logText = "Successfully computed FFT with Cooley-Tukey method.\n";
            break;
        case 4:
            logText = "Successfully computed IFFT with Cooley-Tukey method.\n";
            break;
        default:
            break;
    }

    if (strcmp("", logText) != 0) {
        FILE *fp = fopen(logFileName, "a");
        fwrite(logText, 1, strlen(logText), fp);
        fclose(fp);
    }

}