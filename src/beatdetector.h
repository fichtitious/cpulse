#define T float

/*
 * Holds onto a ring buffer and looks for beats when you push in new samples.
 */
typedef struct {

    // private
    T *_samples;
    T *_bassFiltered;
    T *_trebleFiltered;
    int _bufferLength;
    int _pushIdx;
    T _bassPeak;
    T _treblePeak;
    int _lastBassWasPeak;
    int _lastTrebleWasPeak;

    // public
    int isBassBeat;
    int isTrebleBeat;

} beatdetector_t;

beatdetector_t * new_beatdetector(int bufferLength);
void beatdetector_push(beatdetector_t *bd, T latest);
void del_beatdetector(beatdetector_t *bd);
