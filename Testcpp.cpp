#include<iostream>
#include<Windows.h>
#include<math.h>
using namespace std;
int string_to_number(char s[]) {
	
	int n = 0;
	for (char* p = s; *p; p++) {
		n *= 10;
		n += ((*p) - '0');
	}
	return n;
}
void stringToInt(char s[], int& a, int& b, int& c) 
{
	a = 0; b = 0; c = 0;
	for (char* p = s; *p; p++) {
		if (*(p - 1) == '(') {
			for (p; *p != ','; p++) {
				a *= 10;
				a += ((*p) - '0');
			}
			p++;
			for (p; *p != ','; p++) {
				b *= 10;
				b += ((*p) - '0');
			}
			p++;
			for (p; *p != ')'; p++) {
				c *= 10;
				c += ((*p) - '0');
			}

		}
	}
}
int main() {
	char s[100];
	cin.getline(s, 100);
	int a, b, c;
	//cout << string_to_number(s);
	stringToInt(s, a, b, c);
	cout << a << endl << b << endl << c;
}
