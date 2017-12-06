/**
 * @file 		ops.c
 * @author		Berki Tamás - PDRE31
 * @date		2017.12.02.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "utils.h"
#include "stack.h"
#include "ops.h"

int to_num(char c)
{
	if(c >= '0' && c <= '9') return c - '0';
	return -1;
}

op_t *get_ops()
{
	//Azért inicializáljuk staticként, mivel a függvényen kívül is szeretnénk használni
	//A program futása során nem módosítjuk
	static op_t ops[] = {
		{.type = UNARY, .prc = 6, .ch = '-', .op_unary = negate},
		{.type = UNARY, .prc = 6, .ch = '+', .op_unary = nop},
		{.type = UNARY, .prc = 5, .ch = '!', .op_unary = factorial},
		{.type = BINARY, .prc = 4, .ch = '^', .op_binary = power},
		{.type = BINARY, .prc = 3, .ch = '*', .op_binary = multiply},
		{.type = BINARY, .prc = 2, .ch = '+', .op_binary = add},
		{.type = BINARY, .prc = 2, .ch = '-', .op_binary = subtract},
		{.type = LEFT_BRACKET, .prc = 1, .ch = '('},
		{.type = UNKNOWN, .prc = 0, .ch = '\0', .op_binary = NULL}
	};
	return ops;
}

bool equal(operand_t l, operand_t r)
{
	//A nullát külön kell kezelni, hisz a -0 == 0
	return (!strcmp(l.num, r.num) && (l.sign == r.sign || !strcmp(l.num, "0")))?true:false;
}

bool abs_equal(operand_t l, operand_t r)
{
	return (!strcmp(l.num, r.num))?true:false;
}

// A következőkben mindig a baloldalira ellenőrzünk (pl. itt l > r)
bool greater(operand_t l, operand_t r) {
	if(strlen_long(l.num) > strlen_long(r.num)) {
		return true;
	} else if (strlen_long(l.num) == strlen_long(r.num)) {
		uint64_t i;
		for (i = 0; l.num[i] - r.num[i] == 0 && i < strlen_long(l.num); ++i);
		if(l.num[i] - r.num[i] > 0)
			return true;
	}
	return false;
}

bool lesser(operand_t l, operand_t r)
{
	return (!equal(l, r) && !greater(l, r));
}

bool greater_equal(operand_t l, operand_t r)
{
	return !lesser(l,r);
}

bool lesser_equal(operand_t l, operand_t r)
{
	return !greater(l,r);
}

operand_t number_from_literal(bool sign, char *num)
{
	//Nullánál mindig pozitívra állítjuk az előjelet
	if(!strcmp(num, "0")) sign = false;
	//Ezt akkor kell használni, amikor ennek az értékét módosítani kell
	//Ha vmilyen számot ellenőrizni kell, hogy egyenlő-e egy literállal, akkor NE
	//ezt használjuk, memory leak
	return number(sign, strdup(num));
}

operand_t number(bool sign, char *num)
{
	operand_t n = {.sign = sign, .num = num};
	return n;
}

operand_t nop(operand_t l)
{
	//Unáris plusznál használjuk
	operand_t ret = number_from_literal(l.sign, l.num);
	return ret;
}

operand_t negate(operand_t l)
{
	operand_t ret = number_from_literal((l.sign)?false:true, l.num);
	return ret;
}

operand_t increment(operand_t l)
{
	operand_t ret = add(l, number(false, "1"));
	free(l.num);
	return ret;
}


operand_t factorial(operand_t l)
{
	bool negate = false;
	//Negatív faktoriális úgy kezeljük le, hogy a végén negáljuk
	if (l.sign) {
		negate = true;
		l.sign = false;
	}

	//Végtelen ciklus elkerülése végett hardcodeoljuk ezeket az eseteket
	if (equal(l, number(false, "0")) || equal(l, number(false, "1"))) {
		return number_from_literal(negate, "1");
	}
	operand_t ret = number_from_literal(false, "1");
	operand_t counter = number_from_literal(false, "1");
	//Mindig a számláóval szorozzuk addig, amíg nem ér el a számláló a számhoz
	//Jobb algoritmus nem létezik
	do {
		counter = increment(counter);
		operand_t temp = ret;
		ret = multiply(ret, counter);
		free(temp.num);
	} while(!abs_equal(counter, l));
	free(counter.num);
	ret.sign = negate;
	return ret;
}

operand_t power(operand_t l, operand_t r)
{
	if (r.sign == true) {
		//Ha valaminek a kitevője negatív
		return number_from_literal(false, "0");
	}
	if (equal(r, number(false, "0"))) {
		//0^0 = 1
		return number_from_literal(false, "1");
	}

	//Egyszerű hatványozás, annyiszor szorozzuk meg  számot önmagával, ahányszor kell
	operand_t ret = number_from_literal(false, "1");
	operand_t counter = number_from_literal(false, "0");
	while(!abs_equal(counter, r)) {
		operand_t temp = ret;
		ret = multiply(ret, l);
		free(temp.num);
		counter = increment(counter);
	}
	free(counter.num);
	return ret;
}

operand_t add(operand_t l, operand_t r)
{
	//-a + b
	if (l.sign && !r.sign) {
		l.sign = r.sign = false;
		return subtract(r, l);
	}
	//a + -b
	else if(!l.sign && r.sign) {
		l.sign = r.sign = false;
		return subtract(l, r);
	}
	// -a + -b =  -(a+b) vagy a+b
	else {
		bool negate = false;
		if(l.sign && r.sign) negate = true; //-a + - b

		int64_t i, counter;
		char *temp;
		MY_MALLOC(temp, sizeof(char));
		int carry = 0, k;
		//Papíron összeadás algoritmusa
		for (i = strlen_long(l.num) - 1, k = strlen_long(r.num) - 1, counter = 0;
		 	i >= 0 || k >= 0 || carry > 0; --i, --k, ++counter) {
			int add = carry;
			if (i >= 0) add += to_num(l.num[i]);
			if (k >= 0) add += to_num(r.num[k]);
			if (add >= 10) {
				carry = 1;
				add -= 10;
			} else {
				carry = 0;
			}
			MY_REALLOC(temp, (counter + 2) * sizeof(char));
			temp[counter] = add + '0';
		}
		//Fordítva kapjuk meg az eredményt, meg kell fordítani
		char *ret_num;
		MY_MALLOC(ret_num, (counter+1) * sizeof(char));
		counter--;
		uint64_t ret_counter;
		for (ret_counter = 0; counter >= 0; --counter, ++ret_counter) {
			ret_num[ret_counter] = temp[counter];
		}
		ret_num[ret_counter] = '\0';
		free(temp);
		operand_t ret;
		ret.sign = negate;
		ret.num = ret_num;
		return ret;
	}

}

operand_t subtract(operand_t l, operand_t r)
{
	if (l.sign && !r.sign) {
		//-a-+b = -a-b = -(a+b)
		r.sign = true;
		operand_t ret = add(l, r);
		return ret;
	} else if(!l.sign && r.sign) {
		//a--b = a+b
		l.sign = r.sign = false;
		operand_t ret = add(l, r);
		return ret;
	} else {
		if (l.sign && r.sign) {
			//-a--b = b-a
			operand_t temp = l;
			l = r;
			r = temp;
		}
		bool negate = false;
		if(greater(r, l)) {
			//5 - 10 = -(10 - 5)
			negate = true;
			operand_t temp = l;
			l = r;
			r = temp;
		}

		int64_t i, j, carry = 0, counter = 0;
		char *temp;
		MY_MALLOC(temp, sizeof(char));
		//Papíron kivonás algoritmusa
		for (j = strlen_long(l.num) - 1, i = strlen_long(r.num) - 1; j >= 0; --i, --j) {
			int num = to_num(l.num[j]);
			if (i >= 0) {
				if (num < to_num(r.num[i]) + carry) {
					num += 10;
					num -= (to_num(r.num[i]) + carry);
					carry = 1;
				} else {
					num -= (to_num(r.num[i]) + carry);
					carry = 0;
				}
			} else {
				num -= carry;
				carry = 0;
			}
			MY_REALLOC(temp, (++counter) * sizeof(char));
			temp[counter - 1] = num + '0';
		}
		//Itt is fordítva kapjuk meg az eredményt
		char *ret_num;
		MY_MALLOC(ret_num, sizeof(char));
		uint64_t ret_counter = 0;
		for(--counter;counter >= 0; --counter) {
			if(ret_counter != 0 || temp[counter] != '0') {
				MY_REALLOC(ret_num, (++ret_counter) * sizeof(char));
				ret_num[ret_counter - 1] = temp[counter];
			}
		}

		//Az összeadásnál ügyesen kikerültük a 0-ás eredményt, de sajnos itt elkerülhetetlen
		if(ret_counter == 0) {
			MY_REALLOC(ret_num, (++ret_counter) * sizeof(char));
			ret_num[ret_counter - 1] = '0';
		}
		MY_REALLOC(ret_num, (++ret_counter) * sizeof(char));
		ret_num[ret_counter - 1] = '\0';
		free(temp);
		operand_t ret;
		ret.sign = negate;
		ret.num = ret_num;
		return ret;
	}
}

operand_t multiply(operand_t l, operand_t r)
{
	//1 + 1 = 2 % 2 = 0, azaz - - -> +...
	bool sign = (l.sign + r.sign) % 2;
	size_t sz_left = strlen_long(l.num);
	size_t sz_right = strlen_long(r.num);
	if (sz_left < 1 || sz_right < 1 || sz_left + sz_right <= 1) {
		error("EMPTY", "Number is empty (not 0)");
	}
	//Ha valamelyik nulla, akkor az eredmény is nulla
	if (equal(l, number(false, "0")) || equal(r, number(false, "0"))) {
		return number_from_literal(false, "0");
	} else {
		uint64_t i, j;
		uint64_t num[sz_left + sz_right];
		char str[sz_left + sz_right + 1];
		//Feltöltjük adatokkal a változókat, mivel += fogounk mindeihol használni
		for (i = 1; i <= sz_left + sz_right; ++i) {
			num[i] = 0;
			str[i] = '0';
		}

		//Ha nem lépne be valamiért a ciklusba
		num[0] = 0;
		num[i] = 0;
		str[0] = '0';
		str[i] = '\0';

		//Papíron szorzás úgy, hogy először egy helyiértéken nagyobb szám is szerepelhet, mint 9
		for (i = 0; i < sz_left; ++i) {
			for (j = 0; j < sz_right; ++j) {
				num[i+j+1] += to_num(l.num[i]) * to_num(r.num[j]);
			}
		}
		//Ezután helyiértékesen eltoljuk úgy, hogy a legnagyobb számjegy a 9 legyen
		for (i = sz_left + sz_right - 1; i > 0;--i) {
			if(num[i] > 9) num[i-1]+=num[i]/10;
			str[i] += num[i]%10;
		}
		str[0] += num[0];
		char *ret_num;

		//Leszedjük az értéktelen 0-ákat az elejéről (005)
		if (str[0] == '0') {
			MY_MALLOC(ret_num, (sz_left + sz_right) * sizeof(char));
			for (i = 1; i < sz_left + sz_right; ++i) {
				ret_num[i-1] = str[i];
			}
			ret_num[i-1] = '\0';
		} else {
			MY_MALLOC(ret_num, (sz_left + sz_right + 1) * sizeof(char));
			for (i = 0; i < sz_left + sz_right; ++i) {
				ret_num[i] = str[i];
			}
			ret_num[i] = '\0';
		}
		operand_t ret;
		ret.sign = sign;
		ret.num = ret_num;
		return ret;
	}
}
