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

#include "pipe.h"
#include <QtCore>

const unsigned int DataSize = 2047;
const unsigned int BufferSize = 1024;
QSemaphore ReqPipeFree(BufferSize);
QSemaphore ReqPipeUsed(0);
QSemaphore GetPipeFree(BufferSize);
QSemaphore GetPipeUsed(0);
char RequestPipe[BufferSize][200];
char RequestPipeStat[BufferSize][200];
char GetPipe[BufferSize][200];
char GetPipeStat[BufferSize][200];
