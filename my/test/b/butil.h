#ifndef butil_h
#define butil_h

int getDnIdx(int typidx, const char* dname);
int getStoredValue(int sr, const char* type, const char* dname);
bool isThisXdata(char* idev);
void devOfDevTpc(char* dt, char* d);
int whichDev(char* idev);
int getXsr(int osr, int xd);
int getTypIdx(const char* type);
int getXdataIdx(int dev, int sr);
void setXdata(char* idev, int sr, int darr[]);
void printXdata();

/*inputs*/
void updInputs();
int readSense(int port, int inpidx);
int readSenseI2c(int port, int inpidx);
int readSwitch(int port, int inpidx);
int readButton(int port, int inpidx);
void invokeInp(int h, int i, int (*readSense)(int port, int i));
bool debounce(int port);
int getOldReading(int sr, int da);

/*contols*/
void updCtrl(int sr, int x);
void scanFLAGand(int FLAG, int LEN, void (*updCtrl)(int, int));

/*util*/
int pow2(int e);
void setFlag(int sr, int *FLAG);
void unsetFlag(int sr, int *FLAG);
void printFlag(int FLAG);
void printSrs();
void doStuff(int i,int b);
void bitShift(int bnum, int LEN, void (*doStuff)(int, int));

/*mock*/
int digitalRead(int port);
void digitalWrite(int port, int onoff);
unsigned long int millis(int max, int min);



#endif