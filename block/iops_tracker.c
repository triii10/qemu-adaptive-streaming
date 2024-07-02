#include "block/iops_tracker.h"

IOPSTracker *iops_tracker_new(void)
{
    IOPSTracker *tracker = g_new0(IOPSTracker, 1);
    iops_tracker_init(tracker);
    return tracker;
}

/* Initialize the IOPS tracker for each device */
void iops_tracker_init(IOPSTracker *tracker) {
    tracker->operations = 0;
    tracker->start_time_ns = qemu_clock_get_ns(QEMU_CLOCK_REALTIME);
    qemu_log("iops_tracker_init HIT\n");
}

/* Update the tracker whenever each ops is performed */
void iops_tracker_update(IOPSTracker *tracker, int64_t operations, QemuMutex *mutex) 
{
    qemu_mutex_lock(mutex);
    tracker->operations += operations;
    qemu_mutex_unlock(mutex);
}

/* Calculate the IO per second and re-initialize the tracker to 0 */
double iops_tracker_get_iops(IOPSTracker *tracker, QemuMutex *mutex) 
{
    qemu_mutex_lock(mutex);
    int64_t now_ns = qemu_clock_get_ns(QEMU_CLOCK_REALTIME);
    int64_t elapsed_ns = now_ns - tracker->start_time_ns;
    double elapsed_sec = (double)elapsed_ns / 1e9;  // Convert nanoseconds to seconds
    double iops = tracker->operations / elapsed_sec;
    qemu_mutex_unlock(mutex);
    iops_tracker_init(tracker);
    return iops;
}