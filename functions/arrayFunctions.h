void arrayRotate(unsigned long *ary, unsigned char length) {
	length-- ;
	unsigned long first = ary[0] ;

	for(unsigned char i = 0 ; i < length ; ++i)
		ary[i] = ary[i + 1] ;

	ary[length] = first ;
}

void arrayShuffle(unsigned long *ary, unsigned char length) {
	if (length > 1) {
		unsigned char length_minus_one = length - 1 ;

		for(unsigned char i = 0 ; i < length_minus_one ; ++i) {
			unsigned char j = i + rand() / (RAND_MAX / (length - i) + 1) ;

			unsigned long temp = ary[j] ;
			ary[j] = ary[i] ;
			ary[i] = temp ;
		}
	}
}

// Reject previous colour in LED.
// For example:
//	Invalid: g - g - r - b - b - g
//	Valid: g - r - b - r - b - g
void arrayShuffleUniqColours(const unsigned long *target, unsigned char length, unsigned long *dest, unsigned char dest_length) {
	if (length > 1) {
		unsigned long newAry[length - 1] ;
		unsigned long taken ;

		taken = target[rand() % length] ;
		dest[0] = taken ;

		for(unsigned char i = 1 ; i < dest_length ; ++i) {
			unsigned int k = 0 ;

			for(unsigned char j = 0 ; j < length ; ++j) {
				if (target[j] != taken) {
					newAry[k++] = target[j] ;
				}
			}

			taken = newAry[rand() % k] ;
			dest[i] = taken ;
		}
	}
}

// void rotateColoursAnim(unsigned long *colours, unsigned char length) {
// 	for(unsigned char i = 0 ; i < NUM_LEDS ; ++i) {
// 		leds[i] = colours[0] ;
// 		arrayRotate(colours, length) ;
// 		FastLED.show() ;
// 		delay(10) ;
// 	}
// }
