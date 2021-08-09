// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file genres.h
 * Similar to genfs.h, but utility functions take an extra state pointer to provide optimizations around already open or loaded resources
 * @note all utilities in this header take and write to state pointers, even when it may be harmful to do so, use in combination with raw utilities in genfs.h
 * @see genfs.h for raw versions of filesystem utilities
 */

#ifndef GEN_RES_H
#define GEN_RES_H

#include "gencommon.h"
#include "genfs.h"

#endif
