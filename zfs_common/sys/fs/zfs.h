/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
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
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Portions Copyright 2007 Apple Inc. All rights reserved.
 * Use is subject to license terms.
 */


#ifndef	_SYS_FS_ZFS_H
#define	_SYS_FS_ZFS_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <sys/types.h>
#if __APPLE__
#include <sys/ioccom.h>
#endif

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Types and constants shared between userland and the kernel.
 */

/*
 * Each dataset can be one of the following types.  These constants can be
 * combined into masks that can be passed to various functions.
 */
typedef enum {
	ZFS_TYPE_FILESYSTEM	= 0x1,
	ZFS_TYPE_SNAPSHOT	= 0x2,
	ZFS_TYPE_VOLUME		= 0x4,
	ZFS_TYPE_POOL		= 0x8
} zfs_type_t;

#define	ZFS_TYPE_ANY	\
	(ZFS_TYPE_FILESYSTEM | ZFS_TYPE_VOLUME | ZFS_TYPE_SNAPSHOT)

/*
 * Properties are identified by these constants and must be added to the
 * end of this list to ensure that external conumsers are not affected
 * by the change. The property list also determines how 'zfs get' will
 * display them.  If you make any changes to this list, be sure to update
 * the property table in usr/src/common/zfs/zfs_prop.c.
 */
typedef enum {
	ZFS_PROP_CONT = -2,
	ZFS_PROP_INVAL = -1,
	ZFS_PROP_TYPE,
	ZFS_PROP_CREATION,
	ZFS_PROP_USED,
	ZFS_PROP_AVAILABLE,
	ZFS_PROP_REFERENCED,
	ZFS_PROP_COMPRESSRATIO,
	ZFS_PROP_MOUNTED,
	ZFS_PROP_ORIGIN,
	ZFS_PROP_QUOTA,
	ZFS_PROP_RESERVATION,
	ZFS_PROP_VOLSIZE,
	ZFS_PROP_VOLBLOCKSIZE,
	ZFS_PROP_RECORDSIZE,
	ZFS_PROP_MOUNTPOINT,
	ZFS_PROP_SHARENFS,
	ZFS_PROP_CHECKSUM,
	ZFS_PROP_COMPRESSION,
	ZFS_PROP_ATIME,
	ZFS_PROP_DEVICES,
	ZFS_PROP_EXEC,
	ZFS_PROP_SETUID,
	ZFS_PROP_READONLY,
	ZFS_PROP_ZONED,
	ZFS_PROP_SNAPDIR,
	ZFS_PROP_ACLMODE,
	ZFS_PROP_ACLINHERIT,
	ZFS_PROP_CREATETXG,		/* not exposed to the user */
	ZFS_PROP_NAME,			/* not exposed to the user */
	ZFS_PROP_CANMOUNT,
	ZFS_PROP_SHAREISCSI,
	ZFS_PROP_ISCSIOPTIONS,		/* not exposed to the user */
	ZFS_PROP_XATTR,
	ZFS_PROP_NUMCLONES,		/* not exposed to the user */
	ZFS_PROP_COPIES,
	ZFS_PROP_BOOTFS
} zfs_prop_t;

typedef zfs_prop_t zpool_prop_t;

#define	ZFS_PROP_VALUE		"value"
#define	ZFS_PROP_SOURCE		"source"

typedef enum {
	ZFS_SRC_NONE = 0x1,
	ZFS_SRC_DEFAULT = 0x2,
	ZFS_SRC_TEMPORARY = 0x4,
	ZFS_SRC_LOCAL = 0x8,
	ZFS_SRC_INHERITED = 0x10
} zfs_source_t;

#define	ZFS_SRC_ALL	0x1f

/*
 * The following functions are shared between libzfs and the kernel.
 */
zfs_prop_t zfs_name_to_prop(const char *);
zpool_prop_t zpool_name_to_prop(const char *);
boolean_t zfs_prop_user(const char *);
int zfs_prop_readonly(zfs_prop_t);
const char *zfs_prop_default_string(zfs_prop_t);
const char *zfs_prop_to_name(zfs_prop_t);
const char *zpool_prop_to_name(zfs_prop_t);
uint64_t zfs_prop_default_numeric(zfs_prop_t);
int zfs_prop_inheritable(zfs_prop_t);
int zfs_prop_string_to_index(zfs_prop_t, const char *, uint64_t *);
int zfs_prop_index_to_string(zfs_prop_t, uint64_t, const char **);

/*
 * Property Iterator
 */
typedef zfs_prop_t (*zfs_prop_f)(zfs_prop_t, void *);
typedef zfs_prop_f zpool_prop_f;
extern zfs_prop_t zfs_prop_iter(zfs_prop_f, void *, boolean_t);
extern zpool_prop_t zpool_prop_iter(zpool_prop_f, void *, boolean_t);

/*
 * On-disk version number.
 */
#define	ZFS_VERSION_1			1ULL
#define	ZFS_VERSION_2			2ULL
#define	ZFS_VERSION_3			3ULL
#define	ZFS_VERSION_4			4ULL
#define	ZFS_VERSION_5			5ULL
#define	ZFS_VERSION_6			6ULL
/*
 * When bumping up ZFS_VERSION, make sure GRUB ZFS understand the on-disk
 * format change. Go to usr/src/grub/grub-0.95/stage2/{zfs-include/, fsys_zfs*},
 * and do the appropriate changes.
 */
#define	ZFS_VERSION			ZFS_VERSION_6
#define	ZFS_VERSION_STRING		"6"

/*
 * Symbolic names for the changes that caused a ZFS_VERSION switch.
 * Used in the code when checking for presence or absence of a feature.
 * Feel free to define multiple symbolic names for each version if there
 * were multiple changes to on-disk structures during that version.
 *
 * NOTE: When checking the current ZFS_VERSION in your code, be sure
 *       to use spa_version() since it reports the version of the
 *       last synced uberblock.  Checking the in-flight version can
 *       be dangerous in some cases.
 */
#define	ZFS_VERSION_INITIAL		ZFS_VERSION_1
#define	ZFS_VERSION_DITTO_BLOCKS	ZFS_VERSION_2
#define	ZFS_VERSION_SPARES		ZFS_VERSION_3
#define	ZFS_VERSION_RAID6		ZFS_VERSION_3
#define	ZFS_VERSION_BPLIST_ACCOUNT	ZFS_VERSION_3
#define	ZFS_VERSION_RAIDZ_DEFLATE	ZFS_VERSION_3
#define	ZFS_VERSION_DNODE_BYTES		ZFS_VERSION_3
#define	ZFS_VERSION_ZPOOL_HISTORY	ZFS_VERSION_4
#define	ZFS_VERSION_GZIP_COMPRESSION	ZFS_VERSION_5
#define	ZFS_VERSION_BOOTFS		ZFS_VERSION_6

/*
 * The following are configuration names used in the nvlist describing a pool's
 * configuration.
 */
#define	ZPOOL_CONFIG_VERSION		"version"
#define	ZPOOL_CONFIG_POOL_NAME		"name"
#define	ZPOOL_CONFIG_POOL_STATE		"state"
#define	ZPOOL_CONFIG_POOL_TXG		"txg"
#define	ZPOOL_CONFIG_POOL_GUID		"pool_guid"
#define	ZPOOL_CONFIG_CREATE_TXG		"create_txg"
#define	ZPOOL_CONFIG_TOP_GUID		"top_guid"
#define	ZPOOL_CONFIG_VDEV_TREE		"vdev_tree"
#define	ZPOOL_CONFIG_TYPE		"type"
#define	ZPOOL_CONFIG_CHILDREN		"children"
#define	ZPOOL_CONFIG_ID			"id"
#define	ZPOOL_CONFIG_GUID		"guid"
#define	ZPOOL_CONFIG_PATH		"path"
#define	ZPOOL_CONFIG_DEVID		"devid"
#define	ZPOOL_CONFIG_METASLAB_ARRAY	"metaslab_array"
#define	ZPOOL_CONFIG_METASLAB_SHIFT	"metaslab_shift"
#define	ZPOOL_CONFIG_ASHIFT		"ashift"
#define	ZPOOL_CONFIG_ASIZE		"asize"
#define	ZPOOL_CONFIG_DTL		"DTL"
#define	ZPOOL_CONFIG_STATS		"stats"
#define	ZPOOL_CONFIG_WHOLE_DISK		"whole_disk"
#define	ZPOOL_CONFIG_OFFLINE		"offline"
#define	ZPOOL_CONFIG_ERRCOUNT		"error_count"
#define	ZPOOL_CONFIG_NOT_PRESENT	"not_present"
#define	ZPOOL_CONFIG_SPARES		"spares"
#define	ZPOOL_CONFIG_IS_SPARE		"is_spare"
#define	ZPOOL_CONFIG_NPARITY		"nparity"

#define	VDEV_TYPE_ROOT			"root"
#define	VDEV_TYPE_MIRROR		"mirror"
#define	VDEV_TYPE_REPLACING		"replacing"
#define	VDEV_TYPE_RAIDZ			"raidz"
#define	VDEV_TYPE_DISK			"disk"
#define	VDEV_TYPE_FILE			"file"
#define	VDEV_TYPE_MISSING		"missing"
#define	VDEV_TYPE_SPARE			"spare"

/*
 * This is needed in userland to report the minimum necessary device size.
 */
#define	SPA_MINDEVSIZE		(64ULL << 20)

/*
 * The location of the pool configuration repository, shared between kernel and
 * userland.
 */
#define	ZPOOL_CACHE_DIR		"/etc/zfs"
#define	ZPOOL_CACHE_FILE	"zpool.cache"
// ### bring-up hack until vnode_rename is supported.
//#define	ZPOOL_CACHE_TMP		".zpool.cache"
#define	ZPOOL_CACHE_TMP		"zpool.cache"

#define	ZPOOL_CACHE		ZPOOL_CACHE_DIR "/" ZPOOL_CACHE_FILE

/*
 * vdev states are ordered from least to most healthy.
 * A vdev that's CANT_OPEN or below is considered unusable.
 */
typedef enum vdev_state {
	VDEV_STATE_UNKNOWN = 0,	/* Uninitialized vdev			*/
	VDEV_STATE_CLOSED,	/* Not currently open			*/
	VDEV_STATE_OFFLINE,	/* Not allowed to open			*/
	VDEV_STATE_CANT_OPEN,	/* Tried to open, but failed		*/
	VDEV_STATE_DEGRADED,	/* Replicated vdev with unhealthy kids	*/
	VDEV_STATE_HEALTHY	/* Presumed good			*/
} vdev_state_t;

/*
 * vdev aux states.  When a vdev is in the CANT_OPEN state, the aux field
 * of the vdev stats structure uses these constants to distinguish why.
 */
typedef enum vdev_aux {
	VDEV_AUX_NONE,		/* no error				*/
	VDEV_AUX_OPEN_FAILED,	/* ldi_open_*() or vn_open() failed	*/
	VDEV_AUX_CORRUPT_DATA,	/* bad label or disk contents		*/
	VDEV_AUX_NO_REPLICAS,	/* insufficient number of replicas	*/
	VDEV_AUX_BAD_GUID_SUM,	/* vdev guid sum doesn't match		*/
	VDEV_AUX_TOO_SMALL,	/* vdev size is too small		*/
	VDEV_AUX_BAD_LABEL,	/* the label is OK but invalid		*/
	VDEV_AUX_VERSION_NEWER,	/* on-disk version is too new		*/
	VDEV_AUX_VERSION_OLDER,	/* on-disk version is too old		*/
	VDEV_AUX_SPARED		/* hot spare used in another pool	*/
} vdev_aux_t;

/*
 * pool state.  The following states are written to disk as part of the normal
 * SPA lifecycle: ACTIVE, EXPORTED, DESTROYED, SPARE.  The remaining states are
 * software abstractions used at various levels to communicate pool state.
 */
typedef enum pool_state {
	POOL_STATE_ACTIVE = 0,		/* In active use		*/
	POOL_STATE_EXPORTED,		/* Explicitly exported		*/
	POOL_STATE_DESTROYED,		/* Explicitly destroyed		*/
	POOL_STATE_SPARE,		/* Reserved for hot spare use	*/
	POOL_STATE_UNINITIALIZED,	/* Internal spa_t state		*/
	POOL_STATE_UNAVAIL,		/* Internal libzfs state	*/
	POOL_STATE_POTENTIALLY_ACTIVE	/* Internal libzfs state	*/
} pool_state_t;

/*
 * Scrub types.
 */
typedef enum pool_scrub_type {
	POOL_SCRUB_NONE,
	POOL_SCRUB_RESILVER,
	POOL_SCRUB_EVERYTHING,
	POOL_SCRUB_TYPES
} pool_scrub_type_t;

/*
 * ZIO types.  Needed to interpret vdev statistics below.
 */
typedef enum zio_type {
	ZIO_TYPE_NULL = 0,
	ZIO_TYPE_READ,
	ZIO_TYPE_WRITE,
	ZIO_TYPE_FREE,
	ZIO_TYPE_CLAIM,
	ZIO_TYPE_IOCTL,
	ZIO_TYPES
} zio_type_t;

/*
 * Vdev statistics.  Note: all fields should be 64-bit because this
 * is passed between kernel and userland as an nvlist uint64 array.
 */
typedef struct vdev_stat {
	hrtime_t	vs_timestamp;		/* time since vdev load	*/
	uint64_t	vs_state;		/* vdev state		*/
	uint64_t	vs_aux;			/* see vdev_aux_t	*/
	uint64_t	vs_alloc;		/* space allocated	*/
	uint64_t	vs_space;		/* total capacity	*/
	uint64_t	vs_dspace;		/* deflated capacity	*/
	uint64_t	vs_rsize;		/* replaceable dev size */
	uint64_t	vs_ops[ZIO_TYPES];	/* operation count	*/
	uint64_t	vs_bytes[ZIO_TYPES];	/* bytes read/written	*/
	uint64_t	vs_read_errors;		/* read errors		*/
	uint64_t	vs_write_errors;	/* write errors		*/
	uint64_t	vs_checksum_errors;	/* checksum errors	*/
	uint64_t	vs_self_healed;		/* self-healed bytes	*/
	uint64_t	vs_scrub_type;		/* pool_scrub_type_t	*/
	uint64_t	vs_scrub_complete;	/* completed?		*/
	uint64_t	vs_scrub_examined;	/* bytes examined; top	*/
	uint64_t	vs_scrub_repaired;	/* bytes repaired; leaf	*/
	uint64_t	vs_scrub_errors;	/* errors during scrub	*/
	uint64_t	vs_scrub_start;		/* UTC scrub start time	*/
	uint64_t	vs_scrub_end;		/* UTC scrub end time	*/
} vdev_stat_t;

#define	ZFS_DRIVER	"zfs"
#define	ZFS_DEV		"/dev/zfs"

/*
 * zvol paths.  Irritatingly, the devfsadm interfaces want all these
 * paths without the /dev prefix, but for some things, we want the
 * /dev prefix.  Below are the names without /dev.
 */
#define	ZVOL_DEV_DIR	"zvol/dsk"
#define	ZVOL_RDEV_DIR	"zvol/rdsk"

/*
 * And here are the things we need with /dev, etc. in front of them.
 */
#define	ZVOL_PSEUDO_DEV		"/devices/pseudo/zvol@0:"
#define	ZVOL_FULL_DEV_DIR	"/dev/" ZVOL_DEV_DIR

#define	ZVOL_PROP_NAME		"name"

/*
 * /dev/zfs ioctl numbers.
 */
#define	IOCNUM_MASK	0x000000ff

#define	ZFS_IOC_NUM(cmd)	((cmd) & IOCNUM_MASK)

#define	ZFS_IOC_CMD(num)	_IOWR('Z', (num), struct zfs_cmd)

#define ZFS_IOC_POOL_CREATE         ZFS_IOC_CMD( 0)
#define ZFS_IOC_POOL_DESTROY        ZFS_IOC_CMD( 1)
#define ZFS_IOC_POOL_IMPORT         ZFS_IOC_CMD( 2)
#define ZFS_IOC_POOL_EXPORT         ZFS_IOC_CMD( 3)
#define ZFS_IOC_POOL_CONFIGS        ZFS_IOC_CMD( 4)
#define ZFS_IOC_POOL_STATS          ZFS_IOC_CMD( 5)
#define ZFS_IOC_POOL_TRYIMPORT      ZFS_IOC_CMD( 6)
#define ZFS_IOC_POOL_SCRUB          ZFS_IOC_CMD( 7)
#define ZFS_IOC_POOL_FREEZE         ZFS_IOC_CMD( 8)
#define ZFS_IOC_POOL_UPGRADE        ZFS_IOC_CMD( 9)
#define ZFS_IOC_POOL_GET_HISTORY    ZFS_IOC_CMD(10)
#define ZFS_IOC_POOL_LOG_HISTORY    ZFS_IOC_CMD(11)
#define ZFS_IOC_VDEV_ADD            ZFS_IOC_CMD(12)
#define ZFS_IOC_VDEV_REMOVE         ZFS_IOC_CMD(13)
#define ZFS_IOC_VDEV_ONLINE         ZFS_IOC_CMD(14)
#define ZFS_IOC_VDEV_OFFLINE        ZFS_IOC_CMD(15)
#define ZFS_IOC_VDEV_ATTACH         ZFS_IOC_CMD(16)
#define ZFS_IOC_VDEV_DETACH         ZFS_IOC_CMD(17)
#define ZFS_IOC_VDEV_SETPATH        ZFS_IOC_CMD(18)
#define ZFS_IOC_OBJSET_STATS        ZFS_IOC_CMD(19)
#define ZFS_IOC_DATASET_LIST_NEXT   ZFS_IOC_CMD(20)
#define ZFS_IOC_SNAPSHOT_LIST_NEXT  ZFS_IOC_CMD(21)
#define ZFS_IOC_SET_PROP            ZFS_IOC_CMD(22)
#define ZFS_IOC_CREATE_MINOR        ZFS_IOC_CMD(23)
#define ZFS_IOC_REMOVE_MINOR        ZFS_IOC_CMD(24)
#define ZFS_IOC_CREATE              ZFS_IOC_CMD(25)
#define ZFS_IOC_DESTROY             ZFS_IOC_CMD(26)
#define ZFS_IOC_ROLLBACK            ZFS_IOC_CMD(27)
#define ZFS_IOC_RENAME              ZFS_IOC_CMD(28)
#define ZFS_IOC_RECVBACKUP          ZFS_IOC_CMD(29)
#define ZFS_IOC_SENDBACKUP          ZFS_IOC_CMD(30)
#define ZFS_IOC_INJECT_FAULT        ZFS_IOC_CMD(31)
#define ZFS_IOC_CLEAR_FAULT         ZFS_IOC_CMD(32)
#define ZFS_IOC_INJECT_LIST_NEXT    ZFS_IOC_CMD(33)
#define ZFS_IOC_ERROR_LOG           ZFS_IOC_CMD(34)
#define ZFS_IOC_CLEAR               ZFS_IOC_CMD(35)
#define ZFS_IOC_PROMOTE             ZFS_IOC_CMD(36)
#define ZFS_IOC_DESTROY_SNAPS       ZFS_IOC_CMD(37)
#define ZFS_IOC_SNAPSHOT            ZFS_IOC_CMD(38)
#define ZFS_IOC_DSOBJ_TO_DSNAME     ZFS_IOC_CMD(39)
#define ZFS_IOC_OBJ_TO_PATH         ZFS_IOC_CMD(40)
#define ZFS_IOC_POOL_SET_PROPS		ZFS_IOC_CMD(41)
#define	ZFS_IOC_POOL_GET_PROPS		ZFS_IOC_CMD(42)

/*
 * Internal SPA load state.  Used by FMA diagnosis engine.
 */
typedef enum {
	SPA_LOAD_NONE,		/* no load in progress */
	SPA_LOAD_OPEN,		/* normal open */
	SPA_LOAD_IMPORT,	/* import in progress */
	SPA_LOAD_TRYIMPORT	/* tryimport in progress */
} spa_load_state_t;

/*
 * Bookmark name values.
 */
#define	ZPOOL_ERR_LIST		"error list"
#define	ZPOOL_ERR_DATASET	"dataset"
#define	ZPOOL_ERR_OBJECT	"object"

#define	HIS_MAX_RECORD_LEN	(MAXPATHLEN + MAXPATHLEN + 1)

/*
 * The following are names used in the nvlist describing
 * the pool's history log.
 */
#define	ZPOOL_HIST_RECORD	"history record"
#define	ZPOOL_HIST_TIME		"history time"
#define	ZPOOL_HIST_CMD		"history command"

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_FS_ZFS_H */
