# README
---------------------
The code is working with the Geant4 version 10.01 compiled with the multithread option on 

## How to run:

  1. create the directory phase_space (for output files) at the same level of the main file
  2. copy the file proton.mac in the build directory
  3. find out where the geant4 is installed, source the script
     \<geant4 share dir\>/geant4make/geant4make.sh to setup enviroment variables
  4. to start the run, two arguments are requested as input:
     the file.mac where the gun properties are set and a string with a name
     for the outputfiles:

    ```Shell
    ./dnaphysics proton.mac <output filename>
    ```

## Output:

The output consists on two text files:

  * edep_outputFileName
  * ps_outputFileName (created in the directory phase_space).

With the present input settings the two files dimensions are ~ 1kB and 19kB, respectively. 
To get a smaller output the number of events in the .mac file can be deceased 
(/run/beamOn 10000)


## Notes:

  * In the main file (dnaphysics.cc) the seed can be set. At the moment a fixed seed is set (to compare the output on different machines)  line 63 G4Random::setTheSeed(10). 
  * In the main file line 74 runManager->SetNumberOfThreads(3) set the number of threads to use (is it tis commented all the cores available are used).  
