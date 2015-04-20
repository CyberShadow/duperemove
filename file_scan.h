#ifndef	__FILE_SCAN_H__
#define	__FILE_SCAN_H__

#include "d_tree.h"

/* from duperemove.c */
extern int run_dedupe;
extern int one_file_system;
extern int recurse_dirs;
extern unsigned int blocksize;
extern int do_lookup_extents;
extern unsigned int io_threads;

/*
 * Returns nonzero on fatal errors only
 */
int add_file(const char *name, int dirfd);
int populate_tree_aim(struct hash_tree *tree);
int populate_tree_swap(struct rb_root *tree, char *swapfile);
int revalidate_filerecs(void);

#endif	/* __FILE_SCAN_H__ */
