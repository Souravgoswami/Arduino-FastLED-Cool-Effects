void arrayRotate(uint32_t *ary, uint8_t length) {
  length--;
  uint32_t first = ary[0];

  for(uint8_t i = 0; i < length; ++i)
    ary[i] = ary[i + 1];

  ary[length] = first;
}

void arrayShuffle(uint32_t *ary, uint8_t length) {
  if (length > 1) {
    uint8_t length_minus_one = length - 1;

    for(uint8_t i = 0; i < length_minus_one; ++i) {
      uint8_t j = i + rand() / (RAND_MAX / (length - i) + 1);

      uint32_t temp = ary[j];
      ary[j] = ary[i];
      ary[i] = temp;
    }
  }
}
