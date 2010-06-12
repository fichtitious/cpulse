#define T float

/*
 * Holds onto a ring buffer and looks for peakness when you push in new values.
 */
typedef struct {

    // private
    T *_buffer;
    int _bufferLength;
    int _pushIdx;
    T _peakness;

    // public
    int isPeak;
    int isIncreasing;

} peakdetector_t;

peakdetector_t * new_peakdetector(int bufferLength);
void peakdetector_push(peakdetector_t *pd, T latest);
void del_peakdetector(peakdetector_t *pd);
