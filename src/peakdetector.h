#define T float

/*
 * Holds onto a ring buffer and looks for peaks when you push in new samples.
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

    // public
    int isBassPeak;
    int isTreblePeak;

} peakdetector_t;

peakdetector_t * new_peakdetector(int bufferLength);
void peakdetector_push(peakdetector_t *pd, T latest);
void del_peakdetector(peakdetector_t *pd);
