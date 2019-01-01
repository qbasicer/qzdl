# qZDL INI Format

qZDL's settings are stored in the standard INI format. There is a qZDL.ini file, which stores
settings for the application, and there are .zdl files, which contain only the "zdl.save" section.
As of version 3.2.2.3, q sample qZDL.ini file on Linux might look like this:

	[zdl.general]
	autoclose=0
	zdllaunch=1
	showpaths=1
	rememberFilelist=1
	windowsize=952,1018
	windowpos=960,26
	engine=ZDL
	version=3.2.2.3 beta (linux/custom)
	conflib=sunrise
	lastDir=/home/user/doom/sunlust
	alwaysadd=-nomonsters -turbo 250
	[zdl.save]
	port=GZDoom
	iwad=DOOM 2
	skill=4
	warp=MAP02
	file0=/usr/share/games/doom/lights.pk3
	file1=/usr/share/games/doom/brightmaps.pk3
	file2=/home/user/doom/sunlust/sunlust.wad
	[zdl.iwads]
	i0n=The Ultimate DOOM
	i0f=/home/user/doom/DOOM.WAD
	i1n=DOOM 2
	i1f=/home/user/doom/DOOM2.WAD
	[zdl.ports]
	p0n=DOOM Retro
	p0f=/usr/bin/doomretro
	p1n=GZDoom
	p1f=/usr/bin/gzdoom

While a .zdl file exported from it would look like this:

	[zdl.save]
	port=GZDoom
	iwad=DOOM 2
	skill=4
	warp=MAP02
	file0=/usr/share/games/doom/lights.pk3
	file1=/usr/share/games/doom/brightmaps.pk3
	file2=/home/user/doom/sunlust/sunlust.wad
	[zdl.general]
	conflib=sunrise

## zdl.general

### autoclose

This is a checkbox labelled "Close on launch". Possible values are "0" and "1".

### zdllaunch

This is a checkbox labelled "Launch \*.ZDL files transparently". Possible values are "0" and "1".

### showpaths

This is a checkbox labelled "Show files paths in lists". Possible values are "0" and "1".

### rememberFilelist

This is a checkbox labelled "Save/Load PWAD list automatically". Possible values are "0" and "1".

### windowsize

This is the main windows's size when last closed. The value is taken from QMainWindow::size(). An example value: "986,1023".

### windowpos

This is the main windows's position when last closed. The value is take from QMainWindow::pos(). An example value: "1016,26".

### engine

This is hardcoded to "ZDL".

### version

This is build/versioning metadata.  Example value: "3.2.2.3 beta (linux/custom)"

### conflib

This hardcoded to "sunrise".

### lastDir

This is the directory containing the last file added: either IWAD, PWAD, engine, or .zdl. Example value: "/home/user/doom"

### alwaysadd

This is the text box labelled "Always Add These Parameters". Example value: "-nomonsters -turbo 250"

## zdl.save

This is the subsection that gets saved as .zdl files. It's mainly the PWADs.

### file*n*

The list of "External Files" to load, in order.

### gametype

An index into the "Game Mode" dropdown. Possible values are:

0. Single Player
1. Co-Op
2. Deathmatch

### players

An index into the "Players" dropdown. Possible values are:

0. Joining
1. 1
2. 2
3. 3
4. 4
5. 5
6. 6
7. 7
8. 8

### dmflags

The value of "DMFLAGS". An integer from 0 to 10.

### dmflags2

The value of "DMFLAGS2". An integer from 0 to 10.

### fraglimit

The value of "Frag Limit". An integer from 0 to 10.

### port

The Source Port/Engine to use. Must have a corresponding p*n*n entry in zdl.ports. Example: "DOOM Retro"

### iwad

The IWAD to use. Must have a corresponding i*n*n name in zdl.iwads. Example: "DOOM 2"

### dlgmode

Whether the advanced multiplayer options have been extended. Possible values are "closed" and "open".

### extra

The "Extra command line arguments". Example value: "-playdemo speedrun.lmp"

### host
 The value of "Hostname/IP Address". This is a freeform string.

## zdl.net

These are the ZDL Advanced Multiplayer Settings.

### extratic

The "Extratic: On/Off" checkbox. Possible values are "0" and "1".

### netmode

An index into the "Net Mode" dropdown. The indexed values are:

0. Not Specified
1. 0 (Classic)
2. 1 (Client/Server)

### port

The value of "Port:". This is a freeform string.

### dup

The "Dup" dropdown. Possible values range from "0" ("Not Specified") to "9".

### advenabled

The "Enable: Yes/No" checkbox. Possible values are "0" and "1".

## zdl.iwads

The list of available IWADs, in order. 

### i*n*n/i*n*f

The value of i*n*n is the *n*th IWAD's Name, and the value of i*n*f is *n*th IWAD's File path.

## zdl.ports

The list of Source Port/Engines, in order. 

### p*n*n/p*n*f

The value of p*n*n is the *n*th engine's Name, and the value of p*n*f is the *n*th engine's File path.
