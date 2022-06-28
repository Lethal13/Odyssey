#ifndef ODYSSEY_SRC_ODYSSEY_H
#define ODYSSEY_SRC_ODYSSEY_H

#include <climits>
#include <cstdint>

#define ArraySize(Array) (sizeof(Array) / sizeof(Array[0]))
#define PI 3.14159

#define Min(A, B) ((A < B) ? (A) : (B))
#define Max(A, B) ((A > B) ? (A) : (B))
#define Radians(theta) ((theta) * PI/180.0f)
#define Degrees(theta) ((theta) * 180.0f/PI)

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

// #define Assert(x) do { (void)sizeof(x);} while (0)

#define ODS_ASSERT(Expression) if(!(Expression)) {*(volatile int *)0 = 0;}


#include "../vulkan/vk_odyssey.cpp"

#endif
