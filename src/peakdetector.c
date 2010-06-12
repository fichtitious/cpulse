#include <math.h>
#include <stdlib.h>
#include "peakdetector.h"

/*
 * Gets a pointer to a new peakdetector.
 */
peakdetector_t * new_peakdetector(int bufferLength) {

    peakdetector_t *pd = (peakdetector_t *) malloc(sizeof(peakdetector_t));
    pd->buffer = malloc(sizeof(T) * bufferLength);
    pd->bufferLength = bufferLength;
    pd->pushIdx = 0;
    pd->peakness = 0;
    pd->normalizer = 0;

    int i;
    for (i = 0; i < pd->bufferLength; i++) {
        pd->buffer[i] = 0;
    }

    return pd;

}

/*
 * Takes in a new value, adds it to the ring buffer,
 * and returns a pointer to a float between -1 and 1
 * indicating how much of a peak this value is.
 */
float * peakdetector_peak(peakdetector_t *pd, T latest) {

    int i;

    // mean squares
    T localAverage = 0;
    for (i = 0; i < pd->bufferLength; i++) {
        localAverage += pow(pd->buffer[i], 2);
    }
    localAverage /= pd->bufferLength;

    // mean squared differences from the local average
    T localVariance = 0;
    for (i = 0; i < pd->bufferLength; i++) {
        localVariance += pow(localAverage - pd->buffer[i], 2);
    }
    localVariance /= pd->bufferLength;

    // calculate a prenormalized "peakness"
    double valueToBeat = localAverage * (-0.0025714 * localVariance + 150);
    double prenormalizedPeakness = latest - valueToBeat;

    // increase the normalizer, if necessary
    double absPrenormalizedPeakness = fabs(prenormalizedPeakness);
    if (absPrenormalizedPeakness > pd->normalizer) {
        pd->normalizer = absPrenormalizedPeakness;
    }

    // normalize the peakness to between -1 and 1
    pd->peakness = pd->normalizer == 0 ? 0 : prenormalizedPeakness / pd->normalizer;

    // push the latest value into the ring buffer
    pd->buffer[pd->pushIdx] = latest;
    if ( ++pd->pushIdx == pd->bufferLength ) {
        pd->pushIdx = 0;
    }

    /*
    printf("avg: %f\n", localAverage);
    printf("var: %f\n", localVariance);
    printf("peakness: %f\n", pd->peakness);
    printf("peak?: %i\n\n", pd->peakness > 0);
    */

    return &pd->peakness;

}

/*
 * Cleans up.
 */
void del_peakdetector(peakdetector_t *pd) {

    free(pd->buffer);
    free(pd);

}
