#!/usr/bin/python3

init = '''/run/initialize\n'''

file = open("autogen.mac","w+")

file.write(init);

E = 0.1
Es = []
while(E < 1000):
    Es.append(E)
    if E < 10:
        E += 0.1
        continue
    if E < 50:
        E += 1
        continue
    if E < 500:
        E += 10
        continue
    E += 25


for E in Es:
    file.write(f"/gun/energy {E} MeV\n")
    file.write("/run/beamOn 100\n")

file.close()
