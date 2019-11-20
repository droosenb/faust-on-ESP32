
import os
import shutil
import time

from tkinter import Tk
from tkinter.filedialog import askdirectory, askopenfilename




# End user expirence:
# User runs the script, maybe chooses a file name and a housing directory and maybe a destination directory
# Download a faust esp32 file (pre-compilled) - maybe also want to do it for an uncompiled file? 
# it automatically flashes the esp32 file onto the esp32

# Pseudocode:
# Constantly check the chosen directory for an updated file
# if you find one, move it into the main folder of the project directory
# sit in directory and run the following every second
# for each file in the directory, if there is a more recent modified  date on each of the 

# Directory info
# Important files in main: main.cpp, GenericFaustName.cpp, GenericFaustName.h, AudioCodecFileName.cpp, AudioCodecFilename.h
# Unimportant files (probably don't need to check these): CMakeLists.txt, component.mk

# TO DO
# error handling - script should give a prompt and keep running rather than just erroring out
# fix gui prompts and be clear what they need to be
# set up effective defaults for the program, I.E. regular directories
# do commands that work with windows
# find out if any additional dependencies need to be installed that aren't defaults

homeFile = '/home/dirk/pythonTE/old/FaustBasic.zip'
destDir = '/home/dirk/pythonTE/new/faustBasic'

# open the Gui to select the file that is being check and the project directory
Tk().withdraw()
homeFile = askopenfilename() #get the name of the zip file that will be checked & moved
destDir = askdirectory() #get the name of the project directory

#print(homeFile)
#print(destFile)

#the commands used for setting up esp tools, building the directory, and flashing the ESP32
export = '. $HOME/esp/esp-idf/export.sh'
make = 'make --directory=' + destDir
makeFlash = 'make --directory=' + destDir + ' flash'
#the line for running all of the commands. They seem to need to be run in the same line or the export won't remain applied.
#Should look into fixing this, though it is unlikely to have an effect on funcionality.

#os.system(export + ' && ' + make + ' && ' + makeFlash) 


modified = os.path.getctime(homeFile)

while(True):

    cTime = os.path.getctime(homeFile)
    
    if modified < cTime:
        modified = cTime
        
        shutil.copy(homeFile, destDir)
        
        os.system('cd ' + destDir + ' && ' + 'unzip FaustBasic.zip -d main && cd main && mv FaustBasic/* .')
        
        #going to need to find a way to do this on windows consistently, using python unziping tools & file movement
        
        #print("Faust file moved, unzipped, and placed correctly")
        os.system(export + ' && ' + make + ' && ' + makeFlash) 
        
    
    time.sleep(.5)

