#ifndef butil_h
#define butil_h

int getDnIdx(int typidx, const char* dname);
int getStoredValue(int sr, const char* type, const char* dname);
bool isThisXdata(char* idev);
void devOfDevTpc(char* dt, char* d);
int whichDev(char* idev);
int getXsr(int osr, int xd);
int getTdsIdx(int xrs);
int getXdataIdx(int xsr);
void setXdata(char* idev, int sr, int darr[]);
void printXdata();
void doStuff(int i,int b);
void bitShift(int bnum, int LEN, void (*doStuff)(int, int));
void updSensors();
int getOldReading(int sr, int da);
void printSrs();
int readSense(int port);
int readSenseI2c(int port);
int readSwitch(int port);
int readButton(int port);
void invokeInp(int h, int i, int (*readSense)(int port, int h));
int pow2(int e);
void setFlag(int sr, int *FLAG);
void unsetFlag(int sr, int *FLAG);
void printFlag(int FLAG);
int digitalRead(int port);
void digitalWrite(int port, int onoff);
void updCtrl(int sr, int x);
void scanFLAGand(int FLAG, int LEN, void (*doStuff)(int, int));
bool debounce(int port);
unsigned long int millis(int max, int min);

#endif