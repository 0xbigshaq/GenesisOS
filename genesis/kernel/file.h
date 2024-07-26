/**
 * @file file.h
 * @brief Abstraction of file objects.
 */
#ifndef FILE_GENESIS_H
#define FILE_GENESIS_H
#include "drivers/ff15/ff.h"
#include "kernel/types.h"
#include "drivers/fat32.h"

typedef FIL file_descriptor_t;  //!< File descriptor structure (FatFS structure)
typedef FILINFO file_info_t;    //!< File info structure (FatFS structure)

/**
 * @brief The type of the file object.
 */
enum file_type {
  FD_NONE,        //!< No file
  FD_FILE,        //!< File on disk
  FD_DEVICE       //!< Character Device, see @ref devices
};

/**
 * @brief Kernel file object.
 * @details This structure represents a file object in the kernel. Every 'file' in GenesisOS can
  *  have  two types:
  * - `FD_FILE`: File on disk.
  * - `FD_DEVICE`: Character Device, see @ref devices
 */
typedef struct file {
  enum file_type type;    //!< File type
  int refcount;           //!< Reference count
  file_descriptor_t *fd;  //!< File descriptor
  file_info_t *info;      //!< File info
  uint32_t off;           //!< Current file offset
  short devno;            //!< Device idx in @ref all_devs "all_devs[]" global. Used only when @ref type is @ref FD_DEVICE
} file_t;


#endif // FILE_GENESIS_H