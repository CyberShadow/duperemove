/*
 * serialize.h
 *
 * Copyright (C) 2014 SUSE.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#ifndef __SERIALIZE__
#define __SERIALIZE__

#include <linux/types.h>
#include "d_tree.h"

#define HASH_FILE_MAJOR	1
#define HASH_FILE_MINOR	1

#define HASH_FILE_MAGIC		"dupehash"
struct hash_file_header {
/*00*/	char		magic[8];
	__le64		major;
	__le64		minor;
	__le64		num_files;
/*20*/	__le64		num_hashes;
	__le32		block_size; /* In bytes */
	__le32		num_subvol_info;
	char		hash_type[8];
	__le64		subvol_info_off; /* Absolute offset of start of subvol
					  * info */
	__le64		pad1[8];
};

#define DISK_DIGEST_LEN		32

struct block_hash {
	__le64		loff;
	__le32		flags;
	__le32		pad[2];
	char		digest[DISK_DIGEST_LEN];
};

struct file_info {
/*00*/	__le64		ino;
	__le64		file_size;
	__le64		num_blocks;
	__le16		rec_len;
	__le16		name_len;
	__le16		on_btrfs;
	__le16		pad0;
	__le64		subvolid;
/*20*/	__le64		pad1[2];
	char		name[0];
};

#define	UUID_SIZE	16
struct subvol_info {
	__le64	subvolid;
	__le64	last_gen;
	__le64	pad[2];
	__le64	uuid[UUID_SIZE];
	__le32	pad1;
	__le16	rec_len;
	__le16	path_len;
	__le64	pad3[3];
	char	path[0];
};

struct hash_tree;
int serialize_hash_tree(char *filename, struct hash_tree *tree,
			unsigned int block_size);

#define	FILE_VERSION_ERROR	1001
#define	FILE_MAGIC_ERROR	1002
#define	FILE_HASH_TYPE_ERROR	1003
extern char unknown_hash_type[8];
/* read_hash_tree 'recent' arg tells us whether this tree was just written */
int read_hash_tree(char *filename, struct hash_tree *tree,
		   unsigned int *block_size, struct hash_file_header *ret_hdr,
		   int ignore_hash_type, struct rb_root *scan_tree, int recent);
int read_filerecs(char *filename);
/* Need a variant of read_hash_tree that doesn't make new filerecs */

/* Pretty-prints errors from read_hash_tree for us */
void print_hash_tree_errcode(FILE *io, char *filename, int ret);

int write_header(int fd, uint64_t num_files, uint64_t num_hashes,
		 uint64_t subvol_info_off, uint32_t block_size);
int write_subvol_info(int fd, uint64_t *subvol_info_off);
int write_file_info(int fd, struct filerec *file);
int write_one_hash(int fd, uint64_t loff, uint32_t flags,
			  unsigned char *digest);

#endif /* __SERIALIZE__ */
