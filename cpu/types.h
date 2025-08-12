#ifndef TYPES_H
#define TYPES_H

#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)((address) >> 16 & 0xFFFF)

#endif
