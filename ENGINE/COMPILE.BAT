cls
set path=Z:\;C:\WATCOM\BINW;C:\DOSGAME\INCLUDE
SET INCLUDE=C:\WATCOM\H;C:\DOSGAME\INCLUDE
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
rescan
del main.exe
mkdir build
del build\*.*
cd build

wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\engine.cpp
wlib engine +engine

mkdir dist
del dist\tuilib.lib
copy engine.lib dist\tuilib.lib
mkdir dist\include
del dist\include\*.h
copy ..\include\*.h dist\include

wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\main.cpp
wlink @..\proj.lnk
cd ..
copy build\main.exe main.exe