
all: sifedit

sifedit:  sifedit.o MainWindow.o EditArea.o FramesArea.o \
	 PreviewArea.o SheetManager.o TreeItemList.o SpriteRecord.o gui/MainGUI.o \
	 gencode.o ../siflib/sif.o ../siflib/sifloader.o ../siflib/sectSprites.o ../siflib/sectStringArray.o \
	 ../common/BList.o ../common/StringList.o ../common/DString.o ../common/DBuffer.o ../common/bufio.o \
	 ../common/stat.o ../common/misc.o
	g++ -o sifedit \
	 sifedit.o MainWindow.o EditArea.o FramesArea.o \
	 PreviewArea.o SheetManager.o TreeItemList.o SpriteRecord.o gui/MainGUI.o \
	 gencode.o ../siflib/sif.o ../siflib/sifloader.o ../siflib/sectSprites.o ../siflib/sectStringArray.o \
	 ../common/BList.o ../common/StringList.o ../common/DString.o ../common/DBuffer.o ../common/bufio.o \
	 ../common/stat.o ../common/misc.o \
	 `wx-config --libs` -lstdc++

sifedit.o:	sifedit.cpp sifedit.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h \
		MainWindow.h gui/MainGUI.h TreeItemList.h
	g++ -g -c sifedit.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o sifedit.o

MainWindow.o:	MainWindow.cpp MainWindow.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h \
		MainWindow.h gui/MainGUI.h TreeItemList.h \
		FramesArea.h PreviewArea.h
	g++ -g -c MainWindow.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o MainWindow.o

EditArea.o:	EditArea.cpp EditArea.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h \
		MainWindow.h gui/MainGUI.h TreeItemList.h
	g++ -g -c EditArea.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o EditArea.o

FramesArea.o:	FramesArea.cpp FramesArea.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h \
		MainWindow.h gui/MainGUI.h TreeItemList.h \
		FramesArea.h
	g++ -g -c FramesArea.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o FramesArea.o

PreviewArea.o:	PreviewArea.cpp PreviewArea.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h \
		PreviewArea.h MainWindow.h gui/MainGUI.h \
		TreeItemList.h
	g++ -g -c PreviewArea.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o PreviewArea.o

SheetManager.o:	SheetManager.cpp SheetManager.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h
	g++ -g -c SheetManager.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o SheetManager.o

TreeItemList.o:	TreeItemList.cpp TreeItemList.fdh TreeItemList.h ../common/BList.h \
		../common/SupportDefs.h
	g++ -g -c TreeItemList.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o TreeItemList.o

SpriteRecord.o:	SpriteRecord.cpp SpriteRecord.fdh SpriteRecord.h ../siflib/sif.h \
		../common/BList.h ../common/SupportDefs.h ../common/DString.h \
		../common/basics.h ../common/DBuffer.h
	g++ -g -c SpriteRecord.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o SpriteRecord.o

gui/MainGUI.o:	gui/MainGUI.cpp gui/MainGUI.fdh gui/MainGUI.h
	g++ -g -c gui/MainGUI.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o gui/MainGUI.o

gencode.o:	gencode.cpp gencode.fdh sifedit.h ../common/BList.h \
		../common/SupportDefs.h ../common/StringList.h ../common/DBuffer.h \
		../common/basics.h ../common/DString.h EditArea.h \
		../siflib/sif.h SheetManager.h SpriteRecord.h \
		../siflib/sifloader.h ../siflib/sectSprites.h ../siflib/sectStringArray.h
	g++ -g -c gencode.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o gencode.o

../siflib/sif.o:	../siflib/sif.cpp ../siflib/sif.fdh ../siflib/sif.h ../siflib/sifloader.h \
		../common/BList.h ../common/SupportDefs.h ../siflib/sectSprites.h \
		../siflib/sectStringArray.h
	g++ -g -c ../siflib/sif.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../siflib/sif.o

../siflib/sifloader.o:	../siflib/sifloader.cpp ../siflib/sifloader.fdh ../siflib/sifloader.h ../common/BList.h \
		../common/SupportDefs.h
	g++ -g -c ../siflib/sifloader.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../siflib/sifloader.o

../siflib/sectSprites.o:	../siflib/sectSprites.cpp ../siflib/sectSprites.fdh ../common/DBuffer.h ../common/basics.h \
		../common/bufio.h ../siflib/sectSprites.h ../siflib/sif.h
	g++ -g -c ../siflib/sectSprites.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../siflib/sectSprites.o

../siflib/sectStringArray.o:	../siflib/sectStringArray.cpp ../siflib/sectStringArray.fdh ../common/DBuffer.h ../common/basics.h \
		../common/DString.h ../common/bufio.h ../common/StringList.h \
		../common/BList.h ../common/SupportDefs.h ../siflib/sectStringArray.h
	g++ -g -c ../siflib/sectStringArray.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../siflib/sectStringArray.o

../common/BList.o:	../common/BList.cpp ../common/BList.fdh ../common/BList.h ../common/SupportDefs.h
	g++ -g -c ../common/BList.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/BList.o

../common/StringList.o:	../common/StringList.cpp ../common/StringList.fdh ../common/StringList.h ../common/BList.h \
		../common/SupportDefs.h
	g++ -g -c ../common/StringList.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/StringList.o

../common/DString.o:	../common/DString.cpp ../common/DString.fdh ../common/DString.h ../common/basics.h \
		../common/DBuffer.h
	g++ -g -c ../common/DString.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/DString.o

../common/DBuffer.o:	../common/DBuffer.cpp ../common/DBuffer.fdh ../common/DBuffer.h ../common/basics.h
	g++ -g -c ../common/DBuffer.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/DBuffer.o

../common/bufio.o:	../common/bufio.cpp ../common/bufio.fdh ../common/DBuffer.h ../common/basics.h
	g++ -g -c ../common/bufio.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/bufio.o

../common/stat.o:	../common/stat.cpp ../common/stat.fdh ../common/basics.h
	g++ -g -c ../common/stat.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/stat.o

../common/misc.o:	../common/misc.cpp ../common/misc.fdh ../common/basics.h
	g++ -g -c ../common/misc.cpp `wx-config --cxxflags` -Wno-multichar -Wno-write-strings -Wunused-variable -o ../common/misc.o


clean:
	rm -f sifedit.o
	rm -f MainWindow.o
	rm -f EditArea.o
	rm -f FramesArea.o
	rm -f PreviewArea.o
	rm -f SheetManager.o
	rm -f TreeItemList.o
	rm -f SpriteRecord.o
	rm -f gui/MainGUI.o
	rm -f gencode.o
	rm -f ../siflib/sif.o
	rm -f ../siflib/sifloader.o
	rm -f ../siflib/sectSprites.o
	rm -f ../siflib/sectStringArray.o
	rm -f ../common/BList.o
	rm -f ../common/StringList.o
	rm -f ../common/DString.o
	rm -f ../common/DBuffer.o
	rm -f ../common/bufio.o
	rm -f ../common/stat.o
	rm -f ../common/misc.o

cleanfdh:
	rm -f sifedit.fdh
	rm -f MainWindow.fdh
	rm -f EditArea.fdh
	rm -f FramesArea.fdh
	rm -f PreviewArea.fdh
	rm -f SheetManager.fdh
	rm -f TreeItemList.fdh
	rm -f SpriteRecord.fdh
	rm -f gui/MainGUI.fdh
	rm -f gencode.fdh
	rm -f ../siflib/sif.fdh
	rm -f ../siflib/sifloader.fdh
	rm -f ../siflib/sectSprites.fdh
	rm -f ../siflib/sectStringArray.fdh
	rm -f ../common/BList.fdh
	rm -f ../common/StringList.fdh
	rm -f ../common/DString.fdh
	rm -f ../common/DBuffer.fdh
	rm -f ../common/bufio.fdh
	rm -f ../common/stat.fdh
	rm -f ../common/misc.fdh

cleanall: clean cleanfdh

