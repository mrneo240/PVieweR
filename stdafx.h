// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
// Windows Header Files:
#include <windows.h>

/* Program Defines 
 *
 * NO_FRAME		- Window doesn't have a border or frame
 * PVR_TRANS	- Render transparency algo into BMP and display Transparent window
 */
#define NO_FRAME	0
#define PVR_TRANS	0

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <shellapi.h>
