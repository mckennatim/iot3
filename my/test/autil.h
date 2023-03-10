#ifndef autil_h
#define autil_h

int getDnIdx(int typidx, const char* dname);
int getStoredValue(int sr, const char* type, const char* dname);
bool isThisXdata(char* idev);
void devOfDevTpc(char* dt, char* d);
int whichDev(char* idev);
int getXsr(int osr, int xd);
int getTdsIdx(int xrs);

#endif