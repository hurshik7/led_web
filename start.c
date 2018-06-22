#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>

const char* fifo = "/home/pi/termproject/myfifo";

const int FndSelectPin[6] = {4, 17, 18, 27, 22, 23};
const int FndPin[8] = {6, 12, 13, 16, 19, 20, 26, 21};
const int FndFont[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 
							0x07, 0x7F, 0x67, 0x80};
static char mode = 's';

void Init()
{
		printf("init start\n");
		int i;
		if(wiringPiSetupGpio() == -1)
		{
				printf("WiringPiSetupGpio() error\n");
				exit(-1);
		}

		for(i=0; i<6; i++){
				pinMode(FndSelectPin[i], OUTPUT);
				digitalWrite(FndSelectPin[i], HIGH);
		}

		for(i=0; i<8; i++) {
				pinMode(FndPin[i], OUTPUT);
				digitalWrite(FndPin[i], LOW);
		}
}

void FndSelect(int position)
{
		int i;
		for(i=0; i<6; i++)
		{
				if(i==position)
						digitalWrite(FndSelectPin[i], LOW);
				else
						digitalWrite(FndSelectPin[i], HIGH);
		}
}

static void FndDisplay(int position, int num)
{
		int i,j;
		int flag = 0;
		int shift = 0x01;

		FndSelect(position);
		for(i=0; i<8; i++)
		{
				flag = (FndFont[num] & shift);
				digitalWrite(FndPin[i], flag);
				shift <<= 1;
		}
}

void* stopwatchFunc(void* arg){
		printf("stop watch thread start\n");
		
		int t, i, pos;
		printf("step1\n");
		Init();
		printf("init success!\n");
		int num_00, num_0, num0_, num00_, num000_;
		num_00 = 0 ; num_0 = 0; num0_ = 0; num00_ = 0; num000_ = 0;
		int data[6] = {0, 0, 10, 0, 0, 0};
		
		while(1) {
		printf(&mode);
		printf("\n");
		if(mode == 's') {
				data[0] = num_00 % 10;
				data[1] = num_0 % 10;
				data[3] = num0_ % 10;
				data[4] = num00_ % 10;
				data[5] = num000_ % 10;
			
				for(pos=0; pos<6; pos++){
					FndDisplay(pos, data[pos]);
					delay(1);
				}
				num_00++;
				num_0 = num_00/10 ;
				num0_ = num_0/10;
				num00_ = num0_/10;
				num000_ = num00_/10;
				delay(4);
		}
		else if(mode == 'p') {
				for(pos=0; pos<6; pos++) {
						FndDisplay(pos, data[pos]);
						delay(1);
				}
		}
		else if(mode == 'c') {
			exit(100);
		}
		}
		return (void*)100;
}

void ipcProcess(){
		int fd;
		char buff;
		printf("ipc process  starts!\n");
//		int state;
//		state = mkfifo(fifo, S_IRUSR|S_IWUSR);
//		if(state < 0) {
//				printf("making fifo failure\n");
//				exit(1);
//		}
//		else
//			printf("make fifo success\n");

		while(mode == 's') {
			fd = open(fifo, O_RDONLY | O_NONBLOCK);
			if(fd < 0) {
					printf("fifo open failure\n");
					exit(1);
			}
			else
					printf("fifo open success\n");

			read(fd, &buff, sizeof(buff));
			close(fd);
			printf("buff read : ");
			printf(&buff);
			printf("\n");
			if(buff == 'p') {
				mode = 'p';
				break;
			}
			else if(buff == 'c') {
					mode = 'c';
					break;
			}
			else
					mode = 's';
		}
}

int main() {
	pid_t pid;
	pthread_t p_thread1;
	pthread_t thr_id;
	int status;
	pid = fork();
	if(pid == 0) {
//			stopwatchFunc();
			thr_id = pthread_create(&p_thread1, NULL, stopwatchFunc, NULL);
			if(thr_id) {
					printf("Thread create fail\n");
			}
			else
					printf("Thread create success\n");
	}

	else if(pid>0) {
		pthread_join(p_thread1, (void *)&status);
		printf("thread end %d\n", status);
		return 0;
//		printf("parent process starts!\n");
		//ipcProcess();
//		int state;
//		state = mkfifo(FIFO_FILE, 0666);
//		if(state < 0) {
//				printf("making fifo failure\n");
//				return 1;
//		}
//		printf("make fifo success\n");
	}
	
	return 0;
}
