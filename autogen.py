#!/usr/bin/python3
import numpy as np

file = open("autogen.mac","w+")

for EC in np.arange(-0.4,0.4,0.1):
    file.write(f"/mangling/setEC {EC}\n")
    for std in range(0,50,5):
        file.write(f"/mangling/setStd {std}\n")
        file.write("/run/beamOn 8\n")

file.close()
