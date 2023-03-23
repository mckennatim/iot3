
int readSenseI2c(const int *port, int ix);
int readSwitch(const int *port, int ix);
int readButton(const int *port, int ix);
  bool debounce(int port);

void i_updInputs();
  void invokeInp(int ix, int sen, int (*readSense)(const int *port, int sen));
  int getOldReading(int sr, int da);