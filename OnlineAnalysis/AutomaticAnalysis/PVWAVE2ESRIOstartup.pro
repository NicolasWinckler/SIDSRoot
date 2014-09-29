; PVWAVE2ESRIOstartup.pro
;
; PVWAVE2ESRIOstartup.pro contains PVWAVE2ESRIO specific PV-WAVE startup
; instructions. These instructions must be executed to activate the
; PVWAVE2ESRIO functions. Include them in the PV-WAVE startup routine
; defined by the environmental variable WAVE_STYARTUP or execute them before
; using a PVWAVE2ESRIO function.

; ESRIO functions
!path = !path+':/home/pbuehler/physics/projects/GO/2014/SIDSRoot/OnlineAnalysis/AutomaticAnalysis/pro'
PVWAVE2ESRIOmain
