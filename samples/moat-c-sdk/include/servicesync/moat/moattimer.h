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


#ifndef MOATTIMER_H_
#define MOATTIMER_H_


SSE_BEGIN_C_DECLS

typedef struct MoatTimer_ MoatTimer;
typedef sse_bool (*MoatTimerProc)(sse_int in_timer_id, sse_pointer in_user_data);

MoatTimer * moat_timer_new(void);
void moat_timer_free(MoatTimer *self);
sse_int moat_timer_set(MoatTimer *self, sse_uint in_interval_sec, MoatTimerProc in_proc, sse_pointer in_user_data);
void moat_timer_cancel(MoatTimer *self, sse_int in_id);

SSE_END_C_DECLS

#endif /* MOATTIMER_H_ */
