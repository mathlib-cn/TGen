long double_to_long(double f)
{
	long *p = (long*)&f;
	long temp = *p;
	long exp;
	long tail;
	long res;
	
	exp = ((temp >> 52) & 0x7ff) - 1023;
	
	if (exp < 0) {
		return 0;
	}
	else {
		tail = (temp & 0x000fffffffffffff) | 0x0010000000000000;
		res = tail >> (52 - exp);
		return res;
	}
}
