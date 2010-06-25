#include <math.h>
#include <stdlib.h>
#include "peakdetector.h"

const T HIGH_PASS_FILTER_CONSTANT = 0.6;
const T LOW_PASS_FILTER_CONSTANT = 0.6;
const T DECAY_CONSTANT = 0.94;
const T PEAK_CONSTANT = 2.0;

/*
 * Returns a pointer to a new peakdetector.
 */
peakdetector_t * new_peakdetector(int bufferLength) {

    peakdetector_t *pd = (peakdetector_t *) malloc(sizeof(peakdetector_t));

    pd->_samples = malloc(sizeof(T) * bufferLength);
    pd->_bassFiltered = malloc(sizeof(T) * bufferLength);
    pd->_trebleFiltered = malloc(sizeof(T) * bufferLength);
    pd->_bufferLength = bufferLength;
    pd->_pushIdx = pd->_bassPeak = pd->_treblePeak = pd->isBassPeak = pd->isTreblePeak = 0;

    int i;
    for (i = 0; i < pd->_bufferLength; i++) {
        pd->_samples[i] = pd->_bassFiltered[i] = pd->_trebleFiltered[i] = 0;
    }

    return pd;

}

/*
 * Takes in a new sample, adds it to the peakdetector's ring buffer, and updates
 * the peakdetector's state variables: isBassPeak and isTreblePeak.
 */
void peakdetector_push(peakdetector_t *pd, T sample) {

    // push the latest sample into the ring buffer
    // and cycle the ring pointer if necessary
    pd->_samples[pd->_pushIdx] = sample;
    if ( ++pd->_pushIdx == pd->_bufferLength ) {
        pd->_pushIdx = 0;
    }

    // do low- and high-pass filtering
    pd->_bassFiltered[0] = pd->_trebleFiltered[0] = pd->_samples[pd->_pushIdx];
    int i, j;
    for (i = 1; i < pd->_bufferLength; i++) {
        j = i + pd->_pushIdx; // start at pushIdx and do a full loop around the ring buffer
        if (j >= pd->_bufferLength) {
            j -= pd->_bufferLength;
        }
        pd->_bassFiltered[i] = pd->_bassFiltered[i-1] + LOW_PASS_FILTER_CONSTANT * (pd->_samples[j] - pd->_bassFiltered[i-1]);
        pd->_trebleFiltered[i] = HIGH_PASS_FILTER_CONSTANT * (pd->_trebleFiltered[i-1] + pd->_samples[j] - pd->_samples[j-1]);
    }

    T bass = pd->_bassFiltered[pd->_bufferLength - 1];
    T treble = pd->_trebleFiltered[pd->_bufferLength - 1];

    pd->_bassPeak *= DECAY_CONSTANT;
    if (bass > pd->_bassPeak * PEAK_CONSTANT) {
        pd->isBassPeak = 1;
        pd->_bassPeak = bass;
    } else {
        pd->isBassPeak = 0;
    }

    pd->_treblePeak *= DECAY_CONSTANT;
    if (treble > pd->_treblePeak * PEAK_CONSTANT) {
        pd->isTreblePeak = 1;
        pd->_treblePeak = treble;
    } else {
        pd->isTreblePeak = 0;
    }

}

/*
 * Cleans up.
 */
void del_peakdetector(peakdetector_t *pd) {

    free(pd->_samples);
    free(pd->_bassFiltered);
    free(pd->_trebleFiltered);
    free(pd);

}
