// public fucntions

#include "image.h"
#include "filter.h"

// basic fft
void fft(struct Image input, struct CImage output);
void ifft(struct CImage input, struct Image output);

// cooley tukey fft
void ctftt(struct CImage input);
void ctifft(struct CImage input);

// filtering
void lowPass(struct CImage input, struct Kernel filter);
