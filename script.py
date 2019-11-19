
#faust auto uploader for ESP32 code.
#run this code in the root directory of your esp32 project (update this at the end) 


#OLD CODE

#this shit isnt going to work for windows

#lscheck = 'ls'
#files = os.popen(lscheck).read()

#print(files)

#filetime checking command:
#stat -c '%Y' foobar.txt
#returns int when it was last updated.
#seems like you can easily check all of the items in a list i.e. foobar1.txt foobar2.txt foobar3.txt
#also, directory stuff works i.e. faust/script.py

#afile = 'script.py'
#modcheck = 'stat -c %Y ' + afile


#os.system(modcheck)

#class File:
 #   def __init__(self, name, modified):
  #      self.name = name
   #     self.modified = modified

   
'''
modified = os.path.getctime(destFile) #get the time the last time the file was modified
print(modified)
shutil.copy(homeFile, destFile) #copy the file to the new destination
modified = os.path.getctime(destFile)
print(modified)
'''

#Some stuff from stack exchange for a gui that can select paths
'''
Tk().withdraw() # we don't want a full GUI, so keep the root window from appearing
filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file
print(filename)
'''
   

#END OLD Ccode



#End user expirence:
#User runs the script, maybe chooses a file name and a housing directory and maybe a destination directory
#Download a faust esp32 file (pre-compilled) - maybe also want to do it for an uncompiled file. 
#it automatically flashes the esp32 file onto the esp32

#Pseudocode:
#Constantly check the chosen directory for an updated file
#if you find one, move it into the main folder of the project directory
# 
# sit in directory and run the following every second
#for each file in the directory, if there is a more recent modified  date on each of the 

#Directory info
#Important files in main: main.cpp, GenericFaustName.cpp, GenericFaustName.h, AudioCodecFileName.cpp, AudioCodecFilename.h
#Unimportant files (probably don't need to check these): CMakeLists.txt, component.mk

import os
import shutil
import time

from tkinter import  Tk
from tkinter.filedialog import askdirectory, askopenfilename


homeFile = '/home/dirk/pythonTE/old/faustBasic.zip'
destDir = '/home/dirk/pythonTE/new/faustBasic'

#open the Gui to select the file that is being check and the project directory
Tk().withdraw()
#homeFile = askopenfilename() #get the name of the zip file that will be checked & movedun
#destDir = askdirectory()

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
        
        os.system('unzip faustBasic.zip -d main && cd main && mv faustBasic/* .')
        #going to need to find a way to do this on windows consistently
        
        print("I copied the file. Love me")
        os.system(export + ' && ' + make + ' && ' + makeFlash) 
        break
    
    time.sleep(.5)
