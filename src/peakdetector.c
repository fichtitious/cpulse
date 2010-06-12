#include <math.h>
#include <stdlib.h>
#include "peakdetector.h"

/*
 * Gets a pointer to a new peakdetector.
 */
peakdetector_t * new_peakdetector(int bufferLength) {

    peakdetector_t *pd = (peakdetector_t *) malloc(sizeof(peakdetector_t));
    pd->buffer = malloc(sizeof(int) * bufferLength);
    pd->bufferLength = bufferLength;
    pd->pushIdx = 0;
    pd->isPeak = 0;

    return pd;

}

/*
 * Takes in a new value, adds it to the ring buffer,
 * and determines whether this value is a peak.
 */
int * peakdetector_peak(peakdetector_t *pd, int latest) {

    int i;

    // mean squares
    float localAverage = 0.0;
    for (i = 0; i < pd->bufferLength; i++ ) {
        localAverage += pow(pd->buffer[i], 2);
    }
    localAverage /= pd->bufferLength;

    // mean squared differences from the local average
    float localVariance = 0.0;
    for (i = 0; i < pd->bufferLength; i++ ) {
        localVariance += pow(localAverage - pd->buffer[i], 2);
    }
    localVariance /= pd->bufferLength;

    // latest value is a peak if it exceeds the local average, factoring in local variance
    float levelToBeat = localAverage * (-0.0025714 * localVariance + 1.5142857);
    pd->isPeak = latest > levelToBeat;

    // push the latest value into the ring buffer
    pd->buffer[pd->pushIdx] = latest;
    if ( ++pd->pushIdx == pd->bufferLength ) {
        pd->pushIdx = 0;
    }

    return &pd->isPeak;

}

/*
 * Cleans up.
 */
void del_peakdetector(peakdetector_t *pd) {

    free(pd->buffer);
    free(pd);

}
