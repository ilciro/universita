#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define FLOAT_VAL		5.1
#define SIGN_VAL		1
#define EXPONENT_VAL	120
#define MANTISSA_VAL	1685475



union flt {
	struct ieee754 {
		uint32_t mantissa: 23;
		uint32_t exponent: 8;
		uint32_t sign: 1;
	} raw;
	float f;
};

char *print_bits(int bits, unsigned long long value)
{
	int i;
	char *p;
	char *string = malloc(bits	 + 1);
	
	if(!string)
		return NULL;
	
	p = string + bits;
	p[0] = 0;
	
	for(i = 0; i < bits; i++) {
	    *--p = value & 1 ? '1' : '0';
	    value >>= 1;
	}
	return string;
}

int main(void)
{
	union flt number;
	char *exponent, *mantissa;
	
	number.f = FLOAT_VAL;
	exponent = print_bits(8, number.raw.exponent);
	mantissa = print_bits(23, number.raw.mantissa);
	printf("Converting float %f to components:\n", number.f);
	printf("\tsign......: %d\n", number.raw.sign);
	printf("\texponent..: %s (%d)\n", exponent, number.raw.exponent);
	printf("\tmantissa..: %s (%d)\n", mantissa, number.raw.mantissa);
	free(exponent);
	free(mantissa);
	
	number.raw.sign = SIGN_VAL;
	number.raw.exponent = EXPONENT_VAL;
	number.raw.mantissa = MANTISSA_VAL;
	
	exponent = print_bits(8, number.raw.exponent);
	mantissa = print_bits(23, number.raw.mantissa);
	printf("\fConverting %d %s %s to float:\n", number.raw.sign, exponent, mantissa);
	printf("\t%f\n", number.f);
	free(exponent);
	free(mantissa);

	return 0;
}

