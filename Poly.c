#include "Poly.h"
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#pragma warning(disable : 4244)//this warning is redundant and doesn't lead to program errors
//additional functions(prototypes)
int ParseIsNumber(const char sym);
int ParseValuer(const char* str, int size, int i);
int* ParseMemoryAdd(poly_t* polinomial, const char* str);
int ParseDegree(const char* str, int i);
int FormatSizeNum(poly_t polynomial, int i);
int FormatSizeDeg(int i);
char* FormatInsertNum(char* str, poly_t polynomial, int j, int i, int size);
char* FormatInsertDeg(char* str, int j, int size, int i);
int* AddRank(int* arr, int oldSize, int newSize);
// from Poly.h
poly_t PolyCreate(int* arr, int size) {
	poly_t polynomial;
	polynomial.factor = arr;
	polynomial.N = size - 1;
	return polynomial;
}

void PolyDestroy(poly_t* polynomial) {
	polynomial->factor = NULL;
	polynomial->N = -1;
	free(polynomial->factor);
}

poly_t PolyFromInt(int N, ...) {
	int i;
	poly_t polynomial;
	va_list arg;
	polynomial.factor = malloc(sizeof(int)*(N + 1));
	va_start(arg, N);
	if (polynomial.factor != NULL) {
		for (i = 0; i <= N; i++) {
			*(polynomial.factor + i) = va_arg(arg, int);
		}
	}
	va_end(arg);
	polynomial.N = N - 1;
	return polynomial;
}

poly_t PolyParse(const char* str) {
	poly_t polynomial;
	int i = 0, k;
	int size = 0;
	int sign = 1;
	polynomial.factor = ParseMemoryAdd(&polynomial, str);
	while (str[i] != 0) {
		if ((str[i] == '+') || (str[i] == '-')) {
			if (str[i] == '-') {
				sign = -1;
			}
			else if (str[i] == '+') {
				sign = 1;
			}
		} // ïðîâåðêà íà çíàê

		if (str[i] == 'x') {
			size = 0;
			if ((i == 0) || ((i == 1) && (sign == -1))) {
				polynomial.factor[polynomial.N - ParseDegree(str, i)] = sign * 1;
			}
			k = i;
			while ((k != 0) && ParseIsNumber(str[--k])) {
				size++;
			}
			polynomial.factor[polynomial.N - ParseDegree(str, i)] = sign * ParseValuer(str, size, i);
		}
		i++;
	}
	for (i = i - 1; !ParseIsNumber(str[i]); i--) {
	}
	if (str[i + 1] == 'x') {
		return polynomial;
	}
	for (; ParseIsNumber(str[i]); i--) {
	}
	if (str[i] == '^') {
		return polynomial;
	}
	i++;
	size = 0;
	while (ParseIsNumber(str[i]) && str[i] != 0) {
		size++;
		i++;
	}
	polynomial.factor[polynomial.N] = sign * ParseValuer(str, size, i);
	return polynomial;
}

char* PolyFormat(poly_t* polynomial) {
	int i, j, sizeNum, sizeDeg;
	char* str;
	char* tmp;
	str = (char*)malloc(sizeof(char));
	tmp = (char*)malloc(sizeof(char));
	j = 0;
	for (i = 0; i <= polynomial->N; i++) {
		sizeNum = FormatSizeNum(*polynomial, i);
		sizeDeg = FormatSizeDeg(polynomial->N - i);
		if (i == 0) {
			if (polynomial->factor[i] > 0) {

				if (polynomial->factor[i] == 1) {
					tmp = realloc(str, sizeof(char) * (j + (unsigned long long)sizeDeg + 2));
					if (tmp == NULL) {
						printf("Error: NEM");
						return 0;
					}
					else {
						tmp[j] = 'x';
						tmp[j + 1] = '^';
						str = FormatInsertDeg(tmp, j + 2, sizeDeg, polynomial->N - i);
						j += (2 + sizeDeg);
					}
				}
				else {
					tmp = realloc(str, sizeof(char) * (j + (unsigned long long)sizeNum + (unsigned long long) sizeDeg + 2));
					if (tmp == NULL) {
						printf("Error: NEM");
						return 0;
					}
					else {
						str = FormatInsertNum(tmp, *polynomial, j, i, sizeNum);
						j += sizeNum;
						tmp[j] = 'x';
						tmp[j + 1] = '^';
						str = FormatInsertDeg(tmp, j + 2, sizeDeg, polynomial->N - i);
						j += (2 + sizeDeg);
					}
				}
			}
			else {
				if (polynomial->factor[i] == -1) {
					tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3 + (unsigned long long)sizeDeg));
					if (tmp == NULL) {
						printf("Error: NEM");
						return 0;
					}
					else {
						tmp[j] = '-';
						*(tmp + j + 1) = 'x';
						*(tmp + j + 2) = '^';
						str = FormatInsertDeg(tmp, j + 3, sizeDeg, polynomial->N - i);
						j += (3 + sizeDeg);
					}
				}
				else {
					tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3 + (unsigned long long) sizeNum + (unsigned long long)sizeDeg));
					if (tmp == NULL) {
						printf("Error: NEM");
						return 0;
					}
					else {
						tmp[j] = '-';
						str = FormatInsertNum(tmp, *polynomial, j + 1, i, sizeNum);
						j += (1 + sizeNum);
						tmp[j] = 'x';
						tmp[j + 1] = '^';
						str = FormatInsertDeg(tmp, j + 2, sizeDeg, polynomial->N - i);
						j += (2 + sizeDeg);
					}
				}
			}

		}
		else if (i == polynomial->N - 1) {
			if (polynomial->factor[i] > 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				else {
					tmp[j] = ' ';
					tmp[j + 1] = '+';
					tmp[j + 2] = ' ';
					j += 3;
					str = tmp;
				}
			}
			if (polynomial->factor[i] < 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				tmp[j] = ' ';
				tmp[j + 1] = '-';
				tmp[j + 2] = ' ';
				j += 3;
				str = tmp;
			}
			if (abs(polynomial->factor[i]) == 1) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 1));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				else {
					tmp[j] = 'x';
					j += 1;
					str = tmp;
				}
			}
			else if (polynomial->factor[i] != 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + sizeNum + 1));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				else {
					str = FormatInsertNum(tmp, *polynomial, j, i, sizeNum);
					j += sizeNum;
					tmp[j] = 'x';
					j += 1;
					str = tmp;
				}
			}

		}
		else if (i == polynomial->N) {
			if (polynomial->factor[i] > 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				tmp[j] = ' ';
				tmp[j + 1] = '+';
				tmp[j + 2] = ' ';
				j += 3;
				str = tmp;
			}
			if (polynomial->factor[i] < 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				tmp[j] = ' ';
				tmp[j + 1] = '-';
				tmp[j + 2] = ' ';
				j += 3;
				str = tmp;
			}
			else if (polynomial->factor[i] != 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + (unsigned long long)sizeNum + 1));
				str = FormatInsertNum(tmp, *polynomial, j, i, sizeNum);
				j += sizeNum + 1;
			}
		}
		// GENERAL SITUATION
		else if (i != 0 && i != polynomial->N && i != polynomial->N - 1) {

			if (polynomial->factor[i] > 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				tmp[j] = ' ';
				tmp[j + 1] = '+';
				tmp[j + 2] = ' ';
				j += 3;
				str = tmp;
			}
			if (polynomial->factor[i] < 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + 3));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				tmp[j] = ' ';
				tmp[j + 1] = '-';
				tmp[j + 2] = ' ';
				j += 3;
				str = tmp;
			}
			if (abs(polynomial->factor[i]) == 1) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + (unsigned long long)sizeDeg + 2));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				else {
					tmp[j] = 'x';
					tmp[j + 1] = '^';
					str = FormatInsertDeg(tmp, j + 2, sizeDeg, polynomial->N - i);
					j += (2 + sizeDeg);
				}
			}
			else if (polynomial->factor[i] != 0) {
				tmp = realloc(str, sizeof(char) * ((unsigned long long)j + (unsigned long long)sizeNum + (unsigned long long)sizeDeg + 2));
				if (tmp == NULL) {
					printf("Error: NEM");
					return 0;
				}
				else {
					str = FormatInsertNum(tmp, *polynomial, j, i, sizeNum);
					j += sizeNum;
					tmp[j] = 'x';
					tmp[j + 1] = '^';
					str = FormatInsertDeg(tmp, j + 2, sizeDeg, polynomial->N - i);
					j += (2 + sizeDeg);
				}
			}
		}
	}
	str[j - 1] = '\0';
	return str;
}

poly_t PolyRead(FILE* file) {
	poly_t polynomial;
	int i, j;
	char* tmp;
	char sym;
	char* str;
	j = 0;
	polynomial.N = 0;
	str = (char*)malloc(sizeof(char));
	tmp = (char*)malloc(sizeof(char));
	sym = fgetc(file);

	if (tmp != NULL) {
		for (i = 0; sym != EOF; i++) {
			tmp = (char*)realloc(str, (sizeof(char) * ((unsigned long long)i + 1)));
			if (tmp != NULL) {
				*(tmp + i) = sym;
				str = tmp;
			}
			sym = fgetc(file);
		}
	}
	polynomial = PolyParse(str);

	tmp = NULL;
	str = NULL;

	free(tmp);
	free(str);

	return polynomial;
}

void PolyWrite(poly_t polynomial, FILE* fp) {
	char* str;
	str = PolyFormat(&polynomial);
	for (; *str; str++) putc(*str, fp);
}

poly_t PolyAdd(poly_t poly1, poly_t poly2) {
	poly_t polynomial;
	int max, i;
	max = poly1.N;
	if (poly1.N > poly2.N) {
		max = poly1.N;
		poly2.factor = AddRank(poly2.factor, poly2.N, max);
	}
	else if (poly2.N > poly1.N) {
		max = poly2.N;
		poly1.factor = AddRank(poly1.factor, poly1.N, max);
	}
	polynomial.factor = malloc(sizeof(unsigned long long) * ((unsigned long long)max + 1));
	if (polynomial.factor == NULL) {
		printf("Error : NEM");
	}
	else {
		for (i = 0; i <= max; i++) {
			*(polynomial.factor + i) = poly1.factor[i] + poly2.factor[i];
		}
		polynomial.N = max;
	}
	return polynomial;
}

poly_t PolySub(poly_t poly1, poly_t poly2) {
	poly_t polynomial;
	int max, i;
	max = poly1.N;
	if (poly1.N > poly2.N) {
		max = poly1.N;
		poly2.factor = AddRank(poly2.factor, poly2.N, max);
	}
	else if (poly2.N > poly1.N) {
		max = poly2.N;
		poly1.factor = AddRank(poly1.factor, poly1.N, max);
	}
	polynomial.factor = malloc(sizeof(unsigned long long) * ((unsigned long long)max + 1));
	if (polynomial.factor == NULL) {
		printf("Error : NEM");
	}
	else {
		for (i = 0; i <= max; i++) {
			*(polynomial.factor + i) = poly1.factor[i] - poly2.factor[i];
		}
		polynomial.N = max;
	}
	return polynomial;
}

poly_t PolyMul(poly_t poly1, poly_t poly2) {
	poly_t polynomial;
	int max, i, j;
	max = poly1.N + poly2.N;
	polynomial.factor = malloc(sizeof(unsigned long long) * ((unsigned long long)max + 1));
	if (polynomial.factor == NULL) {
		printf("Error : NEM");
	}
	else {
		for (i = 0; i <= max; i++) {
			*(polynomial.factor + i) = 0;
		}
		for (i = 0; i <= poly1.N; i++) {
			for (j = 0; j <= poly2.N; j++) {
				*(polynomial.factor + i + j) += poly1.factor[i] * poly2.factor[j];
			}
		}
		polynomial.N = max;
	}
	return polynomial;
}

poly_t PolyMulInt(poly_t polynomial, int num) {
	int i;
	for (i = 0; i <= polynomial.N; i++) {
		polynomial.factor[i] *= num;
	}
	return polynomial;
}

int PolyEvaluate(poly_t polynomial, int x) {
	int i, res;
	res = 0;
	for (i = 0; i <= polynomial.N; i++) {
		res += polynomial.factor[i] * pow(x, (unsigned long long)polynomial.N - i);
	}
	return res;
}

int PolyElement(poly_t polynomial, int i) {
	return polynomial.factor[i];
}

//additional functions
int* AddRank(int* arr, int oldSize, int newSize) {
	int* tmp1;
	int tmp2;
	int i;
	tmp1 = arr;
	for (i = 0; i < (oldSize + 1) / 2; i++) {
		tmp2 = tmp1[i];
		tmp1[i] = tmp1[oldSize - i];
		tmp1[oldSize - i] = tmp2;
	}
	arr = tmp1;
	tmp1 = realloc(arr, sizeof(unsigned long long) * ((unsigned long long)newSize + 1));
	if (tmp1 == NULL) {
		printf("Error : NEM");
		return 0;
	}
	else {
		for (i = oldSize + 1; i <= newSize; i++) {
			tmp1[i] = 0;
		}
		i = 0;
		for (i = 0; i < (newSize + 1) / 2; i++) {
			tmp2 = *(tmp1 + i);
			*(tmp1 + i) = *(tmp1 + newSize - i);
			*(tmp1 + newSize - i) = tmp2;
		}
	}
	return tmp1;
}

int ParseIsNumber(const char sym) {
	if (sym>='0'&&sym<='9') {
		return 1;
	}
	return 0;
}

int ParseValuer(const char* str, int size, int i) {
	int delta;
	int value = 0;
	if (size == 0) {
		value = 1;
		return value;
	}
	for (delta = i - size; delta < i; delta++) {
		if (ParseIsNumber(str[delta])) {
			value += ((int)str[delta] - 48) * (int)pow(10, (--size));
		}
	}

	return value;
}

int ParseDegree(const char* str, int i) {
	int size = 0;
	if (!ParseIsNumber(str[i + 2])) {
		return 1;
	}
	i = i + 2;
	while (str[i] != 0 && ParseIsNumber(str[i])) {
		size++;
		i++;
	}
	return ParseValuer(str, size, i);
}

int* ParseMemoryAdd(poly_t* polynomial, const char* str) {
	int i = 0;
	int* tmp;
	int size = 0;
	while ((str[i] != 'x') && (str[i] != 0)) {
		i++;
	}
	if (str[i] == 0) {
		polynomial->N = 0;
	}
	else {
		polynomial->N = ParseDegree(str, i);
	}
	tmp = (int*)malloc(sizeof(int) * ((unsigned long long)(polynomial->N) + 1));
	if (tmp == NULL) {
		printf("ERROR : NEM");
		return 0;
	}
	for (i = 0; i <= polynomial->N; i++) {
		*(tmp + i) = 0;
	}
	return tmp;
}

int FormatSizeNum(poly_t polynomial, int i) {
	int size = 1;
	int tmp;
	tmp = polynomial.factor[i];
	while ((tmp /= 10) != 0) {
		size++;
	}
	return size;
}

int FormatSizeDeg(int i) {
	int size = 1;
	while ((i /= 10) != 0) {
		size++;
	}
	return size;
}

char* FormatInsertNum(char* str, poly_t polynomial, int j, int i, int size) {
	int tmp, k;
	int num;
	tmp = size;
	for (k = j + size - 1; k >= j; k--) {
		if (k == j + size - 1) {
			num = abs(polynomial.factor[i]) % (int)pow(10, (unsigned long long)size - (--tmp));
		}
		else {
			num = (abs(polynomial.factor[i]) % (int)pow(10, (unsigned long long)size - (--tmp)) - abs(polynomial.factor[i]) % (int)pow(10, (unsigned long long)size - ((unsigned long long)tmp + 1))) / (int)pow(10, (unsigned long long)size - ((unsigned long long)tmp + 1));
		}
		str[k] = (char)(num + 48);
	}
	return str;
}

char* FormatInsertDeg(char* str, int j, int size, int i) {
	int tmp, k;
	int num;
	tmp = size;
	for (k = j + size - 1; k >= j; k--) {
		if (k == j + size - 1) {
			num = i % (int)pow(10, (unsigned long long)size - (--tmp));
		}
		else {
			num = (i % (int)pow(10, (unsigned long long)size - (--tmp)) - i % (int)pow(10, (unsigned long long)size - ((unsigned long long)tmp + 1))) / (int)pow(10, (unsigned long long)size - ((unsigned long long)tmp + 1));
		}
		str[k] = num + 48;
	}
	return str;
}
