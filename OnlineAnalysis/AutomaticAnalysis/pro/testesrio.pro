; ----------------------------------------------------------------------------
pro testesrio, outfname,datafname,dtb,dfb,foff,qq,fdm,numions,decdata
;
;
;
; ............................................................................
stat = write_esrinjdata(outfname,datafname, $
  dtb,dfb,foff,qq,fdm,numions,decdata)

return
end

; ----------------------------------------------------------------------------
outfname  = 'myout.root'
datafname = 'mydata.root'

dtb  = 32.D-3
dfb  = 3.125D1
foff = 2.44D8
qq = 1
fdm = [350,400]
numions = [3.,1.,2.]

; decdata = dblarr(4,n)
;  decdata(0,*) = decay time (time bins)
;  decdata(1,*) = decay frequency (frequency bins)
;  decdata(2,*) = arrival time (time bins)
;  decdata(3,*) = arrival frequency (frequency bins)
decdata      = dblarr(4,6)
decdata(*,0) = [100,405,105,416]
decdata(*,1) = [362,401,367,416]
decdata(*,2) = [445,409,450,416]
decdata(*,3) = [123,407,129,416]
decdata(*,4) = [943,402,945,416]
decdata(*,5) = [423,401,434,416]

testesrio, outfname,datafname,dtb,dfb,foff,qq,fdm,numions,decdata

stat = get_numinjdata(outfname,ndata)
print,"Number of EsrInjData: ",ndata

decaytype = 1
stat = read_esrinjdata(outfname,decaytype,resstruct)
print,resstruct.m0

stop

return
end

; ----------------------------------------------------------------------------
