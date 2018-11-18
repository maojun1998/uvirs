#Set Env
CC=cl
LD=link
AR=lib
CCFLAG=/c /MD /O2
LDFLAG=

all : uvirs.exe server.exe query.exe
    
uvirs.exe : uvirs.obj uvirs.res debug.obj
    $(LD) uvirs.obj uvirs.res Psapi.lib user32.lib advapi32.lib  WS2_32.lib /out:tch.exe
    
uvirs.obj : uvirs.c
    $(CC) $(CCFLAG) uvirs.c

debug.obj : debug.c
    $(CC) $(CCFLAG) debug.c

uvirs.res : uvirs.ico uvirs.rc
    rc uvirs.rc
    
server.exe : server.obj 
    $(LD) server.obj WS2_32.lib
    
server.obj : server.c
    $(CC) $(CCFLAG) server.c
    
query.exe : query.obj
    $(LD)  query.obj
    
query.obj : query.c
    $(CC) $(CCFLAG) query.c

clean :
    del *.exe *.obj 
    
    
    
    