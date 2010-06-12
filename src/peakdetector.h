/*
 * Holds onto a ring buffer and a flag indicating whether
 * the latest value is a peak.
 */
typedef struct {
    int *buffer;
    int bufferLength;
    int pushIdx;
    int isPeak;
} peakdetector_t;

peakdetector_t * new_peakdetector(int bufferLength);
int * peakdetector_peak(peakdetector_t *pd, int latest);
void del_peakdetector(peakdetector_t *pd);
