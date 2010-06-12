#define T float

/*
 * Holds onto a ring buffer and a flag indicating whether
 * the latest value is a peak.
 */
typedef struct {
    T *buffer;
    int bufferLength;
    int pushIdx;
    T peakness;
    double normalizer;
} peakdetector_t;

peakdetector_t * new_peakdetector(int bufferLength);
float * peakdetector_peak(peakdetector_t *pd, T latest);
void del_peakdetector(peakdetector_t *pd);
