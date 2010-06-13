#include <math.h>
#include <stdlib.h>
#include "peakdetector.h"

/*
 * Returns a pointer to a new peakdetector.
 */
peakdetector_t * new_peakdetector(int bufferLength) {

    peakdetector_t *pd = (peakdetector_t *) malloc(sizeof(peakdetector_t));
    pd->_buffer = malloc(sizeof(T) * bufferLength);
    pd->_bufferLength = bufferLength;
    pd->_pushIdx = 0;
    pd->_peakness = 0;

    pd->isPeak = 0;
    pd->isIncreasing = 0;

    int i;
    for (i = 0; i < pd->_bufferLength; i++) {
        pd->_buffer[i] = 0;
    }

    return pd;

}

/*
 * Takes in a new value, adds it to the peakdetector's ring buffer,
 * and updates the peakdetector's state variables (_peakness, isPeak,
 * and isIncreasing) to reflect the peakness of the new value.
 */
void peakdetector_push(peakdetector_t *pd, T latest) {

    int i;

    // mean squares
    T localAverage = 0;
    for (i = 0; i < pd->_bufferLength; i++) {
        localAverage += pow(pd->_buffer[i], 2);
    }
    localAverage /= pd->_bufferLength;

    // mean squared differences from the local average
    T localVariance = 0;
    for (i = 0; i < pd->_bufferLength; i++) {
        localVariance += pow(localAverage - pd->_buffer[i], 2);
    }
    localVariance /= pd->_bufferLength;

    // temp save last peakness
    T lastPeakness = pd->_peakness;

    // calculate new peakness
    T valueToBeat = localAverage * (-0.0002 * localVariance + 128.0);
    pd->_peakness = latest - valueToBeat;

    // save values for the derived state variables
    pd->isPeak = pd->_peakness > 0;
    pd->isIncreasing = pd->_peakness > lastPeakness;

    // push the latest value into the ring buffer
    // and cycle the ring pointer if necessary
    pd->_buffer[pd->_pushIdx] = latest;
    if ( ++pd->_pushIdx == pd->_bufferLength ) {
        pd->_pushIdx = 0;
    }

}

/*
 * Cleans up.
 */
void del_peakdetector(peakdetector_t *pd) {

    free(pd->_buffer);
    free(pd);

}
