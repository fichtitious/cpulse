#include <math.h>
#include <stdlib.h>
#include "beatdetector.h"

const T FILTER_CONSTANT = 0.6;
const T DECAY_CONSTANT = 0.94;
const T PEAK_CONSTANT = 2.4;

/*
 * Returns a pointer to a new beatdetector.
 */
beatdetector_t * new_beatdetector(int bufferLength) {

    beatdetector_t *bd = (beatdetector_t *) malloc(sizeof(beatdetector_t));

    bd->_samples = malloc(sizeof(T) * bufferLength);
    bd->_bassFiltered = malloc(sizeof(T) * bufferLength);
    bd->_trebleFiltered = malloc(sizeof(T) * bufferLength);
    bd->_bufferLength = bufferLength;
    bd->_pushIdx = bd->_bassPeak = bd->_treblePeak
                 = bd->_lastBassWasPeak = bd->_lastTrebleWasPeak
                 = bd->isBassBeat = bd->isTrebleBeat = 0;

    int i;
    for (i = 0; i < bd->_bufferLength; i++) {
        bd->_samples[i] = bd->_bassFiltered[i] = bd->_trebleFiltered[i] = 0;
    }

    return bd;

}

/*
 * Takes in a new sample, adds it to the beatdetector's ring buffer, and ubdates
 * the beatdetector's state variables: isBassBeat and isTrebleBeat.
 */
void beatdetector_push(beatdetector_t *bd, T sample) {

    // push the latest sample into the ring buffer
    // and cycle the ring pointer if necessary
    bd->_samples[bd->_pushIdx] = sample;
    if ( ++bd->_pushIdx == bd->_bufferLength ) {
        bd->_pushIdx = 0;
    }

    // do low- and high-pass filtering
    bd->_bassFiltered[0] = bd->_trebleFiltered[0] = bd->_samples[bd->_pushIdx];
    int i, j;
    for (i = 1; i < bd->_bufferLength; i++) {
        j = i + bd->_pushIdx; // start at pushIdx and do a full loop around the ring buffer
        if (j >= bd->_bufferLength) {
            j -= bd->_bufferLength;
        }
        bd->_bassFiltered[i] = bd->_bassFiltered[i-1] + FILTER_CONSTANT * (bd->_samples[j] - bd->_bassFiltered[i-1]);
        bd->_trebleFiltered[i] = FILTER_CONSTANT * (bd->_trebleFiltered[i-1] + bd->_samples[j] - bd->_samples[j-1]);
    }

    // latest filtered bass and treble values
    T bass = bd->_bassFiltered[bd->_bufferLength - 1];
    T treble = bd->_trebleFiltered[bd->_bufferLength - 1];

    int isBassPeak, isTreblePeak;

    // check whether latest values are peaks
    bd->_bassPeak *= DECAY_CONSTANT;
    if (bass > bd->_bassPeak * PEAK_CONSTANT) {
        isBassPeak = 1;
        bd->_bassPeak = bass;
    } else {
        isBassPeak = 0;
    }

    bd->_treblePeak *= DECAY_CONSTANT;
    if (treble > bd->_treblePeak * PEAK_CONSTANT) {
        isTreblePeak = 1;
        bd->_treblePeak = treble;
    } else {
        isTreblePeak = 0;
    }

    // detect a beat right at a falling edge
    bd->isBassBeat = bd->_lastBassWasPeak && !isBassPeak;
    bd->isTrebleBeat = bd->_lastTrebleWasPeak && !isTreblePeak;

    bd->_lastBassWasPeak = isBassPeak;
    bd->_lastTrebleWasPeak = isTreblePeak;

}

/*
 * Cleans up.
 */
void del_beatdetector(beatdetector_t *bd) {

    free(bd->_samples);
    free(bd->_bassFiltered);
    free(bd->_trebleFiltered);
    free(bd);

}
