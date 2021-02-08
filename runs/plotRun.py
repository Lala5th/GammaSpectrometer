import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cm
from matplotlib.widgets import Slider

args = sys.argv
cmap = cm.get_cmap('gnuplot')

dir = args[1]

E = np.load(dir + '/InitialEnergy.npy')['0']
y = np.load(dir + '/y.npy')['0']
z = np.load(dir + '/z.npy')['0']
data = np.reshape(np.load(dir + '/DepositedEnergy.npy'),(len(E),len(y),len(z)))

norm = colors.Normalize(vmin=0,vmax=np.max(data['1']))

plt.ion()
fig,ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.25)
l = plt.contourf(z,y,data['1'][0,:,:],cmap=cmap)
#l.set_array(data['1'][5,:,:])
plt.colorbar(cm.ScalarMappable(norm=norm, cmap=cmap)).set_label('Deposited Energy [MeV]')
plt.ylabel('Transverse crystal ID')
plt.xlabel('Longitudinal crystal ID')
axd = plt.axes([0.25, 0.05, 0.55, 0.03])
sd = Slider(axd, 'E [MeV]',min(E),max(E),valinit=E[0],valstep=E[1]-E[0])


def update(val):
    global l
    id = np.where(np.abs(E - float(val))<=1e-6)[0][0]
    for coll in l.collections:
        coll.remove()
    l = ax.contourf(z,y,data['1'][id,:,:],cmap=cmap)
    fig.canvas.draw()
    fig.canvas.flush_events()

sd.on_changed(update)

plt.show()
