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

pa_simple *_pulseAudio;
int _pulseAudioError;
int _sampleSize;

aubio_beattracking_t *_aubioTracker;
fvec_t *_aubioInput;
fvec_t *_aubioOutput;

void _getDefaultSink(char *deviceName) {

    FILE* pipe = popen("pacmd list-sinks | grep state", "r");

    char buffer[128];
    int idx = 0;
    int deviceIdx = -1;
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL) {
            if (strstr(buffer, "RUNNING")) {
                deviceIdx = idx;
                break;
            } else {
                idx++;
            }
        }
    }

    pclose(pipe);

    if (deviceIdx == -1) {
        printf("cpulse could not find default sink device (ran pacmd list-sinks)\n");
        exit(1);
    } else {
        sprintf(deviceName, "%i", deviceIdx);
        printf("found default sink device '%s'... ", &deviceName[0]);
    }

}

void cpulse_start() {

    _aubioTracker = new_aubio_beattracking(NUM_AUDIO_FRAMES, NUM_CHANNELS);
    _aubioInput = new_fvec(NUM_AUDIO_FRAMES, NUM_CHANNELS);
    _aubioOutput =  new_fvec(NUM_AUDIO_FRAMES / 4, NUM_CHANNELS);
    _sampleSize = sizeof(*fvec_get_data(_aubioInput)) * NUM_AUDIO_FRAMES / 2;

    const pa_sample_spec samplingType = {PA_SAMPLE_FLOAT32LE, SAMPLE_RATE, NUM_CHANNELS};

    char deviceName[1];
    _getDefaultSink(deviceName);
    const char *hwSource = &deviceName[0];
    printf("cpulse connecting to pulseaudio... ");

    if (!(_pulseAudio = pa_simple_new( NULL, "cpulse", PA_STREAM_RECORD, hwSource, "cpulse",
                                      &samplingType, NULL, NULL, &_pulseAudioError ))) {
        printf("cpulse couldn't connect to pulseaudio: %s\n", pa_strerror(_pulseAudioError));
        exit(1);      
    }

    printf("connected!\n");

}

void cpulse_stop() {

    pa_simple_free(_pulseAudio);
    del_fvec(_aubioInput);
    del_fvec(_aubioOutput);
    del_aubio_beattracking(_aubioTracker);
    printf("cpulse successfully closed its pulseaudio connection\n");

}

float * cpulse_pulse() {

    if (pa_simple_read( _pulseAudio, *fvec_get_data(_aubioInput), _sampleSize, &_pulseAudioError ) < 0) {
        printf("cpulse error reading from pulseaudio: %s\n", pa_strerror(_pulseAudioError));
    } else {
        aubio_beattracking_do( _aubioTracker, _aubioInput, _aubioOutput );
    }

    return *fvec_get_data(_aubioOutput);

}
