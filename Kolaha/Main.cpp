#include "field.h"
using namespace std;

int main() {
	game gm;
	uint16_t pl1, pl2;
	do {
		cout << "Choose first player: ";
		cin >> pl1;
		cout << "Choose second player: ";
		cin >> pl2;
		cout << endl;
	} while (pl1 > 5 || pl2 > 5);
	int tmp = gm.start(pl1, pl2);
	/*int i;
	int win_0 = 0, win_1 = 0;
	for (i = 0; i < 100; i++) {
		int tmp = gm.start(2, 2);
		if (tmp == 0) win_0++;
		if (tmp == 1) win_1++;
	}
	cout << endl << endl << win_0 << " || " << win_1 << endl << float(win_0)/50 << endl;*/
	system("PAUSE");
}