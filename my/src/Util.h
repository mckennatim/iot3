#ifndef butil_h
#define butil_h

/*util*/
void u_scanFLAGand(int FLAG, int LEN, void (*doStuff)(int, int));
void doStuff(int i,int b);
int pow2(int e);
void bitShift(int bnum, int LEN, void (*doStuff)(int, int));

void u_setFlag(int sr, int *FLAG);
void u_unsetFlag(int sr, int *FLAG);
int u_getFlag(int sr, int FLAG);

void u_printFlag(int FLAG);
void u_printSrs();
void u_printPrgs();
int u_getNumdl(int sr);

#endif