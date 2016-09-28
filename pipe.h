/*
Copyright (C) 2016 Sailley Thomas

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License v3 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef PIPE_H
#define PIPE_H
#include <QtCore>
#define TIMEOUT(k) 1000*k
extern const unsigned int DataSize;
extern const unsigned int BufferSize;
extern QSemaphore ReqPipeFree;
extern QSemaphore ReqPipeUsed;
extern QSemaphore GetPipeFree;
extern QSemaphore GetPipeUsed;
extern char RequestPipe[1024][200];
extern char RequestPipeStat[1024][200];
extern char GetPipe[1024][200];
extern char GetPipeStat[1024][200];

#endif // PIPE_H
