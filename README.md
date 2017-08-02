# G4HadStudies
Is a stand alone geant4 application to calculate hadronic cross sections. 

Prerequisites: Geant4 and ROOT installed and the runtime properly initialized.  
cmake 



Checking the code out from github:
git clone https://hanswenzel@github.com/hanswenzel/G4HadStudies.git

Build Instructions:
For an out of source build do:

mkdir G4HadStudies-build

cd G4HadStudies-build

cmake ../G4HadStudies/ -DCMAKE_INSTALL_PREFIX=../G4HadStudies-install

make install

To run it :

First use the provided python script to generate the geant4 input: 
cd ../G4HadStudies-install/bin
python ../../G4HadStudies/scripts/makePi+Inputs.py
chmod +x run.sh

./run.sh

if you want to run more eventsfor better statistics  change the line 
print >> out, "/run/beamOn 100000" 
in makePi+Inputs.py accordingly.








