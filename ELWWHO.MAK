###############################################################################
#####                 This makefile generates ELWWHO.DLL                 #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

!if $(OS) == DOS
!    if $(MODEL) == LARGE
!         error ELWWHO not supported in Large Model under DOS!
!    endif
     DLLLIB = $(DLLLIB) $(COMPILER)\lib\mathh.lib
     PRODLL = $(PRODLL) $(DOSEXT)\bc5\lib\fpdmy
!else
     PARSEOPTS = $(PARSEOPTS)
!endif

!if $(OS) == WNT
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib
PRODLL =
DEFILE = 
PARSEOPTS = $(PARSEOPTS) -I. -DIS30
!endif

!if $(OS) == WG2
DLLLIB = $(DLLLIB) wgserver.lib galgsbl.lib $(BORLAND)\lib\mathh.lib ${DOSEXT}\bc4\lib\phapi.lib
PRODLL = $(DOSEXT)\bc4\lib\fpdmy
DEFILE = $(WGLIBDIR)\mathdef.def
PARSEOPTS = $(PARSEOPTS) -I. 
!endif

VIRFILES =

!if $(OS) == WNT
OBJFILES = \
     ${TEMPDIR}\elwwho.obj 
!endif

!if $(OS) == WG2
OBJFILES = \
     ${TEMPDIR}\elwwho.obj 
!endif

start:              \
     $(GCBUILD)     \
     elwwho.dll

elwwho.dll:         \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
