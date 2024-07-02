#ifndef IOPS_TRACKER_H
#define IOPS_TRACKER_H


#include "block/aio.h"
#include "block/block-common.h"
#include "block/block-global-state.h"
#include "block/snapshot.h"
#include "qemu/iov.h"
#include "qemu/rcu.h"
#include "qemu/stats64.h"

/*
 * IOPS Tracker
 *
 * @operations: Number of I/O operations
 * @start_time_ns: Start time of the IOPS tracking in nanoseconds
 */
typedef struct IOPSTracker{
    int64_t operations;      // Number of I/O operations
    int64_t start_time_ns;   // Start time in nanoseconds
} IOPSTracker;


/* Initialize the IOPS tracker for each device */
IOPSTracker* iops_tracker_new(void);

/* Initialize the IOPS tracker for each device */
void iops_tracker_init(IOPSTracker *tracker);

/* Update the tracker whenever each ops is performed */
void iops_tracker_update(IOPSTracker *tracker, int64_t operations, QemuMutex*);

/* Calculate the IO per second and re-initialize the tracker to 0 */
double iops_tracker_get_iops(IOPSTracker *tracker, QemuMutex*);

#endif // IOPS_TRACKER_H