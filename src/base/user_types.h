// user_types.h

#ifndef USER_TYPES_H
#define USER_TYPES_H

// operating system dependent
extern const char* DIR_SEPARATOR;
extern const char* HOME_DIRECTORY;

// standard types
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

// compiler specific types
#ifdef _MSC_VER
typedef __int64 int64;
typedef unsigned __int64 uint64;
#endif
#ifdef __GNUG__
typedef long long int64;
typedef unsigned long long uint64;
#endif

#ifdef _BIG_ENDIAN
// change byte order
extern inline void swapBytes(int8 &);
extern inline void swapBytes(uint8 &);
extern inline void swapBytes(int16 &value);
extern inline void swapBytes(uint16 &value);
extern inline void swapBytes(int32 &value);
extern inline void swapBytes(uint32 &value);
extern inline void swapBytes(int64 &value);
extern inline void swapBytes(uint64 &value);
extern inline void swapBytes(float32 &value);
extern inline void swapBytes(float64 &value);
#endif

#endif
