#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>

const int Led[8] = {
	4, 17, 18, 27, 22, 23, 24, 25
};

void Setup() {
		int i;
		if(wiringPiSetupGpio() == -1) {
				exit(-1);
		}

		for(i=0; i<8; i++) {
				pinMode(Led[i], OUTPUT);
				digitalWrite(Led[i], LOW);
		}
}

void LedTest() {
		int i;
		for(i=0; i<8; i++) {
				digitalWrite(Led[i], HIGH);
		}
		delay(500);

		for(i=0; i<8; i++) {
				digitalWrite(Led[i], LOW);
		}

		delay(500);
}

int main() {
		printf("Content-type:text/html\n\n");
		printf("<html>\n<head>\n<title>LED TEST!</title>\n</head>\n");
		printf("<body>\n<p>LED BLINK</p>\n");

		int i;

		Setup();

		for(i=0; i<5; i++) {
				LedTest();
		}

		printf("</body>\n</html>\n");

		return 0;
}
