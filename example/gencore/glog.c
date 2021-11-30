// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>

int main(void) {
    glog(TRACE, "This is a trace level message");
    glog(PERFORMANCE, "This is a performance level message");
    glog(NOTE, "This is a note level message");
    glog(INFO, "This is an info level message");
    glog(WARNING, "This is a warning level message");
    glog(ERROR, "This is an error level message");
    glog(FATAL, "This is a fatal level message");

    glogf(TRACE, "This is a trace level message: %zu %s %lf", 42, "This is another string!", 0.5354);
    glogf(PERFORMANCE, "This is a performance level message: %zu %s %lf", 42, "This is another string!", 0.5354);
    glogf(NOTE, "This is a note level message: %zu %s %lf", 42, "This is another string!", 0.5354);
    glogf(INFO, "This is an info level message: %zu %s %lf", 42, "This is another string!", 0.5354);
    glogf(WARNING, "This is a warning level message: %zu %s %lf", 42, "This is another string!", 0.5354);
    glogf(ERROR, "This is an error level message: %zu %s %lf", 42, "This is another string!", 0.5354);
    glogf(FATAL, "This is a fatal level message: %zu %s %lf", 42, "This is another string!", 0.5354);
}
