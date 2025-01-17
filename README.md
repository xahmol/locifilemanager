# LOCI File Manager
File manager for the LOCI mass storage device for Oric Atmos

## Contents:

[Version history and download](#version-history-and-download)

[Introduction](#introduction)

[Requirements](#requirements)

[Start program](#start-program)

[Credits](#credits)

![LOCI FM Main Interface](https://github.com/xahmol/locifilemanager/blob/main/screenshots/LociFM%20-%20main%20interface.png?raw=true)

## Version history and download
([Back to contents](#contents))

Link to latest build:
- https://github.com/xahmol/locifilemanager/raw/refs/heads/main/BUILD/locifm.tap

Version 0.1.0:
- First feature complete beta release

## Introduction
([Back to contents](#contents))

LOCI Filemanager is a filemanager to be used with the great LOCI mass storage device for the Oric Atmos.

For more information about the LOCI:
https://github.com/sodiumlb/loci-hardware/wiki/LOCI-User-Manual

Features of this file manager:
- Browse through both the internal storage of the LOCI as all connected USB mass storage devices
- Features two browser panes with two independently loaded directories
- Can copy files (not directories) between panes
- Cam delete and rename files and directories
- Can create directories
- Can perform copy and delete based on a user made selection of multiple files
- Can mount disk, tape and ROM images
- On exit boot based on mounted images (disk > tape > ROM)
- Can browse inside tape image to select file on the tape to mount / boot
- IJK joystick interface supported: broswing and all menu operation can be done via joystick
- Both menu as keyboard driven
- Source fully public, includes LOCI API lib for own projects

## Requirements
([Back to contents](#contents))

Minimum requirements:
- a LOCI device with minimum firmware 0.2.5
- Firmware 0.2.6 is needed for the ability to create directories

## Start program
([Back to contents](#contents))

- Go to the LOCI user interface by pressing the red button on the LOCI
- 

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