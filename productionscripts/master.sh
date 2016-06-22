 #!/bin/bash

#ask for and export the parameters
if [ "$#" = "7" ]; then 
    echo "You have inputted the correct number of arguments"
else
    echo "You don't have the correct number of arguments."
    echo "The 7 arguments are as follows:"
    echo "1: Particle, 2: Material, 3: Physics List, 4: Minimum Energy (MeV),"
    echo "5: Maximum Energy (MeV), 6: Number of Bins, 7: Number of Particles."
    exit 0
fi
echo "$1, $2, $3, $4, $5, $6, $7"
NEWDIR="$1"
NEWDIR+="_"
NEWDIR+="$2"
NEWDIR+="_"
NEWDIR+="$3"
export ARGONE="$1"
export ARGTWO="$2"
export ARGTHREE="$3"
export ARGFOUR="$4"
export ARGFIVE="$5"
export ARGSIX="$6"
export ARGSEVEN="$7"

#makes new directory and inputs, asks for the rest of the variables
python make_Inputs.py

#copy the .root reading program into the new directory
cp readtuple2.C $NEWDIR

#gets into the directory with all the .in  files and the .root reading program
cd $NEWDIR
echo "The program is currently running,"
echo "Thank you for your patience."
#turn the run.sh file into an executable and run it to make .root files
chmod +x run.sh 
./run.sh

#Read the .root files with the .root reading program
root -b -q .L readtuple2.C++

#change the name of the graph to include the particle, material, plist less confusion...

NEWGRAPH="$NEWDIR"
NEWGRAPH+="_"
NEWGRAPH+="XS_graph.png"

mv XS_graph.png $NEWGRAPH
