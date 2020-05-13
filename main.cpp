#include "Program.h"
#include "GUI.h"
#include <stdint.h>

bool checkPeppr(string key, string toTEST)
{
	SHA256 sha256;

	uint8_t v = 0;
	while (true) {
		cout << toTEST + (char)v << endl;

		string toSHA256 = toTEST + (char)v;

		toSHA256 = sha256(toSHA256);

		if (key.compare(toSHA256) == 0) return true;

		if (v == 1) break;
		else v++;
	}

	/*
	for (uint8_t v = 0; v < 256; v++) {
		
		cout << toTEST + (char)v << "\t";

		string toSHA256 = sha256(toTEST + (char)v);

		if (key.compare(toSHA256) == 0) return true;
	}*/

	return false;
}

bool check(string key, string pw) {
	SHA256 sha256;
	uint8_t salt[3] = { 0x4E , 0x48 ,0x54 };

	for (int i = 0; i < pw.length(); i++) {
		string toTEST = pw;
		toTEST[i] = toTEST[i] ^ salt[0];

		for (int j = 0; j < pw.length(); j++) {
			toTEST[j] = toTEST[j] ^ salt[1];

			for (int u = 0; u < pw.length(); u++) {
				toTEST[u] = toTEST[u] ^ salt[2];

				cout << i << " " << j << " " << u << "\t";

				if (checkPeppr(key, toTEST)) return true;
				/*
				for (uint16_t v = 0; v < 256; v++) {
					string toSHA256 = sha256(toTEST + (char)v);

					//if (key.compare(toSHA256) == 0) return true;
				}*/
			}
		}
	}

	return false;
}

int main() {
	srand((int)time(0));
	/*Program fileSystem;
	fileSystem.run();*/

	SHA256 sha256;
	SHA256 hash1;

	string x;
	cin >> x;

	uint8_t v = 1;

	string val = addSalt(x);
	val += (char)v;

	cout << val << endl;

	val = sha256(val);


	system("pause");
	cout << check(val, x);

	return 0;
}

