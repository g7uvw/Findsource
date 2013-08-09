; ProjectSource
; Calculate the position of the shadow edges projected from a cylinder (mounted on rotation stage) onto a detector
;

pro ProjectSource, sx, sy, d, r, alpha0, q, doff, cx, cy, alpha, p1,p2

;
;  Distances in mm, angles in radians
;
;  sx, sy: Coordinates of X-ray source
;  d:      Perpendicular distance from source to detector
;  r:      Radius of cylinder
;  alpha0: Angle offset of cylinder with respect to rotation stage
;  q:      Distance offset of cylinder with respect to rotation stage
;
;  cx, cy: Coordinates of rotation stage centre
;  alpha:  Angle set on rotation stage
;
;  p1, p2: Distance of edge shadow from detector centre; p2 has the larger y value
;
x = cx + q*cos(alpha + alpha0) - sx
;y = sy - (cy + q*sin(alpha + alpha0));   Y stage is inverted
y =  (cy + q*sin(alpha + alpha0))-sy;   Y stage is not inverted
theta = atan(y/x)
l = sqrt(x*x + y*y)
phi = asin(r/l)
p1 = d*tan(theta - phi)+doff
p2 = d*tan(theta + phi)+doff
;print,p1,p2
end


Function Func, Params
Common Scanresults, CxArr, CyArr, AlphaArr, P1Arr, P2Arr
Error = 0.0D
for a = 0, 26 do begin
 Sx = Params(0)
 Sy = Params(1)
 d  = Params(2)
 r  = Params(3)
 Alpha0 = params(4)
 q  = params(5)
 doff = params(6)

 ProjectSource, Sx, Sy, d, r, Alpha0, q, doff, CxArr(a), CyArr(a), AlphaArr(a), P1, P2
 Error += (P1 - P1Arr(a))^2 + (P2 - P2Arr(a))^2
endfor
print, params, Error
return, Error
end

Pro FindSource
Common Scanresults, CxArr, CyArr, AlphaArr, P1Arr, P2Arr
close,1
close,2
CxArr = dblarr(27)
CyArr = dblarr(27)
AlphaArr = dblarr(27)
P1Arr = dblarr(27)
P2Arr = dblarr(27)
openr,1,"RodEdges.txt"
readf, 1, Sx, Sy, d, r, Alpha0, q, doff
for a = 0, 26 do begin
 readf, 1, t1, t2, t3, t4, t5
 CxArr(a) = t1
 CyArr(a) = t2
 AlphaArr(a) = t3
 P1Arr(a) = t4
 P2Arr(a) = t5
endfor
close,1

Rad = 3.14159/180
AlphaArr *= Rad

FirstEstimate = [Sx, Sy, d, r, Alpha0, q, doff]

FirstEstimate += 0.2 ;    This is just a random offset to check amoeba works
FirstEstimate = double(FirstEstimate)
result = amoeba(1e-11, Scale = [10D, 20D, 50D, 0.2D, 3.2D, 5D,10D], P0 = FirstEstimate, nmax = 100000, ncalls = numberofcalls)
openw,1,"geometry.txt"
print, "Number of calls = ", numberofcalls
printf,1,result[2]/10.0, result[1], result[0]
close,1
Print, "Finished", result

openw,1,"SourcePositionErrors.txt",width = 500
for a = 0, 26 do begin
 ProjectSource, result[0], result[1], result[2], result[3], result[4], result[5], result[6], CxArr(a), CyArr(a), AlphaArr(a), P1, P2
 print,"cheese 11",P1arr[a],p1,P2arr[a],p2, "              Errors in pixels: ",(P1arr[a]-p1)/0.06,(P2arr[a]-p2)/0.06
 printf,1,P1arr[a],p1,P2arr[a],p2, "              Errors in pixels: ",(P1arr[a]-p1)/0.06,(P2arr[a]-p2)/0.06
endfor
close,1


end