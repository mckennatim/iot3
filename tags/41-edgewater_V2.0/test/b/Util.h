#ifndef butil_h
#define butil_h

/*inputs*/
void i_updInputs();
  void invokeInp(int h, int i, int (*readSense)(const int *port, int i));
  int getOldReading(int sr, int da);
    int readSense(const int *port, int ix);
    int readSenseI2c(const int *port, int ix);
    int readSwitch(const int *port, int ix);
    int readButton(const int *port, int ix);
      bool debounce(int port);
void setXdata(char* idev, int sr, int darr[]);
  int whichDev(char* idev);
  int getXdataIdx(int dev, int sr);
  int getTypIdx(const char* type);

/*contols*/
void updCtrl(int sr, int x);
// void scanFLAGand(int FLAG, int LEN, void (*updCtrl)(int, int));

/*sched*/
// void scanFLAGand(int FLAG, int LEN, void (*updCtrl)(int, int));
void ckAlarms(int sr, int b);
  int getPrgIdx(int sr);
  void findCurNxt(int idx, int &curr, int &nxt, int &tleft);
// void scanFLAGand(int FLAG, int LEN, void (*updTimers)(int, int));
void updTimers(int sr, int b);

/*req*/
void cmdMsg(int sr, int sra[]);
void prgMsg(int sr, int ev, int pro[][3]);

/*util*/
void scanFLAGand(int FLAG, int LEN, void (*doStuff)(int, int));
void scanFLAGn();
int pow2(int e);
void setFlag(int sr, int *FLAG);
void unsetFlag(int sr, int *FLAG);
bool getFlag(int sr, int FLAG);
void printFlag(int FLAG);
void printSrs();
void printPrgs();
void doStuff(int i,int b);
void doSt(int sr, int *FLAG);
void bitShift(int bnum, int LEN, void (*doStuff)(int, int));

int u_getNda(int sr);

/*mock*/
void pubState(int sr);
void pubTimr(int sr);
int digitalRead(int port);
void digitalWrite(int port, int onoff);
unsigned long int millis(int max, int min);
int hour();
int minute();


#endif