#!/bin/bash
gcc -Imnt/e/fs/iot3/my/test -I/mnt/e/fs/iot3/my/.pio/libdeps/d1_mini/ArduinoJson/src -o amain amain.cpp aconf.cpp autil.cpp && ./amain