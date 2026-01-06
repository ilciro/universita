#include <stdio.h>
#include <stdint.h>

union binary_float_t {
	float real;
	uint32_t integer; // Assumes float is 32 bits wide
};

int main(void)
{
	union binary_float_t f;
	f.real = 3.141592F;
	printf("Hex representation of %f is %#04x\n", f.real, f.integer);
	return 0;
}
