#include "types.h"
#include <stdarg.h>
#include <memory.h>
#include <stdlib.h>
#include <float.h>
#define fn static
#define ArraySize(Array) (sizeof(Array) / sizeof(*Array))

#define KB(Bytes) (Bytes*1024)
#define MB(Bytes) (Bytes*1024*1024)
#define GB(Bytes) (Bytes*1024*1024*1024)

#define ZeroStruct(Struct) memset(Struct, 0, sizeof(*Struct))

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

typedef struct 
{
	int32_t size;
	uint8_t *bytes;
} file_t;

fn file_t OpenSystemFile(const char *path);
fn void CloseSystemFile(file_t*file);
fn void Error(const char *format, ...);

fn void _Assert(const char *message, const char *file, const char *function, int32_t line);
#define Assert(expression) (void) ((!!(expression)) || \
(_Assert(#expression, __FILE__, __FUNCTION__, __LINE__), 0))
#ifdef _DEBUG
#define DebugAssert(...) Assert(__VA_ARGS__)
#else
#define DebugAssert(...)
#endif

#ifdef _DEBUG
#include <stdio.h>
#else
#endif

// NOTE():
fn s32 StringLength(const char *string);

#define _USE_MATH_DEFINES
#include <math.h>
#define M_TAU32 (f32)(M_PI * 2.0)

// NOTE(): Numerical.
fn f32 Sine(f32 x);
fn f32 Cosine(f32 x);
fn f32 SqRoot(f32 x);
fn f32 Lerp(f32 a, f32 b, f32 t);

fn s32 RandomInt(void);
fn s32 MinS32(s32 a, s32 b);