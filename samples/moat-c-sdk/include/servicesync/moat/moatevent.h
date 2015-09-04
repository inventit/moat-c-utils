/*
 * LEGAL NOTICE
 *
 * Copyright (C) 2013 InventIt Inc. All rights reserved.
 *
 * This source code, product and/or document is protected under licenses 
 * restricting its use, copying, distribution, and decompilation.
 * No part of this source code, product or document may be reproduced in
 * any form by any means without prior written authorization of InventIt Inc.
 * and its licensors, if any.
 *
 * InventIt Inc.
 * 9F KOJIMACHI CP BUILDING
 * 4-4-7 Kojimachi, Chiyoda-ku, Tokyo 102-0083
 * JAPAN
 * http://www.yourinventit.com/
 */


#ifndef MOATEVENT_H_
#define MOATEVENT_H_


SSE_BEGIN_C_DECLS

enum moat_event_id_ {
  MOAT_EVENT_IO_READY = SSE_EVENT_MIN_ID,
  MOAT_EVENT_APP_MIN = SSE_USER_EVENT_MIN_ID + 1000,
  MOAT_EVENT_IDs
};

typedef struct SSEEventService_ MoatEventService;
typedef sse_int (*MoatListenerProc)(sse_int in_event_id, sse_pointer in_data, sse_uint in_len, sse_pointer in_user_data);
typedef void (*MoatFreeEventDataProc)(sse_pointer data);

/* Event Service */
MoatEventService * moat_event_service_get_instance(void);
sse_int moat_event_service_subscribe(MoatEventService *self, sse_int in_event_id, MoatListenerProc in_proc, sse_pointer in_user_data);
sse_int moat_event_service_unsubscribe(MoatEventService *self, sse_int in_event_id, MoatListenerProc in_proc);
sse_int moat_event_service_publish(MoatEventService *self, sse_int in_event_id, sse_pointer in_data, sse_uint in_len, MoatFreeEventDataProc in_free_proc, sse_bool in_async);

/* IO Watcher */
enum moat_io_flags_ {
  MOAT_IO_FLAG_ERROR = (1 << 1),
  MOAT_IO_FLAG_READ = (1 << 2),
  MOAT_IO_FLAG_WRITE = (1 << 3),
  MOAT_IO_FLAGs
};
typedef struct MoatIOWatcher_ MoatIOWatcher;
typedef void (*MoatIOEventProc)(MoatIOWatcher *in_watcher, sse_pointer in_user_data, sse_int in_desc, sse_int in_event_flags);
MoatIOWatcher * moat_io_watcher_new(sse_int in_desc, MoatIOEventProc in_handler, sse_pointer in_user_data, sse_int in_event_flags);
void moat_io_watcher_free(MoatIOWatcher *self);
sse_int moat_io_watcher_start(MoatIOWatcher *self);
void moat_io_watcher_stop(MoatIOWatcher *self);
sse_int moat_io_watcher_get_descriptor(MoatIOWatcher *self);
void moat_io_watcher_set_descriptor(MoatIOWatcher *self, sse_int in_desc, sse_int in_event_flags);
void moat_io_watcher_set_handler(MoatIOWatcher *self, MoatIOEventProc in_handler, sse_pointer in_user_data);
sse_pointer moat_io_watcher_get_user_data(MoatIOWatcher *self);
sse_bool moat_io_watcher_is_active(MoatIOWatcher *self);

/* Idle */
typedef struct MoatIdle_ MoatIdle;
typedef void (*MoatIdleProc)(MoatIdle *in_idle, sse_pointer in_user_data);
MoatIdle * moat_idle_new(MoatIdleProc in_proc, sse_pointer in_user_data);
void moat_idle_free(MoatIdle *self);
sse_int moat_idle_start(MoatIdle *self);
void moat_idle_stop(MoatIdle *self);
sse_bool moat_idle_is_active(MoatIdle *self);
void moat_idle_set_handler(MoatIdle *self, MoatIdleProc in_proc, sse_pointer in_user_data);
sse_pointer moat_idle_get_user_data(MoatIdle *self);

/* Periodic */
typedef struct MoatPeriodic_ MoatPeriodic;
typedef void (*MoatPeriodicProc)(MoatPeriodic *in_periodic, sse_pointer in_user_data);
MoatPeriodic * moat_periodic_new(MoatPeriodicProc in_proc, sse_pointer in_user_data, sse_uint in_period_sec);
void moat_periodic_free(MoatPeriodic *self);
sse_int moat_periodic_start(MoatPeriodic *self);
void moat_periodic_stop(MoatPeriodic *self);
sse_bool moat_periodic_is_active(MoatPeriodic *self);
sse_uint moat_periodic_get_period(MoatPeriodic *self);
void moat_periodic_set_period(MoatPeriodic *self, sse_uint in_period_sec);
void moat_periodic_set_handler(MoatPeriodic *self, MoatPeriodicProc in_proc, sse_pointer in_user_data);
sse_pointer moat_periodic_get_user_data(MoatPeriodic *self);

SSE_END_C_DECLS

#endif /* MOATEVENT_H_ */
