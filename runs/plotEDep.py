import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm

args = sys.argv
cmap = cm.get_cmap('gnuplot')

dir = args[1]

E = np.load(dir + '/InitialEnergy.npy')['0']
y = np.load(dir + '/y.npy')['0']
z = np.load(dir + '/z.npy')['0']
data = np.reshape(np.load(dir + '/DepositedEnergy.npy'),(len(E),len(y),len(z)))

norm = colors.Normalize(vmin=0,vmax=max(E))

plt.ion()
plt.figure()
[plt.plot(z,np.sum(data['1'][i,:,:],axis = 0),color=cmap(norm(E[i]))) for i in range(len(E))]
plt.colorbar(cm.ScalarMappable(norm=norm, cmap=cmap)).set_label('Initial Energy [MeV]')
plt.ylabel('Deposited Energy [MeV]')
plt.xlabel('Longitudinal crystal ID')
plt.show()
