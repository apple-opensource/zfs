/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Portions Copyright 2007 Apple Inc. All rights reserved.
 * Use is subject to license terms.
 */


#ifndef	_SYS_TASKQ_IMPL_H
#define	_SYS_TASKQ_IMPL_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <sys/taskq.h>
#include <sys/vmem.h>
#include <sys/kstat.h>

#ifdef	__cplusplus
extern "C" {
#endif


typedef struct taskq_ent {
	struct taskq_ent	*tqent_next;
	struct taskq_ent	*tqent_prev;
	task_func_t		*tqent_func;
	void			*tqent_arg;
	kthread_t		*tqent_thread;
	kcondvar_t		tqent_cv;
} taskq_ent_t;


/*
 * Bucket flags.
 */
#define	TQBUCKET_CLOSE		0x01
#define	TQBUCKET_SUSPEND	0x02

/*
 * taskq implementation flags: bit range 16-31
 */
#define	TASKQ_ACTIVE		0x00010000
#define	TASKQ_SUSPENDED		0x00020000
#define	TASKQ_NOINSTANCE	0x00040000

struct taskq {
	char		tq_name[TASKQ_NAMELEN + 1];
	kmutex_t	tq_lock;
	krwlock_t	tq_threadlock;
	kcondvar_t	tq_dispatch_cv;
	kcondvar_t	tq_wait_cv;
	uint_t		tq_flags;
	int		tq_active;
	int		tq_nthreads;
	int		tq_nalloc;
	int		tq_minalloc;
	int		tq_maxalloc;
	taskq_ent_t	*tq_freelist;
	taskq_ent_t	tq_task;
	int		tq_maxsize;
	pri_t		tq_pri;		/* Scheduling priority	    */
	union {
		kthread_t *_tq_thread;
		kthread_t **_tq_threadlist;
	}		tq_thr;
	/*
	 * Statistics.
	 */
	hrtime_t	tq_totaltime;	/* Time spent processing tasks */
	int		tq_tasks;	/* Total # of tasks posted */
	int		tq_executed;	/* Total # of tasks executed */
	int		tq_maxtasks;	/* Max number of tasks in the queue */
};

#define	tq_thread tq_thr._tq_thread
#define	tq_threadlist tq_thr._tq_threadlist

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_TASKQ_IMPL_H */