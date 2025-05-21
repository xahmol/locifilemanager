# LOCI File Manager
File manager for the LOCI mass storage device for Oric Atmos

## Contents:

[Version history and download](#version-history-and-download)

[Introduction](#introduction)

[Requirements](#requirements)

[Start program](#start-program)

[Keyboard commands](#keyboard-commands)

[Main application interface](#main-application-interface)

[Command explanation](#command-explanation)

[Credits](#credits)

![LOCI FM Main Interface](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20main%20interface.png?raw=true)

## Version history and download
([Back to contents](#contents))

Link to latest build:
- https://github.com/xahmol/locifilemanager/raw/refs/heads/main/BUILD/locifm.tap

Version 0.1.2:
- Add move function

Version 0.1.1:
- Bugfixes on tape browse and boot preference

Version 0.1.0:
- First feature complete beta release

## Introduction
([Back to contents](#contents))

LOCI File Manager is a file manager to be used with the great LOCI mass storage device for the Oric Atmos.

For more information about the LOCI:
https://github.com/sodiumlb/loci-hardware/wiki/LOCI-User-Manual

Features of this file manager:
- Browse through both the internal storage of the LOCI as all connected USB mass storage devices
- Features two browser panes with two independently loaded directories
- Can copy files (not directories) between panes
- Can delete and rename files and directories
- Can create directories
- Can perform copy and delete based on a user-made selection of multiple files
- Can mount disk, tape, and ROM images
- On exit boot based on mounted images (disk > tape > ROM)
- Can browse inside tape image to select file on the tape to mount/boot
- IJK joystick interface supported: browsing and all menu operations can be done via joystick
- Both menu and keyboard driven
- Source fully public, includes LOCI API lib for own projects

## Requirements
([Back to contents](#contents))

Minimum requirements:
- a LOCI device with minimum firmware 0.2.5
- Firmware 0.3.0 is needed for the ability to create directories

## Start program
([Back to contents](#contents))
- Download the latest build of the 'locifm.tap' file from [here](https://github.com/xahmol/locifilemanager/raw/refs/heads/main/BUILD/locifm.tap).
- Save the image on the desired location on the USB stick you want to use for your LOCI device and connect that USB (back) into the LOCI device
- Go to the LOCI user interface by pressing the red button on the LOCI. You should get the LOCI interface like this:
![LOCI UI Start program](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20start%20program.png?raw=true)
- Go to the field to select tape images by pressing T (or go there via cursor keys)
- Press SPACE to go to the file browser
- Navigate to the location on your USB stick where you have stored the 'locifm.tap' file.
- Select by SPACE
- Boot into the application by pressing ESC.
- If the Auto load option of the LOCI is enabled, loading from tape will start automatically. If not, press CLOAD" and RETURN.
- The program should now load and start.
- If no LOCI is detected, or the connected LOCI has a lower than supported firmware version the program will give an error message and exit.

For details on how to operate your LOCI device, see the [LOCI manual](https://github.com/sodiumlb/loci-hardware/wiki/LOCI-User-Manual).

## Keyboard commands
([Back to contents](#contents))

### In file browser pane:

|Key|Description
|---|---|
|**Cursor Up / Down**|Move down or up in the file browser pane
|**Cursor Left**|Go to parent directory
|**Cursor Right**|Go to main menu
|**RETURN**|Perform action selected for RETURN button: Select, mount or launch a file.
|**ESC**|Exit application and boot to mounted image(s)
|**.**|Go to next available LOCI drive for the active pane
|**,**|Go to previous available LOCI drive for the active pane
|**/**|Switch the active pane to the other pane
|**\\**|Go to the root directory of the drive in the active pane
|**D**|Page **d**own in the file browser pane
|**P**|Page u**p** in the file browser pane
|**T**|Go to **t**op: first entry in the file browser pane
|**B**|Go to **b**ottom: last entry in the file browser pane
|**S**|Toggle if file is **s**elected or not.
|**A**|Select **a**ll files in the active file browser pane
|**N**|Select **n**one of the files, so deselect all, in file browser pane
|**I**|**I**nverse selection in file browser pane
|**O**|Toggle if directories are alphabetically s**o**rted or not
|**F**|Select which **F**ilter to apply for showing directory entries or disable filtering.
|**C**|**C**opy present file or all selected files from directory in active pane to directory in non-active pane
|**V**|**C**opy present file or all selected files from directory in active pane to directory in non-active pane
|**DEL**|**Del**ete present file or directory (if empty)
|**G**|Select tar**g**et disk drive for disk images to mount to.
|**R**|**R**ename present file or directory
|**M**|**M**ount present file to disk, tape or ROM.
|**U**|Select which drive, tape or ROM should be **u**nmounted.
|**W**|Start bro**w**sing inside a tape image from present .TAP file
|**E**|Create n**e**w directory
|**H**|Show **h**elp screen for keyboard commands

### In main menu and pulldown menus

|Key|Description
|---|---|
|**Cursor Up / Down**|Move down or up in the pulldown menu options
|**Cursor Left / Right**|Go to left or right in main menu.
|**RETURN**|Select option.
|**ESC**|Cancel (if allowed)

### Joystick direction mapping to keyboard commands

The left, right, down, up joystick directions are translated to the corresponding cursor keys and invoke the same commands.
The fire button invokes the same action as the RETURN key.

## Main application interface
([Back to contents](#contents))

After application start, the main application interface looks similar to:
![LOCI FM Main Interface](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20main%20interface.png?raw=true)

Top line of the screen is a header showing application name, followed by the identification and firmware version string of the connected LOCI device.

Second line is the main menu bar. Go there by pressing the **Cursor Right** key or push right on your joystick.

Below are two panes, both representing a directory on a LOCI provided storage drive ID. The pane shown in yellow is the active pane, white the non-active.
First line of both panes is the name identification of the drive from which a directory is shown, second line shows the active path of the directory shown, of which the first number is the drive ID number. 0 is internal storage, 1 to 9 the USB mass storage devices connected to the LOCI.

Below are the directory entries, shown per page of 10. First the name, to the right the type:
- DSK: Disk image
- TAP: Tape image
- ROM: ROM image
- LCE: LOCI Executable (not yet implemented)
- DIR: Directory
- spaces: unknown type

In the active pane, in cyan the present entry is highlighted. This can be moved up and down by pressing cursor keys or joystick. Also page up/down is supported, just as go to top or bottom.
Based on the selected present files all commands are executed.
Reference is made to the [Keyboard commands](#keyboard-commands) section above for available keyboard commands, see [Command explanation](#command-explanation) for explanation of what all commands do and how they should be operated, sorted by the different main menu options.

## Command explanation
([Back to contents](#contents))

### App: Application options

![Application menu](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20app.png?raw=true)

*Confirm*

Confirm is a toggle that decides if an 'are you sure' popup is shown, to confirm either a delete or a copy with overwrite action, once or on every individual file separately. Default is Once.

This function can be reached via the menu only.

*Return*

![App: Return](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20app%20-%20return.png?raw=true)

Selects which action should be performed when pressing RETURN:
- Select: Toggles if the file is selected or not on pressing **RETURN**, so **RETURN** will behave as the **S** key. Default option.
- Mount: File will be mounted on pressing **RETURN**, so **RETURN** will behave as the **M** key.
- Launch: File will be launched on pressing **RETURN**. This means that all other mounts will be unmounted, present file will be mounted, and application will be exited to boot to this mount. **RETURN** in this case will behave as pressing first the **U** key for every active mount, then the **ESC** key.

This function can be reached via the menu only.

*Filter*

![App: Filter](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20app%20-%20filter.png?raw=true)

Selects which filter to apply when showing directories:
- None: No filter applied
- .DSK: Only .DSK disk images shown
- .TAP: Only .TAP tape images shown
- .ROM: Only .ROM rom images shown
- .LCE: Only LOCI Executables shown (not yet implemented as .LCE format is to be designed)

This function can be reached also by pressing the **F** key.

*Sort*

Toggle if sorting of directory entries is enabled or not.

**NB**: Sorting can be very slow on large directories, so beware of switching it on on those large directories. Also for this reason, default is Off.

This function can be reached also by pressing the **O** key.

*Exit*

![App: Exit](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20app%20-%20exit.png?raw=true)

Application will exit and boot based on the active mounts (drive A > .TAP > ROM).

Autostarting from .TAP images is enabled.

This function can be reached also by pressing the **ESC** key.

### File: File operations

![File menu](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file.png?raw=true)

*Select toggle*

Toggles if presently highlighted file is selected or not. Selected files will be shown with a - before their name like in the screenshot below.

This function can be reached also by pressing the **S** key.

![File: Select](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20select.png?raw=true)

*Select all*

Selects all files in the present pane. This function can be reached also by pressing the **A** key.

*Select none*

Deselects all files in the present pane. This function can be reached also by pressing the **N** key.

*Inverse select*

Inverses the selection of files in the present pane. This function can be reached also by pressing the **I** key.

*Delete*

Deletes the present file or directory. Or, if a selection of files is made, all selected files.

A popup will ask for confirmation (based on the application configuration setting for Confirm in the App menu, only once, or for every file), after which deletion will proceed. Press key to return.

This function will also delete a directory, but:
- A directory must be empty, otherwise an error message will be shown. Recursive directory delete is not (yet) implemented.
- Directories can not be selected, so deletion of directories has to be performed one by one.

This function can be reached also by pressing the **DEL** key.

![File: Delete](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20delete.png?raw=true)

*Rename*

Renames the present file or directory.

A popup will appear to edit the name. Cursor keys and DEL key are supported in editing. Press RETURN to accept, ESC to cancel.

This function can be reached also by pressing the **R** key.

![File: Rename](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20rename.png?raw=true)

*Copy / Move*

Copies or moves the present file. Or, if a selection of files is made, all selected files. Copying or moving directories with or without content is not (yet) implemented.

Copy or move will be performed from the directory in the active pane to the directory in the non-active pane. You can not copy or move to the same directory, in this case an error message will be shown:

![File: Copy - error same dir](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20copy%20-%20same%20dir.png?raw=true)

If file(s) with the same name already exist, a popup will ask for confirmation to overwrite the files in the target directory (based on the application configuration setting for Confirm in the App menu, only once, or for every file).

![File: Copy - confirm overwrite](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20copy%20-%20confirm.png?raw=true)

Otherwise a popup will appear showing copy or move progress. Pressing **ESC** can cancel file copying after finishing copying the present file. Mid-file copy cancellation is not (yet) implemented.

![File: Copy](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20copy.png?raw=true)

This function can be reached also by pressing the **C** key for copy or **V** key for move.

*Browse tape*

Enables browsing within a .TAP tape image. Obviously only works if the present highlighted directory entry is identified as type TAP.

Selecting first mounts the tape image. This popup is shown, press key to continue:

![File: Browse tape - mount message](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20browse%20tape%20-%20mount.png?raw=true)


Then the active pane of the browser will look like this:

![File: Browse tape](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20browse%20tape%20-%20browse.png?raw=true)

First line now shows Tape: to indicate you are inside a tape image, followed by the name of the tape image. Second line is still the active path to the tape image.

As directory entries now the files within the tape image are shown, starting with the name (if any), followed by the start address and size, finally the detected tape: BAS for a BASIC file, BIN for all others.

Inside a tape the copy, delete and rename functions do not operate (not possible, or not (yet) implemented). Therefore selecting files also does not work.

Only thing that does work in browsing tape is moving to the desired file inside a tape by pressing **RETURN**. This will give this popup:

![File: Browse tape - move to file](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-file%20-%20browse%20tape%20-%20mount%20file%20on%20tape.png?raw=true)

Go back to the normal directory by pressing **Cursor Left** (or joystick left).

This function can be reached also by pressing the **W** key.

### Dir: Directory navigation and operations

![Dir menu](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20dir.png?raw=true)

*Go to root*

Go to the root directory of the drive in the active pane. This function can be reached also by pressing the **\\** key.

*Back*

Go to the parent directory of the drive in the active pane. This function can be reached also by pressing the **Cursor Left** key.

*Page down*

Go to the next page of entries in the active pane. This function can be reached also by pressing the **D** key.

*Page up*

Go to the previous page of entries in the active pane. This function can be reached also by pressing the **P** key.

*Top^

Go to the first entry in the active pane. This function can be reached also by pressing the **T** key.

*Bottom^

Go to the last entry in the active pane. This function can be reached also by pressing the **B** key.

*New dir*

Create a new directory in the active pane from the active path.

This will give a popup to enter the dir name. Input the name (cursor keys and DEL key can be used for editing), press **RETURN** to accept and **ESC** to cancel.

This function can be reached also by pressing the **E** key.

![Dir: New dir](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20dir%20-%20create%20dir.png?raw=true)

### Info: Version information and help

![Info menu](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20info.png?raw=true)

*Version/credits*

Shows version information of the application.

![Info: Version](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20info%20-%20version.png?raw=true)

*Help*

Shows a help screen for the keyboard commands.

![Info: Help](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20menu%20-%20info%20-%20help.png?raw=true)


## Credits
([Back to contents](#contents))

LOCI File Manager
File manager for the LOCI mass storage device for Oric Atmos
Written in 2025 by Xander Mol

https://github.com/xahmol/locifilemanager

https://www.idreamtin8bits.com/

For information and documentation on the LOCI mass storage device:
-   LOCI User Manual

    https://github.com/sodiumlb/loci-hardware/wiki/LOCI-User-Manual
-   Sellers of my assembled LOCI device:
    - Raxiss: https://www.raxiss.com/article/id/38-LOCI

Code and resources from others used:
-   LOCI ROM by Sodiumlightbaby, 2024

    https://github.com/sodiumlb/loci-rom

-   CC65 cross compiler:

    https://cc65.github.io/

-   DraBrowse source code for inspiration and text input routine
    DraBrowse (db*) is a simple file browser.
    Originally created 2009 by Sascha Bader.
    Used version adapted by Dirk Jagdmann (doj)

    https://github.com/doj/dracopy

-   lib-ijk-egoist from oricOpenLibrary (for joystick support via Raxiss IJK interface)
    By Raxiss, (c) 2021
    
    https://github.com/iss000/oricOpenLibrary/tree/main/lib-ijk-egoist

-   forum.defence-force.org: For inspiration and advice while coding.

-   Original windowing system code on Commodore 128 by unknown author.
   
-   Tested using real hardware Oric Atmos plus LOCI

The code can be used freely as long as you retain
a notice describing original source and author.

THE PROGRAMS ARE DISTRIBUTED IN THE HOPE THAT THEY WILL BE USEFUL,
BUT WITHOUT ANY WARRANTY. USE THEM AT YOUR OWN RISK!