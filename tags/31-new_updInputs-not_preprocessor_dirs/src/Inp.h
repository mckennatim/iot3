
int readSenseDht(int ix, int re);
int readSenseI2c(int ix, int re);
int readSenseSpi(int ix, int re);
int readButton(int ix, int re);
int readDs18b20(int ix, int re);
  int debounce(int port);

void i_updInputs();
  int getOldReading(int sr, int da);

void i_updCtrl(int sr, int x);  

void i_butnLoop();