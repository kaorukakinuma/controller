/*
 * MIT License
 * 
 * Copyright (c) 2020 Kaoru Kakinuma
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _CONTROLLER_LOG_H_
#define _CONTROLLER_LOG_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ERRLOG( fmt, ... )\
    (void)printf( "[ \x1b[31mERR\x1b[m ] %s %s() %d, %s: " fmt "\n",\
    __FILE__, __FUNCTION__, __LINE__, strerror(errno), ##__VA_ARGS__ )

#define DBGLOG( fmt, ... )\
    (void)printf( "[ \x1b[36mDBG\x1b[m ] " fmt "\n", ##__VA_ARGS__ )

#endif /* _CONTROLLER_LOG_H_ */
