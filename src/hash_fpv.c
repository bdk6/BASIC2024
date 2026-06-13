

/* Fowler-Noll-Vo hash function */
/* See Wikipedia article */


#include <stdint.h>


#define OFFSET_BASIS_32  (2166136261L)
#define FNV_PRIME_32     (16777619L)

uint32_t hash_fnv1a(uint8_t* str)
{
  uint32_t hash = OFFSET_BASIS_32;
  while(*str)
  {
    hash ^= *str;
    hash *= FNV_PRIME_32;
    str++;
  }
  return hash;
}

