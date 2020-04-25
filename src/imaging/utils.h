#ifndef UTILS_H
#define UTILS_H
// swaps
void swap2d(float **input, short y_in, short x_in, short y_out, short x_out);
void swap1d(float *input, short loc_in, short loc_out);

// spectrum visualizations
void shift(float **input, short size);

void logAction(short actionToLog);

#endif