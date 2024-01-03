void arrayRotate(unsigned long *ary, unsigned char length) {
  length--;
  unsigned long first = ary[0];

  for(unsigned char i = 0; i < length; ++i)
    ary[i] = ary[i + 1];

  ary[length] = first;
}

void arrayShuffle(unsigned long *ary, unsigned char length) {
  if (length > 1) {
    unsigned char length_minus_one = length - 1;

    for(unsigned char i = 0; i < length_minus_one; ++i) {
      unsigned char j = i + rand() / (RAND_MAX / (length - i) + 1);

      unsigned long temp = ary[j];
      ary[j] = ary[i];
      ary[i] = temp;
    }
  }
}
