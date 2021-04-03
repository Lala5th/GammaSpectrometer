#!/usr/bin/python3
import numpy as np

file = open("autogen.mac","w+")

file.write(f"/mangling/setEC 0\n")
for E in range(10,200,20):
    file.write(f"/gun/energy {E} MeV\n")
    for std in range(0,200,40):
        file.write(f"/mangling/setStd {std}\n")
        file.write("/run/beamOn 8\n")

file.close()
