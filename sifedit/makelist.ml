
<DEFAULT
COMPILE=g++ -g -c %SRCFILE% `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o %MODULE%.%OBJ_EXT%
LPREFIX=g++ -o %OUTPUT%
LSUFFIX=`wx-config --libs` -lstdc++
OBJ_EXT=o
OUTPUT=sifedit

>>
sifedit.cpp
MainWindow.cpp
EditArea.cpp
FramesArea.cpp
PreviewArea.cpp
SheetManager.cpp
TreeItemList.cpp
SpriteRecord.cpp
gui/MainGUI.cpp
gencode.cpp

../siflib/sif.cpp
../siflib/sifloader.cpp
../siflib/sectSprites.cpp
../siflib/sectStringArray.cpp

../common/BList.cpp
../common/StringList.cpp
../common/DString.cpp
../common/DBuffer.cpp
../common/bufio.cpp
../common/stat.cpp
../common/misc.cpp
<<
