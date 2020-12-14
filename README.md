# MS-DOS TUI engine


## Prerequisites to compile

### Dosbox
To compile and run this project, you need dosbox.

Download it here: https://www.dosbox.com/download.php?main=1


#### Dosbox config file
You might also want to edit the dosbox config file (usually located under ~/.dosbox in linux)
and append lines to autoexec.bat (the bottom of the config file),
for example a "keyb gr" command or the mount command for c: (this root folder should be mounted to C:, so the compile script works properly)


### OpenWatcom
The compiler used is OpenWatcom: http://www.openwatcom.org/download.php

Download the DOS Version and install it in your Dosbox C:\ Drive under C:\WATCOM (default path)

Install the 16bit and 32bit compiler for dos.


## Compile
To compile the project, run COMPILE.BAT in the ENGINE directory.


## Run
To run the project, execute MAIN.EXE in the ENGINE directory.
