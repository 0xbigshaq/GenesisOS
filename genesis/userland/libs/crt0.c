#include "crt0.h"
#include "userland/libs/std.h"


void _start() {
    int rc = OK;
    int argc_dummy = 1;
    char *argv_dummy[] = { "/init" };
    rc = main(argc_dummy, argv_dummy); // FIXME: should setup aux data
    exit(rc);
}