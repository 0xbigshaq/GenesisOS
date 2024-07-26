/**
 * @file device.h
 * @brief Device driver interface
 * @details This file contains the interface.
 */
#ifndef _DEVICE_GENESIS_H_
#define _DEVICE_GENESIS_H_

#include "drivers/fat32.h"
#include "kernel/types.h"
#include "drivers/fat32.h"

#define NDEV 10         //!< Maximum number of devices
#define DEV_CONSOLE 1   //!< ID of the Console device

/**
 * @brief The device interface.
 * @details This structure contains the read and write functions for the device.
 */
typedef struct device {
  int (*read)(uint8_t*, uint32_t);
  int (*write)(uint8_t*, uint32_t);
} device_t;

/**
 * @brief global device array.
 * @details Each element in this array is a device_t structure. The kernel choose the device to use
  by its ID in `file_t::devno`.
 */
extern device_t all_devs[];

#endif  // _DEVICE_GENESIS_H_

// ------- EOF ---------
// ------ doxygen ------

/*! \page devices Devices
 *  \brief A description about the device driver interface.

  \tableofcontents
  
  \section sec Architecture Overview
  The `device_t` interface is a structure that provides a standardized  way for device  drivers  to 
  implement read and write operations. This interface allows the kernel to interact with different 
  device drivers in a uniform manner.

  The `device_t` structure is defined as follows:
  \code
  typedef struct device {
    int (*read)(uint8_t*, uint32_t);
    int (*write)(uint8_t*, uint32_t);
  } device_t;
  \endcode
  Each device driver must implement the read and write  functions.  These functions are responsible 
  for reading from and writing to the device, respectively.

\dot
digraph architecture_diagram {
  node [shape=box, style=filled, color=lightblue];

  UserProcess [label="User Process"];
  SyscallDispatch [label="sys_dispatch()"];
  SysRead [label="sys_read()"];
  SysWrite [label="sys_write()"];
  DeviceInterface [label="device_t Interface"];

  UserProcess -> SyscallDispatch [label="trigger a syscall"];
  SyscallDispatch -> SysRead;
  SyscallDispatch -> SysWrite;
  SysRead -> DeviceInterface [label="read"];
  SysWrite -> DeviceInterface [label="write"];
}
\enddot


  \section sec_registration Device Registration
  Each device driver must register itself for the kernel. This is typically done during the device's
  initialization. For example, the video console driver registers its read and write functions as 
  follows:
  \code
  void vconsole_init() {
    vconsole_ctx_t *v = vconsole_get_ctx();
    devices[DEV_CONSOLE].write = &vconsole_write;
    devices[DEV_CONSOLE].read = &vconsole_read;
  }
  \endcode

  \section sec_usage Device Usage in GenesisOS
  The kernel uses the registered read and write callbacks during system calls. When a user process 
  invokes a read or write operation, the corresponding device driver's functions are called. Here is
   an example of how this is handled in the kernel:

  \subsection subsection_read Reading from a Device
  The `sys_read` function handles read syscall logic. It checks the type of the file descriptor and 
  calls the appropriate read function:

  \code{.c}
  int sys_read(void) {
      uint32_t fd = arg_word(0);
      char* out = (char*)arg_ptr(1);
      uint32_t count = arg_word(2);
      task_t *proc = cur_proc();
      int rc = 0;

      // ... more code ...
      if(proc->ofile[fd].type == FD_DEVICE) {
          rc = devices[proc->ofile[fd].devno].read(out, count);
      }
      // ... more code ...

      return rc;
  }
  \endcode

  \subsection subsection_write Writing to a Device
  The `sys_write` syscall handles write operations. It similarly checks the type of 
  the file descriptor and calls the appropriate write function:

  \code{.c}
  int sys_write(void) {
      uint32_t fd = arg_word(0);
      uint8_t *buf = (uint8_t*)arg_ptr(1);
      uint32_t count = arg_word(2);
      task_t *proc = cur_proc();
      int rc = OK;
      UINT bytes_out;

      // ... more code ...
      if(proc->ofile[fd].type == FD_DEVICE) {
          rc = devices[proc->ofile[fd].devno].write(buf, count);
      }
      // ... more code ...
      return rc;
  }
  \endcode

  \section sec_conclusion Conclusion
  The `device_t`  interface  provides a  flexible and  standardized method  for implementing device 
  drivers in the kernel.     By adhering to this interface, developers can ensure their drivers are 
  compatible with the kernel's read and write operations,   enabling seamless interaction with user 
  processes.

*/

