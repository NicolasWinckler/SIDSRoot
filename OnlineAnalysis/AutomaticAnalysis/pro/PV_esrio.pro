;+ -----------------------------------------------------------------------
; Author: P. Buehler
; Date: Summer 2014
;
; NAME:	PV_esrio
; PURPOSE:	
;                           
; CALLING SEQUENCE:
;		stat=write_esrinjdata( fname, decdata )
; INPUTS:
;
;
; OUTPUTS:
;		
;- -----------------------------------------------------------------------
function get_esriostruct
;
;
;.........................................................................
;
; prepare resstruct
tmpstruct = {,      $
  fn:   '',         $
  dd:   today(),    $
  dtb:  0.D0,       $
  dfb:  0.D0,       $
  foff: 0.D0,       $
  qq:   -9,         $
  fdm:  dblarr(2),  $
	m0:   dblarr(3),  $
  ndec: 0,          $
  tdec: dblarr(20), $
  fdec: dblarr(20), $
  tarr: dblarr(20), $
  farr: dblarr(20)  }
  
return, tmpstruct

end

;- -----------------------------------------------------------------------
function get_numinjdata, outfname,ndata
;
;
;
common ESRIOcommon, ESRIOobject

; initialisations
ndata = 0L

; read number of data points in outfname
status = LINKNLOAD(ESRIOobject,"_Z13getnumdata_cwiPPc",	$
  outfname,long(ndata))
      
return, status
end

;-------------------------------------------------------------------------
function write_esrinjdata, outfname,datafname,  $
  dtb,dfb,foff,qq,fmd,numions,decdata
;
;
;
common ESRIOcommon, ESRIOobject

; initialisations
; decdata must be a dblarr(4,ndecays)
ss = size(decdata)
if (ss(1) ne 4) then return, -1
ndecays = 1
if (ss(0) eq 2) then ndecays = ss(2)

; write data
status = LINKNLOAD(ESRIOobject,"_Z12writedata_cwiPPc",	$
  outfname,datafname,double(dtb),double(dfb),double(foff),	$
	long(qq),double(fmd),double(numions),long(ndecays),double(decdata))
      
return, status
end

;-------------------------------------------------------------------------
function read_esrinjdata, outfname,decaytype,resstruct
;
;
;
common ESRIOcommon, ESRIOobject

; initialisations
esrio_maxnumdecays = 20

stat = get_numinjdata(outfname,ndata)
if (stat lt 0) then return, stat

; preapre buffers
dtbs    = dblarr(ndata)
dfbs    = dblarr(ndata)
foffs   = dblarr(ndata)
qq      = lonarr(ndata)
fdm     = dblarr(2,ndata)
numions = dblarr(3,ndata)
decdata = dblarr(4,esrio_maxnumdecays,ndata) - 1.

allusers =''
for ii=0L,50L*ndata do allusers=allusers+' '
allfilenames =''
for ii=0L,50L*ndata do allfilenames=allfilenames+' '

; read data
status = LINKNLOAD(ESRIOobject,"_Z11readdata_cwiPPc",	$
  outfname,long(decaytype), $
  allusers,allfilenames, $
  dtbs,dfbs,foffs,qq,fdm,numions,decdata)

; extract users
users = strsplit(allusers,'//')
nfn = n_elements(users)
users = users(1:nfn-2)

; extract filenames
filenames = strsplit(allfilenames,'//')
nfn = n_elements(filenames)
filenames = filenames(1:nfn-2)

; prepare resstruct
tmpstruct = get_esriostruct()
resstruct = replicate(tmpstruct,ndata)

for ii=0,ndata-1 do begin
  
  resstruct(ii).fn   = filenames(ii)
  resstruct(ii).dtb  = dtbs(ii)
  resstruct(ii).dfb  = dfbs(ii)
  resstruct(ii).foff = foffs(ii)
  resstruct(ii).qq   = qq(ii)
  resstruct(ii).fdm  = fdm(*,ii)
  resstruct(ii).m0   = numions(*,ii)

  sel = where(decdata(0,*,ii) gt 0,ndec)
  resstruct(ii).ndec = ndec
  if (ndec gt 0) then begin
    resstruct(ii).tdec(0:ndec-1) = decdata(0,0:ndec-1,ii)
    resstruct(ii).fdec(0:ndec-1) = decdata(1,0:ndec-1,ii)
    resstruct(ii).tarr(0:ndec-1) = decdata(2,0:ndec-1,ii)
    resstruct(ii).farr(0:ndec-1) = decdata(3,0:ndec-1,ii)
  endif
endfor

return, status
end

;-------------------------------------------------------------------------
