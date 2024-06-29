#pragma once
#include "ff.h"
#include "kernel/types.h"
#include "drivers/fat32.h"

#define NDEV 10
#define NFILE 100

#define DEV_CONSOLE 1

typedef FIL file_descriptor_t;
typedef FILINFO file_info_t;

typedef struct device {
  int (*read)(void);
  void (*write)(char byte);
} device_t;

typedef struct file {
  enum { FD_NONE, FD_FILE, FD_DEVICE } type;
  int refcount;
  file_descriptor_t *fd;
  file_info_t *info;
  uint32_t off;
  short devno;
} file_t;


extern device_t devices[];
extern file_t fd_table[];
