// LCD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string.h>
using namespace std;

// kich thuoc man hinh lcd
int l = 16, w = 2;

// toa do con tro
int px = 11; int py = 3;
enum {YEAR, MONTH, DAY, HOUR, MINUTE, SECOND};


#define MICROS_CYCLE_LENGHT     100
void one_micros() {
    for (int i = 0; i < MICROS_CYCLE_LENGHT; i++) {}
}
void one_milisecond() {
    for (int i = 0; i < 1000; i++) {
        one_micros();
    }
}
void delay(int ms) {
    while (ms--) {
        one_milisecond();
    }
}

// So ngay trong thang
int days_of_month(int year, int month) {
    switch (month)
    {
    case 2: return (year & 3) ? 28 : 29;
    case 4: case 6: case 9: case 11: return 30;
    default: return 31;
    break;
    }
}

// In ra 2 chu so
void print_2digit(int n, char k) {
    if (n < 10) cout << '0' << n << k;
    else cout << n << k;   
}

// In ra 1 hang
void row(int left, int mid, int right) {
    cout << char(left);
    for (int i = 0; i < l; i++) {
        cout << char(mid);
    }
    cout << char(right);
    cout << endl;
}

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
// Hien thi con tro
void showCursor(bool show) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = show; // an con tro
    SetConsoleCursorInfo(console, &cursorInfo);
}
// Vi tri con tro
void setCursorPosition(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(console, pos);
    showCursor(false);
}

// Tang thoi gian tung giay mot
void increaseTime(int time[], int index) {
    int max[6] = { 10000, 12, days_of_month(time[YEAR], time[MONTH]), 24, 60, 60 };
    time[index]++;
    if (index >= HOUR) {
        if (time[index] == max[index]) {
            time[index] = 0;
            increaseTime(time, index - 1);
        }
    }
    else {
        if (time[index] == max[index] + 1) {
            time[index] = 1;
            increaseTime(time, index - 1);
        }
    }
}

// Man hinh LCD
void drawLCD() {
    setCursorPosition(px, py);
    row(201, 205, 187);
    for (int y = 0; y < w; y++) {
        setCursorPosition(px, py + y + 1);
        cout << char(186);
        for (int x = 0; x < l; x++) {
            cout << char(32);
        }
        cout << char(186);
    }
    setCursorPosition(px, py + w + 1);
    row(200, 205, 188);
}

void Screen_Lv1(int time[]) {
    for (int i = DAY; i > YEAR; i--) {
        print_2digit(time[i], '/');
    }

    if (time[YEAR]< 1000) {
        cout << '0';
        if (time[YEAR] < 100) cout << '0';
    }
    print_2digit(time[YEAR], ' ');

    for (int i = HOUR; i <= SECOND; i++) {
        print_2digit(time[i], (i == SECOND) ? '\0' : ':');

    }
}

void Screen_Lv2(int time[]) {

    setCursorPosition(px + 1, py + 1);
    for (int i = DAY; i > YEAR; i--) {
        print_2digit(time[i], '/');
    }

    if (time[YEAR] < 1000) {
        cout << '0';
        if (time[YEAR] < 100) cout << '0';
    }
    print_2digit(time[YEAR], ' ');

    setCursorPosition(px + 1, py + 2);
    for (int i = HOUR; i <= SECOND; i++) {
        print_2digit(time[i], (i == SECOND) ? '\0' : ':');
    }
}

const string c[6] = { "Year: ", "Month: ", "Day: ", "Hour: ", "Minute: ", "Second: " };
// Nhap thong tin(Lv2)
void inputTime(int& n, string c) {
    setCursorPosition(px, py + w + 2);
    cout << c;
    cin >> n;
}


// Xau ky tu thanh so nguyen 
void atoIn(char s[], int& i, int& n) {
    n = 0;
    i++;
    while (s[i] >= '0' && s[i] <= '9') {
        n = n * 10 + int(s[i] - '0');
        i++;
    }
}
/// <summary>
/// Lay cac so nguyen tu xau nhap vao DATE(y,m,d) TIME(h,m,s) (lv3)
/// </summary>
/// <param name="s"> Xau dau vao </param>
/// <param name="a"> Mang chua cac so nguyen can lay </param>
void stringToInt(char s[], int* a) {
    for (int i = 0; char d = s[i]; i++) {
        if (s[i] == '(') {
            atoIn(s, i, *a);
            atoIn(s, i, *(++a));
            atoIn(s, i, *(++a));
        }
    }
}


//// lay chuoi tu mang char
//string convertToString(char* a, int size)
//{
//    string s = "";
//    for (int i = 0; i < size; i++) {
//        s = s + a[i];
//    }
//    return s;
//}

void Lv1(int time[]) {
    while (1) {
        Screen_Lv1(time);
        cout << endl;
        delay(500);
        increaseTime(time, 5);
    }
}

void Lv2(int time[]) {
    drawLCD();
    setCursorPosition(px, py + w + 2);
    cout << "Press ESC to setup";

    while (1) {
        int key = GetAsyncKeyState(VK_ESCAPE);
        Screen_Lv2(time);
        cout << endl;
        increaseTime(time, 5);
        
        if (key) {
            system("cls");
            drawLCD();
            Screen_Lv2(time);
            for (int i = YEAR; i <= SECOND; i++) {
                inputTime(time[i], c[i]);
                system("cls");
                drawLCD();
                Screen_Lv2(time);
            }
            Lv2(time);
        }
        delay(500);
    }

}

void Lv3(int time[]) {
    drawLCD();
    setCursorPosition(px, py + w + 2);
    cout << "Press ESC to setup";

    while (1) {
        int key = GetAsyncKeyState(VK_ESCAPE);
        Screen_Lv2(time);
        increaseTime(time, 5);
        delay(100);

        if (key) {
            setCursorPosition(px, py + w + 2);
            cout << "Enter DATE(y,m,d) or TIME(h,m,s) ";
            setCursorPosition(px, py + w + 3);
            showCursor(true);

            char s[100];
            cin.getline(s, 100);
            // Xac dinh thong tin DATE or TIME
            if (*s == 'D') {
                stringToInt(s, &time[YEAR]);
                system("cls");
            }
            if (*s == 'T') {
                stringToInt(s, &time[HOUR]);
                system("cls");
            }

            Lv3(time);
        }
    }
}

int main()
{
    int time[6] = {1, 1, 1, 0, 0, 0};
    cout << "Enter program level (1 - 3): ";
    int level;
    cin >> level;
    system("cls");
    switch (level) {
    case 1: Lv1(time); break;
    case 2: Lv2(time); break;
    case 3: Lv3(time); break;
    }
}





















// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
