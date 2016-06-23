import os

g4particle = os.environ["ARGONE"] #raw_input("Particle (G4 shortname)?  ")
Material= os.environ["ARGTWO"] #raw_input("Target Material?          ")
plist = os.environ["ARGTHREE"] #raw_input("Physics List?             ")
Emin= float(os.environ["ARGFOUR"]) #float(raw_input("Minimum Energy? (MeV)     "))
Emax= float(os.environ["ARGFIVE"]) #float(raw_input("Maximum Energy? (MeV)     "))
NBins= int(os.environ["ARGSIX"]) #int(raw_input("Number of Bins?           "))
NParticles= int(os.environ["ARGSEVEN"]) #raw_input("Number of Particles?      ")

step=(Emax-Emin)/(NBins-1)
energies = [ step*j+Emin for j in range(0,NBins) ]

path = "%s_%s_%s" % (g4particle,Material,plist)

if not os.path.exists(path):
    os.makedirs(path)

master = open( os.path.join(path,"run.sh"), "w" )
tuples = open( os.path.join(path,"tuplelist.txt"), "w" )
print >> master, "export PHYSLIST=%s" % (plist)
n = 0

for run in energies:
 
  out = open( os.path.join(path,"%s_%s_%03d.in" % (g4particle,plist,n)), "w" )

  print >> out, "/control/verbose 1 "
  print >> out, "/run/verbose 1 "
  print >> out, "/tracking/verbose 0 "
  print >> out, ""
  print >> out, "/testhadr/TargetMat G4_%s" % Material
  print >> out, "/testhadr/TargetRadius 100 cm "
  print >> out, "/testhadr/TargetLength 1 cm "
  print >> out, ""
  print >> out, "/testhadr/Update "
  print >> out, "/run/initialize "
  print >> out, "/random/setSeeds %d %d " % (n, 31+n)
  print >> out, ""
  print >> out, "/gun/particle %s " % g4particle
  print >> out, "/gun/energy %2.1f MeV " % run
  print >> out, "/run/beamOn %s" % str(NParticles)

#You may need to make some changes if you are running on a different computer:
#in the following line, replace /home/isaac13/ ... with the path to g4had
#on your own computer.
  print >> master, "/home/isaac13/G4HadStudies-install/bin/g4had %s_%s_%03d.in >& %s_%s_%03d.log" % (g4particle,plist,n,g4particle,plist,n)
  print >> tuples, "ntuple_%04d.root" % n

  n += 1

