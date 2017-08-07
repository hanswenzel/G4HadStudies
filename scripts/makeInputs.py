#!/usr/bin/env python 
import numpy
import sys, getopt
def main(argv):
  g4particle="pi+"
  plist='FTFP_BERT'
  nsamples=10
  nevts=10000
  material="G4_C"
  mine = 31
  maxe=700000
  #print 'Number of arguments:', len(sys.argv), 'arguments.'
  #print 'Argument List:', str(sys.argv)
  try:
    opts, args = getopt.getopt(argv,"p:n:l:s:",["mat=","min=","max="])
  except getopt.GetoptError:
    print './makePi+Inputs.py  -p <particle> -n <nrofeevents>'
    sys.exit(2)

  for opt, arg in opts:
    if opt == '-p':
      g4particle= arg
      print g4particle
    if opt == '-n':
      nevts = arg
      print nevts
    if opt == '-l':
      plist= arg
      print plist
    if opt == '-s':
      nsamples  = arg
      print nsamples
    if opt == '--mat':
      material = arg
    if opt == '--min':
      mine = arg
    if opt == '--max':
      maxe = arg
  master = open( "run.sh", "w" )
  tuples = open( "tuplelist.txt", "w" )
  print >> master,"export PHYSLIST=%s" % plist
  n = 0
  for energies in  numpy.exp(numpy.linspace(numpy.log(mine), numpy.log(maxe), nsamples)):
    macroname =  "%s_%s_%s_%06d_MeV.in"   % (g4particle,material,plist,energies)
    filename  =  "%s_%s_%s_%06d_MeV.root" % (g4particle,material,plist,energies)
    out = open(macroname , "w" )
    
    print >> out, "/control/verbose 1 "
    print >> out, "/run/verbose 1 "
    print >> out, "/tracking/verbose 0 "
    print >> out, "/testhadr/TargetMat %s" % material
    print >> out, "/testhadr/TargetRadius 100 cm "
    print >> out, "/testhadr/TargetLength 1 cm "
    print >> out, "/testhadr/Update "
    print >> out, "/testana/FileName %s" % filename
    print >> out, "/run/initialize "
    print >> out, "/random/setSeeds %d %d " % (n, 31+n)
    print >> out, "/gun/particle %s " % g4particle
    print >> out, "/gun/energy %2.1f MeV " % energies
    print >> out, "/run/beamOn %s" % nevts
    print >> master,"../bin/g4had ",macroname
    print >> tuples, filename
    
    n += 1
    
if __name__ == "__main__":
  main(sys.argv[1:])
