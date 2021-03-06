pro PVWAVE2ESRIOinit, PVWAVE2ESRIOmaindir
;
; initializes PVWAVE2ESRIOmain.pro and PVWAVE2ESRIOstartup.pro
; PVWAVE2ESRIOstartup.pro contains PVWAVE2ESRIO PV-WAVE startup
; instructions which must be executed prior to usage of the PVWAVE2ESRIO
; functions. PVWAVE2ESRIOstartup.pro also includes a call to
; PVWAVE2ESRIOmain, which initialises the PVWAVE2ESRIO specific common
; block ESRIOcommon
;
declare func, PVWAVE2ESRIOmkstartup
declare func, PVWAVE2ESRIOmkmain


; create the programs
if (PVWAVE2ESRIOmkmain(PVWAVE2ESRIOmaindir) lt 0) then return
if (PVWAVE2ESRIOmkstartup(PVWAVE2ESRIOmaindir) lt 0) then return

return
end

; -----------------------------------------------------------------------------
function PVWAVE2ESRIOmkstartup, PVWAVE2ESRIOmaindir
;
; creates the file PVWAVE2ESRIOstartup.pro.
; PVWAVE2ESRIOstartup.pro contains PVWAVE2ESRIO specific PV-WAVE startup
; instructions. These instructions must be executed before PVWAVE2ESRIO is
; used. Include them in the PV-WAVE startup routine defined by the
; environmental variable WAVE_STARTUP or execute them to activate the
; PVWAVE2ESRIO functions.
;

; make sure that PVWAVE2ESRIOmaindir ends with a `/'
dnlen=strlen(PVWAVE2ESRIOmaindir)
if (strmid(PVWAVE2ESRIOmaindir,dnlen-1,1) ne "/") then $
  PVWAVE2ESRIOmaindir=PVWAVE2ESRIOmaindir+"/"

; check if PVWAVE2ESRIOmaindir and all the subdirectories exist
dirs=PVWAVE2ESRIOmaindir+['','c','pro']
ndirs=n_elements(dirs)

for i=0,ndirs-1 do begin
  stat=checkfile(dirs(i),/write,is_dir=is_dir)
  if (stat ne 1 or is_dir ne 1) then begin
    print
    print, "Error: The directory, "+dirs(i)+" can not be accessed!"
    print, "The procedure is stopped and no output is created!"
    print
    return, -2
  endif
endfor


; create PVWAVE2ESRIOstartup.pro
PVWAVE2ESRIO_PROGS=PVWAVE2ESRIOmaindir+"pro"
fname=PVWAVE2ESRIOmaindir+'PVWAVE2ESRIOstartup.pro'
openw,fid,fname,error=error,/get_lun
if (error ne 0) then begin
  print
  print, "Error: The file, "+fname+" could not be opened!"
  print, "The procedure is stopped and no output is created!"
  print
  return, -1
endif
  
printf,fid,"; PVWAVE2ESRIOstartup.pro"
printf,fid,";"
printf,fid,"; PVWAVE2ESRIOstartup.pro contains PVWAVE2ESRIO specific "+	$
  "PV-WAVE startup"
printf,fid,"; instructions. These instructions must be executed to "+		$
  "activate the"
printf,fid,"; PVWAVE2ESRIO functions. Include them in the PV-WAVE "+	$
  "startup routine"
printf,fid,"; defined by the environmental variable WAVE_STYARTUP or "+	$
  "execute them before"
printf,fid,"; using a PVWAVE2ESRIO function."
printf,fid,""
printf,fid,"; ESRIO functions
printf,fid,"!path = !path+':"+PVWAVE2ESRIO_PROGS+"'"
printf,fid,"PVWAVE2ESRIOmain"


; close PVWAVE2ESRIOstartup.pro
close,fid
free_lun,fid

return, 0
end

; -----------------------------------------------------------------------------
function PVWAVE2ESRIOmkmain, PVWAVE2ESRIOmaindir
;
; creates the file PVWAVE2ESRIOmain.pro.
; PVWAVE2ESRIOmain.pro is used to initialise the PVWAVE2ESRIO specific common block
; ESRIOcommon
;

; make sure that PVWAVE2ESRIOmaindir ends with a `/'
dnlen=strlen(PVWAVE2ESRIOmaindir)
if (strmid(PVWAVE2ESRIOmaindir,dnlen-1,1) ne "/") then $
  PVWAVE2ESRIOmaindir=PVWAVE2ESRIOmaindir+"/"

; check if PVWAVE2ESRIOmaindir and all the subdirectories exist
dirs=PVWAVE2ESRIOmaindir+['','c','pro']
ndirs=n_elements(dirs)

for i=0,ndirs-1 do begin
  stat=checkfile(dirs(i),/write,is_dir=is_dir)
  if (stat ne 1 or is_dir ne 1) then begin
    print
    print, "Error: The directory, "+dirs(i)+" can not be accessed!"
    print, "The procedure is stopped and no output is created!"
    print
    return, -2
  endif
endfor


; create PVWAVE2ESRIOmain.pro
PVWAVE2ESRIO_PROGS=PVWAVE2ESRIOmaindir+"pro"
fname=PVWAVE2ESRIO_PROGS+"/PVWAVE2ESRIOmain.pro"
openw,fid,fname,error=error,/get_lun
if (error ne 0) then begin
  print
  print, "Error: The file, "+fname+" could not be opened!"
  print, "The procedure is stopped and no output is created!"
  print
  return, -1
endif
  

printf,fid,";+ --------------------------------------------------------"+	$
  "---------------"
printf,fid,"; Author: P. Buehler"
printf,fid,"; Date: Summer 2014"
printf,fid,";"
printf,fid,"; NAME: PVWAVE2ESRIOmain"
printf,fid,"; PURPOSE:      initialise the ESRIOobject"
printf,fid,";"                           
printf,fid,"; CALLING SEQUENCE:"
printf,fid,";               PVWAVE2ESRIOmain"
printf,fid,"; INPUTS:"
printf,fid,";"
printf,fid,"; OUTPUTS:"
printf,fid,";"
printf,fid,";- ---------------------------------------------------------"+	$
  "--------------"
printf,fid,"pro PVWAVE2ESRIOmain"
printf,fid,";"
printf,fid,";"
printf,fid,";"
printf,fid,"common ESRIOcommon, ESRIOobject"
printf,fid,"ESRIOobject='"+PVWAVE2ESRIOmaindir+"c/PVWAVE2ESRIO.so"
printf,fid,""
printf,fid,"end"
printf,fid,""
printf,fid,";------------------------------------------------------------"+	$
  "-------------"


; close PVWAVE2ESRIOmain.pro
close,fid
free_lun,fid

return, 0
end

; -----------------------------------------------------------------------------
PVWAVE2ESRIOinit, '/home/pbuehler/physics/projects/GO/2014/SIDSRoot/OnlineAnalysis/AutomaticAnalysis'

end

; -----------------------------------------------------------------------------
