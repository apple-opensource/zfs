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

#ifndef	_SYS_TASKQ_H
#define	_SYS_TASKQ_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <sys/types.h>
#include <sys/thread.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define	TASKQ_NAMELEN	31

typedef struct taskq taskq_t;
typedef uintptr_t taskqid_t;
typedef void (task_func_t)(void *);

/*
 * Public flags for taskq_create(): bit range 0-15
 */
#define	TASKQ_PREPOPULATE	0x0001	/* Prepopulate with threads and data */
#define	TASKQ_CPR_SAFE		0x0002	/* Use CPR safe protocol */

/*
 * Flags for taskq_dispatch. TQ_SLEEP/TQ_NOSLEEP should be same as
 * KM_SLEEP/KM_NOSLEEP.
 */
#define	TQ_SLEEP	0x00	/* Can block for memory */
#define	TQ_NOSLEEP	0x01	/* cannot block for memory; may fail */
#define	TQ_NOALLOC	0x04	/* cannot allocate memory; may fail */

#ifdef _KERNEL

extern void	taskq_init(void);
extern void	taskq_fini(void);


extern taskq_t	*taskq_create(const char *, int, pri_t, int, int, uint_t);
extern taskqid_t taskq_dispatch(taskq_t *, task_func_t, void *, uint_t);
extern void	taskq_destroy(taskq_t *);
extern void	taskq_wait(taskq_t *);
extern void	taskq_suspend(taskq_t *);
extern int	taskq_suspended(taskq_t *);
extern void	taskq_resume(taskq_t *);
extern int	taskq_member(taskq_t *, kthread_t *);

#endif	/* _KERNEL */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_TASKQ_H */
