/*
 * Author: Hiroyuki Chishiro
 * License: 2-Clause BSD
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "id.h"
#include "period.h"

void swap(uint16_t *pa, uint16_t *pb) {
  int tmp;

  tmp = *pa;
  *pa = *pb;
  *pb = tmp;
}

void rotate(size_t first, size_t middle, size_t last, uint16_t v[]) {
  size_t middle_org;

  if (first == middle || middle == last) {
    return;
  }

  middle_org = middle;

  while (first != middle_org && middle != last) {
    swap(&v[first++], &v[middle++]);
  }

  if (first == middle_org) {
    rotate(first, middle, last, v);
  } else {
    rotate(first, middle_org, last, v);
  }
}

bool next_combination(size_t first, size_t last, size_t r, uint16_t v[]) {
  size_t subset = first + r;
  size_t src = subset;
  size_t dst = subset;

  if (first == last || first == subset || last == subset) {
    return false;
  }

  while (first != src) {
    src--;

    if (v[src] < v[last - 1]) {

      while (v[src] >= v[dst]) {
        dst++;
      }

      swap(&v[src], &v[dst]);
      rotate(src + 1, dst + 1, last, v);
      rotate(subset, subset + (last - dst) - 1, last, v);

      return true;
    }
  }

  rotate(first, subset, last, v);

  return false;
}

void calc_mask(uint16_t ids[], size_t n, size_t r, uint16_t filt_mask[]) {
  uint16_t mask = 0;
  filt_mask[0] = 0x0;
  for (int i = 0; i < 11; i++) {
    mask = 1 << i;
    for (int j = 0; j < r; j++) {
      if ((ids[0] & mask) != (ids[j] & mask)) {
        mask = 0;
        break;
      }
    }
    filt_mask[0] += mask;
  }
  filt_mask[1] = 0x0;
  for (int i = 0; i < 11; i++) {
    mask = 1 << i;
    for (int j = r; j < n; j++) {
      if ((ids[r] & mask) != (ids[j] & mask)) {
        mask = 0;
        break;
      }
    }
    filt_mask[1] += mask;
  }
}

void calc_result(uint16_t filt_id[], uint16_t filt_mask[]) {
  uint16_t cids = 0;
  float cperiods = 0;
  for (int i = 0; i < sizeof(periods) / sizeof(periods[0]); i++) {
    if (((filt_id[0] & filt_mask[0]) == (periods[i][0] & filt_mask[0])) ||
        ((filt_id[1] & filt_mask[1]) == (periods[i][0] & filt_mask[1]))) {
      cids++;
      cperiods += (1000.0 / periods[i][1]);
    }
  }
  printf("%.1f %02d/%02ld ", cperiods, cids, sizeof(periods) / sizeof(periods[0]));
}

int main(void) {
  size_t i;
  size_t n = sizeof(ids) / sizeof(ids[0]);
  size_t r = 1;
  uint16_t top = ids[0];
  uint16_t filt_id[] = {0x0, 0x0};
  uint16_t filt_mask[] = {0x7ff, 0x7ff};

  for (r = 1; r <= n / 2; r++) {
    do {
      if (!(n % 2) && r == n / 2 && ids[0] != top) {
        break;
      }
      
      calc_mask(ids, n, r, filt_mask);
      filt_id[0] = ids[0];
      filt_id[1] = ids[r];
      calc_result(filt_id, filt_mask);
      printf("| ");
      for (i = 0; i < r; i++) {
        printf("0x%03x ", ids[i]);
      }

      printf("| ");

      for (i = r; i < n; i++) {
        printf("0x%03x ", ids[i]);
      }
      printf("| ");

      printf("0x%03x 0x%03x ", filt_mask[0], filt_mask[1]);
      printf("\n");
      
    } while (next_combination(0, n, r, ids));
    rotate(0, 0, n, ids);
  }

  return 0;
}
