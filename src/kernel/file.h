#pragma once
#include "kernel/types.h"

#define NDEV 10
#define NFILE 100

#define DEV_CONSOLE 1

typedef struct device {
  int (*read)(void);
  void (*write)(char byte);
} device_t;

typedef struct file {
  enum { FD_NONE, FD_INODE, FD_DEVICE } type;
  int refcount;
  struct inode *ip;
  uint32_t off;
  short devno;
} file_t;


extern device_t devices[];
extern file_t fd_table[];
