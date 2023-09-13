/*
 * "streamable kanji code filter and converter"
 * Copyright (c) 1998-2002 HappySize, Inc. All rights reserved.
 *
 * LICENSE NOTICES
 *
 * This file is part of "streamable kanji code filter and converter",
 * which is distributed under the terms of GNU Lesser General Public
 * License (version 2) as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with "streamable kanji code filter and converter";
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
 * The author of this file:
 *
 */
/*
 * the source code included in this files was separated from mbfilter_ja.c
 * by moriyoshi koizumi <moriyoshi@php.net> on 4 dec 2002.
 *
 */

#ifndef MBFL_MBFILTER_CP932_H
#define MBFL_MBFILTER_CP932_H

#include "libmbfl_php80/mbfl/mbfilter.h"

extern const mbfl_encoding mbfl_encoding_cp932;
extern const struct mbfl_identify_vtbl vtbl_identify_cp932;
extern const struct mbfl_convert_vtbl vtbl_cp932_wchar;
extern const struct mbfl_convert_vtbl vtbl_wchar_cp932;

int mbfl_filt_conv_cp932_wchar(int c, mbfl_convert_filter *filter);
int mbfl_filt_conv_wchar_cp932(int c, mbfl_convert_filter *filter);

#endif /* MBFL_MBFILTER_CP932_H */