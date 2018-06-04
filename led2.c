#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<wiringPi.h>

#define MAX_CONTENT_LENGTH 16

const int Led[16] = {
		4, 17, 18, 27, 22, 23, 24, 25
};

void getData(char **data) {
		char *queryString, *delim;
		int len = 0;
		int i;

		queryString = getenv("QUERY_STRING");
		delim = strchr(queryString, '=');
		for(i=0; i<sizeof(delim); i++) {
				delim[i] = delim[i+1];
		}

		delim[sizeof(delim)] = '\0';

		if(delim) {
				len = strlen(delim);
				*data = malloc(sizeof(char) *len);
				strcpy(*data, delim);
		}

		else {
				len = 1;
				*data = malloc(sizeof(char) *len);
				**data = '\0';
		}
}

void Setup() {
		int i;
		if(wiringPiSetupGpio() == -1) {
				exit(-1);
		}

		for(i=0; i<16; i++) {
				pinMode(Led[i], OUTPUT);
				digitalWrite(Led[i], LOW);
		}
}

void LedTest(char *data) {
		int i;
		int num = atoi(data);
		for(i=0; i<4; i++) {
				digitalWrite(Led[i], ((num >> i)%2));
		}
}

int main() {
		printf("Content-type:text/html\n\n");
		printf("<html>\n<head>\n<title>LED2 TEST!</title>\n</head>\n");
		printf("<body>\n<p>LED BLINK</p>\n");

		char *data = (char *)0;
		getData(&data);
		Setup();
		LedTest(data);

		printf("</body>\n</html>\n");

		return 0;
}
