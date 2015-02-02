# G4HadStudies
Is a stand alone geant 4 application to calculate hadronic cross sections. 

Prerequisites: Geant 4 and ROOT installed and the runtime properly initialized.  cmake 

Build Instructions:
For an out of source build do:

mkdir G4HadStudies-build
cd G4HadStudies-build
cmake ../G4HadStudies/ -DCMAKE_INSTALL_PREFIX=../G4HadStudies-install
make install


