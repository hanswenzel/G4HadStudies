particle = "Pion+"
g4particle = "pi+"
plist = "QGSP_BERT"
energies = [ 50.0*j for j in range(1,41) ]
master = open( "run.sh", "w" )
tuples = open( "tuplelist.txt", "w" )

n = 0

for run in energies:
  out = open( "%s_%s_%03d.in" % (particle,plist,n), "w" )

  print >> out, "/control/verbose 1 "
  print >> out, "/run/verbose 1 "
  print >> out, "/tracking/verbose 0 "
  print >> out, ""
  print >> out, "/testhadr/TargetMat G4_C "
  print >> out, "/testhadr/TargetRadius 100 cm "
  print >> out, "/testhadr/TargetLength 1 cm "
  print >> out, ""
  print >> out, "/testhadr/Update "
  print >> out, "/run/initialize "
  print >> out, "/random/setSeeds %d %d " % (n, 31+n)
  print >> out, ""
  print >> out, "/gun/particle %s " % g4particle
  print >> out, "/gun/energy %2.1f MeV " % run
  print >> out, "/run/beamOn 1000000"

  print >> master, "g4had.exe %ss_%s_%03d.in" % (particle,plist,n)
  print >> tuples, "ntuple_%04d.root" % n

  n += 1

