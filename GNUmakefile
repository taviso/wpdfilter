include ../GNUmakefile.common

# include ../winutil/GNUmakefile.inc

CPPFLAGS += /DUNICODE /Ilibwpd/inc
LDLIBS += advapi32 ole32 shlwapi libwpd/build/win32/Release/lib/libwpd-0.9


all: wpdfilt64.dll

wpdfilt64.dll: LINKFLAGS+=/DEF:wpdfilter.def
wpdfilt64.dll: filter64.obj callbacks64.obj stream64.obj wpdfilter.res
