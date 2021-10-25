#ifndef MD5_H
#define MD5_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <string.h>
#include <math.h>


typedef uint32_t uint32;
typedef uint64_t uint64;
uint32* md5(std::istream* input);

#endif // MD5_H
