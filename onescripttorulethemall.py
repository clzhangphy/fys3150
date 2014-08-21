#!/usr/bin/env python
from numpy import *
from pylab import *
from matplotlib import rc, rcParams
import matplotlib.units as units
import matplotlib.ticker as ticker
import sys, os

try:
    infilename = sys.argv[1]
except:
    print "Usage of this script", sys.argv[0], "infile", sys.argv[1]; sys.exit(1)
# compile and link c++ program1.cpp
cmd = 'c++ -O3 -o program1.x program1.cpp'


# Now run code, here c++ code  which has been compiled and linked
cmd1 = './program1.x out1.dat'
failure = os.system(cmd)
if failure:
   print 'running program1 failed'; sys.exit(1)
# and then make plot
rc('text',usetex=True)
rc('font',**{'family':'serif','serif':['Error as function of h on log scale']})

data = loadtxt("out.dat")

axis([-6.0,-1.0,-9.0, -3.0])
xlabel(r'$log_{10}(h)$',fontsize=20)
ylabel(r'$log_{10}(\epsilon)$',fontsize=20)
plot(data[:,0], data[:,1],'b-*', markersize=7)

#xticks( [4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20])
# Save the figure in a separate file
savefig('out.pdf', format='pdf')
# Draw the plot to screen
show()
    

