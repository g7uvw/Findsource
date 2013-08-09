

pro findedges, im, range, edge, edgearray
temp = size(im, /dimensions)
xdim = temp[0]
ydim = temp[1]

; De-pepper image
im1 = im - smooth(im,3)
pepper = where(im1 gt 1, count)
print,"pepper count = ",count
if count gt 0 then begin
 for a = 0, count-1 do begin
  y = pepper[a]/xdim
  x = pepper[a] - y*xdim
  im[x,y] = (im[x,y-2]+im[x,y+2])/2.0
 endfor
endif
ystart = (ydim-range)/2
yend = ystart + range -1

edge = fltarr(2)
edge[0] = 0
edge[1] = 0

a = 0
for y = ystart, yend do begin
 x = 1
 repeat ++x until im[x,y] lt 0.5

 edge[0] += x
 h = (0.5 - im[x,y])/(im[x-1,y] - im[x,y])
 edge[0] -= h
 edgearray(a,0) = x-h
 x += 10
 repeat ++x until im[x,y] gt 0.5

 edge[1] += x
 h = (im[x,y] - 0.5)/(im[x,y] - im[x-1,y])
 edge[1] -= h
 edgearray(a,1) = x-h
 ++a
endfor

edge /= range
end



xdim = 1020
ydim = 800
close,1
close,2

range = 500
widtharray = fltarr(range,27)
edgearray = fltarr(range,2)
dark=uintarr(xdim,ydim)
light=uintarr(xdim,ydim)
proj=uintarr(xdim,ydim,27)

header = intarr(256)
FileName = ""
read,prompt="Enter source file name without extension:  ",FileName
openr,1,FileName+".mcd"
readu,1,header
readu,1,dark
readu,1,light
readu,1,proj
close,1

dark(xdim-1,*) = dark(xdim-2,*)
light(xdim-1,*) = light(xdim-2,*)
proj(xdim-1,*,*) = proj(xdim-2,*,*)

light -= dark

for a = 0,26 do proj(*,*,a) -= dark
proj = float(proj)
for a = 0,26 do proj(*,*,a)/=light


openr,1,"ScanForSource.txt"
openw,2,"RodEdges.txt"
readf,1,t1,t2,t3,t4,t5,t6,t7
printf,2,t1,t2,t3,t4,t5,t6,t7

for a = 0,26 do begin
 readf,1,t1,t2,t3,t4,t5
 findedges, proj[*,*,a], range, edge,edgearray
 widtharray(*,a) = edgearray(*,1) - edgearray(*,0)
 edge -= (xdim-1)/2.0; Subtract centre of detector
 edge *= 0.06    ;     Multiply by pixel size
 printf,2,t1,t2,t3,edge[0], edge[1]
endfor
close,1
close,2
end
