from tkinter import *
from tkinter.filedialog import askdirectory, askopenfilename
from tkinter import messagebox

import os
from os import path
import zipfile
import esptool
from PIL import ImageTk, Image
'''
#Uncomment this block for english
######### ERROR MESSAGES ##########
noFileError = "ERROR: File not found, was it moved or deleted?"
connectionErrorMessage = "ERROR: The gramophone seems to be disconnected"
missingFileErrorMessage = "ERROR: The zip file doesn't seem to contain the needed files"
noFileSelectedError = "ERROR: A zipfile was not selected"

#########STATUS MESSAGES###########
flashingMessage = "Flashing to the Gramophone"
appRunningMessage = 'Flashing Finished, waiting for a new file'
initialZipMessage = 'Please select the Faust binary you downloaded'
waitingMessage =  "Waiting for a file to be selected"

########LABELS##########
windowTitle = "ESP32 Automatic USB Flashing Tool"
selectLabel = "Select File"
statusMessage = 'Status:'
quitLabel = 'QUIT'
restartLabel = 'Restart Checking'
'''

#uncomment this block for french
######### ERROR MESSAGES ##########
noFileError = "ERREUR : Fichier non trouvé, a-t-il été déplacé ou supprimé ?"
connectionErrorMessage = "ERREUR : Le gramophone semble déconnecté."
missingFileErrorMessage = "ERREUR: Le fichier zip ne semble pas contenir les fichiers attendus."
noFileSelectedError = "ERREUR : Aucun fichier zip sélectionné."

#########STATUS MESSAGES###########
flashingMessage = "Programmation du Gramophone en cours."
appRunningMessage = 'Programmation terminée, en attente d’un nouveau fichier.'
initialZipMessage = 'Veuillez sélectionner le binaire Faust que vous avez téléchargé.'
waitingMessage =  "En attente de la sélection d'un fichier."

########LABELS##########
windowTitle = "ESP32 outil USB de programmation automatique"
selectLabel = "Sélectionner un fichier"
statusMessage = 'Statut :'
quitLabel = 'QUITTER'
restartLabel = 'Réessayer'


# global varibles
zipPath = initialZipMessage  # permenant path to the zip file
running = False
modifiedT = -1
flashing = False

def checkFile(path):
    # returns true if the file has been modified more recently than the last modifed date, otherwise returns false
    global modifiedT, running
    try:
        cTime = os.path.getctime(path)
        # errorM.configure(text = "None")
    except FileNotFoundError:
        # print('Did you delete or move the file? It doesn\'t seem to be where it was...')
        status.configure(fg='red', text=noFileError)
        cTime = -1
        running = False

    if modifiedT < cTime:
        modifiedT = cTime
        return True;
    else:
        return False;


def unzip(path):
    # unzips the directory at the path. Should probably change this to only accept a zipFile
    # print("unzipping")
    tempzip = zipfile.ZipFile(path, 'r')
    tempzip.extractall(os.path.dirname(path))


def flash(path):  # takes path to directory contanining the three binaries
    status.configure(text=flashingMessage)
    bootloaderPath = path + 'bootloader.bin'
    mainPath = path + 'faustBasic.bin'
    partitions_singleappPath = path + 'partitions_singleapp.bin'

    command = ['--chip', 'esp32', '--baud', '115200', '--before', 'default_reset', '--after', 'hard_reset',
               'write_flash', '-z', '--flash_mode', 'dio', '--flash_freq', '40m', '0x1000', bootloaderPath, '0x10000',
               mainPath, '0x8000', partitions_singleappPath]

    # command does not contain --port or --flash_size since both seem to cause some issues.
    window.after(10, lambda: asyncFlash(command))


def asyncFlash(command):  # command for asynchronously flashing the binaries.
    global flashing, running, modifiedT
    try:
        flashing = True
        esptool.main(command)

    except esptool.FatalError:
        status.configure(fg='red', text=connectionErrorMessage)
        # print("the board doesn't seem to be connected")
        running = False
        modifiedT = -1

    except SystemExit:
        status.configure(fg='red', text=missingFileErrorMessage)
        # print("the board doesn't seem to be connected")
        # e = sys.exc_info()[0]
        # xprint( "<p>Error: %s</p>" % e )
        running = False
        modifiedT = -1

    flashing = False


def checkDir(path):
    global modifiedT
    # Checks to see if there are any files that have similar names to path and were modified more recently
    # if there are it renames them to path
    basepath = os.path.dirname(path)
    with os.scandir(basepath) as entries:
        for entry in entries:
            if os.path.basename(entry)[-4:] == '.zip':
                if os.path.basename(entry)[1:5] == os.path.basename(path)[1:5]:
                    if (modifiedT < os.path.getctime(entry)):
                        # print('renaming to ' +  path)
                        os.rename(entry, path)


def daemon(event=None):
    global zipPath, running

    if (running and not flashing):
        status.configure(text=appRunningMessage)

        try:
            checkDir(zipPath)
            # errorM.configure(text = "None")
        except FileNotFoundError:
            # print('Did you delete or move the file? It doesn\'t seem to be where it was...')
            status.configure(fg='red', text=noFileError)
            cTime = -1
            running = False
        if (checkFile(zipPath)):
            # print('trying to flash')
            unzip(zipPath)
            flash(zipPath.rstrip('.zip') + '/')

    if (running):
        window.after(500, daemon)


def changeFile():  # change the file which is being scanned
    global zipPath, modifiedT, directory, running

    zipFile = askopenfilename(filetypes=[("ZIP", "*.zip")])  # get the name of the zip file that will be checked & moved

    if zipFile[-4:] == '.zip':
        zipPath = zipFile
        modifiedT = 0
        directory.configure(text=zipPath)
        # print("changed zip path to " + zipPath)

        # errorM.configure(text = 'None')
        status.configure(fg='black')

        if (not running):
            window.after(500, daemon)
            running = True

    else:
        # print('Thats not a zip file, please select the zip file you downloaded')
        status.configure(fg='red', text=noFileSelectedError)


def exitGUI():
    global running
    running = False
    raise SystemExit


def restart():
    global running
    if not running:
        running = True
        window.after(500, daemon)
        status.configure(fg='black')


window = Tk()

window.title(windowTitle)
window.geometry('500x500')
try:
    bundle_dir = getattr(sys, '_MEIPASS', path.abspath(path.dirname(__file__)))
    background_image = Image.open(path.join(bundle_dir, 'Backround.jpg'))
    background_image = background_image.resize((500,500), Image.ANTIALIAS)
    background_photo =  ImageTk.PhotoImage(background_image)
    background_label =Label(window, image=background_photo)
    background_label.image = background_photo
    background_label.place(anchor=NW, x=0, y=0)
except:
    pass


directory = Label(window, text = zipPath, font=("Helvetica", 10))
directory.place(anchor=NW, x=(5/2), y=(20/2))

changeFileB = Button(window, text=selectLabel, command=changeFile, font=("Helvetica", 15))
changeFileB.place(anchor=NE, x=(995/2), y=(80/2))

status = Label(window, text = waitingMessage, font=("Helvetica", 12))
status.place(anchor=CENTER, x=(500/2), y=(550/2))

sLabel = Label(window, text = statusMessage, font=("Helvetica", 20))
sLabel.place(anchor=CENTER, x=(500/2), y=450/2)

quitB = Button(window,
    text=quitLabel, #the text on the button
    fg="red", #the foreground color
    command=exitGUI,
    font=("Helvetica", 20)
    )
quitB.place(anchor=SE, x=900/2, y=750/2) #place the button as far left as possible

restartB = Button(window, text=restartLabel, command=restart, font=("Helvetica", 20))
restartB.place(anchor=SW, x=100/2, y=750/2)
window.mainloop()
raise SystemExit
