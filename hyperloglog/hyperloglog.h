/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */

#ifndef hyperloglog_h_
#define hyperloglog_h_

#include <stdint.h>

#define HLL_P 14 /* The greater is P, the smaller the error. */
#define HLL_REGISTERS (1<<HLL_P) /* With P=14, 16384 registers. */
#define HLL_P_MASK (HLL_REGISTERS-1) /* Mask to index register. */
#define HLL_BITS 6 /* Enough to count up to 63 leading zeroes. */
#define HLL_REGISTER_MAX ((1<<HLL_BITS)-1)
#define HLL_HDR_SIZE sizeof(HyperLogLog)
#define HLL_DENSE_SIZE (HLL_HDR_SIZE+((HLL_REGISTERS*HLL_BITS+7)/8))
#define HLL_DENSE 0 /* Dense encoding. */
#define HLL_SPARSE 1 /* Sparse encoding. */
#define HLL_RAW 255 /* Only used internally, never exposed. */
#define HLL_MAX_ENCODING 1

#define HLL_MAX_SPARSE_BYTES 11000

typedef struct {
  /*
   * Dense, Sparse or Explicit, Dirty or Clean?
   */
  char encoding;
  /*
   * Cached computed cardinality which can be used until new data is added.
   */
  long card;
  /*
   * Number of leading bits for the register allocation.
   */
  uint8_t p;
  /*
   * Number of bytes allocated for the bitmap.
   */
  int mlen;
  /*
   * Registers to store the bitmap.
   */
  uint8_t registers[HLL_REGISTERS];
} HyperLogLog;

int hll_create(HyperLogLog *hll);
int hll_add(HyperLogLog *hll, unsigned char *ele, size_t elesize);
uint64_t hll_count(HyperLogLog *hll);

#endif//hyperloglog_h_
