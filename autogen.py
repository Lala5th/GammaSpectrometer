#!/usr/bin/python3

init = '''/run/initialize\n'''

file = open("autogen.mac","w+")

file.write(init);

for E in range(10,1010,10):
    file.write(f"/gun/energy {E} MeV\n")
    file.write("/run/beamOn 8\n")

file.close()
