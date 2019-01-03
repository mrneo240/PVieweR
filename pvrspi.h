/*
Copyright 2018 NeoDC/HaydenK.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#pragma once
#include "targetver.h"

#define	SKIP_GBIX(data) \
	if (memcmp(data,"GBIX",4)==0) {\
		long size = ((long*)data)[1]; \
		data+=size+8; \
	}

enum { ARGB1555, RGB565, ARGB4444, YUV422, BUMP, PAL4, PAL8 };

typedef struct {
	BYTE  id[4];
	DWORD length;
	BYTE  type[4];
	WORD width;
	WORD height;
} PVRHDR;

int pvr2bmphdr(BITMAPINFOHEADER *pInfo, PVRHDR *hdr);
int pvr2bmp(char *pBm, PVRHDR *hdr);
int _pvr2bmp(char *pBm, PVRHDR *hdr, BOOL alpha_map);
int pvr2bmpfile(LPCWSTR infile, LPCWSTR outfile);