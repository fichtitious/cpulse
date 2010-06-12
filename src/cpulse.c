/*
  Finds your pulseaudio output device, gets a read-mode connection to it,
  to read all the audio data that's going out through your speakers, from
  whatever source, and pulls the data through the aubio beat tracker.

  cpulse_beats() gets the latest data from pulseaudio and returns a pointer
  to aubio's output, whose nature is not at all well documented, but best
  understood by reading aubio_beattracking_do() here:

    http://dev.aubio.org/browser/src/tempo/beattracking.c

  There is no threading here.  You simply call cpulse_beats() continuously
  to get continuous beat tracking.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pulse/error.h>
#include <pulse/simple.h>
#include <aubio/aubio.h>
#include <aubio/beattracking.h>

const unsigned int NUM_AUDIO_FRAMES = 1024;
const uint8_t NUM_CHANNELS = 2;
const uint32_t SAMPLE_RATE = 44100;

pa_simple *_pulseAudioClient;
int _pulseAudioError;
int _sampleSize;

aubio_beattracking_t *_aubioBeatTracker;
fvec_t *_aubioInput;
fvec_t *_aubioOutput;

// Finds the index of your running pulseaudio output device.
void _getRunningSink(char *sinkIndex) {

    FILE *pipe = popen("pacmd list-sinks | grep state", "r");

    char buffer[128];
    int i = 0;
    int deviceIdx = -1;
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            if (strstr(buffer, "RUNNING")) {
                deviceIdx = i;
                break;
            } else {
                i++;
            }
        }
    }

    pclose(pipe);

    if (deviceIdx == -1) {
        printf("cpulse could not find a running pulseaudio sink device (ran pacmd list-sinks)\n");
        exit(1);
    } else {
        sprintf(sinkIndex, "%i", deviceIdx);
    }

}

// Starts up cpulse.
void cpulse_start(void) {

    // find the running pulseaudio sink
    char sinkIndex[1];
    _getRunningSink(sinkIndex);
    const char *audioDevice = &sinkIndex[0];

    // make a pulseaudio connection
    printf("cpulse is connecting to pulseaudio sink %s... ", audioDevice);
    const pa_sample_spec paSampleSpec = { PA_SAMPLE_FLOAT32LE, SAMPLE_RATE, NUM_CHANNELS };
    if (!(_pulseAudioClient = pa_simple_new( NULL, "cpulse", PA_STREAM_RECORD, audioDevice, "cpulse",
                                             &paSampleSpec, NULL, NULL, &_pulseAudioError ))) {
        printf("cpulse couldn't connect to pulseaudio: %s\n", pa_strerror(_pulseAudioError));
        exit(1);
    } else {
        printf("connected.\n");
    }

    // initialize an aubio beat tracker
    _aubioBeatTracker = new_aubio_beattracking(NUM_AUDIO_FRAMES, NUM_CHANNELS);
    _aubioInput = new_fvec(NUM_AUDIO_FRAMES, NUM_CHANNELS);
    _aubioOutput =  new_fvec(NUM_AUDIO_FRAMES / 4, NUM_CHANNELS);
    _sampleSize = sizeof(*fvec_get_data(_aubioInput)) * NUM_AUDIO_FRAMES / 2;

}

// Reads the latest audio data from pulseaudio, pulls it through
// the aubio beat tracker, and returns a pointer to the output.
float * cpulse_beats(void) {

    if (pa_simple_read( _pulseAudioClient, *fvec_get_data(_aubioInput), _sampleSize, &_pulseAudioError ) < 0) {
        printf("cpulse error reading from pulseaudio: %s\n", pa_strerror(_pulseAudioError));
    } else {
        aubio_beattracking_do( _aubioBeatTracker, _aubioInput, _aubioOutput );
    }

    return *fvec_get_data(_aubioOutput);

}

// Cleans up.
void cpulse_stop(void) {

    pa_simple_free(_pulseAudioClient);
    del_fvec(_aubioInput);
    del_fvec(_aubioOutput);
    del_aubio_beattracking(_aubioBeatTracker);
    printf("cpulse successfully closed its pulseaudio connection.\n");

}
