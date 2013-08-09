; ProjectSource
; Calculate the position of the shadow edges projected from a cylinder (mounted on rotation stage) onto a detector
;

pro ProjectSource, sx, sy, d, r, alpha0, q, doff, cx, cy, alpha,p1,p2

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
y = sy - (cy + q*sin(alpha + alpha0)) ; Y stage is inverted

theta = atan(y/x)
l = sqrt(x*x + y*y)
phi = asin(r/l)
p1 = d*tan(theta - phi)+doff
p2 = d*tan(theta + phi)+doff

end

pro SourceProject
Rad = 3.14159/180

openr,1,"geometry.txt"
readf,1,d,sy,sx
close,1
d *= 10.0

;Sx = 23.3605
;Sy = 136.0
;d = 250.0
r = 2.0D
alpha0 = 0.0D
q = 0.0D
doff = 0.0D
print, "starting"
openw,1,"ScanForSource.txt", width = 500
printf,1,Sx, Sy, d, r, alpha0, q, doff

alpha = 0.0
for x = 30, 80, 25 do begin
 for p = -10, 10, 10 do begin
  for a = 0, 2 do begin
   y = p*1.0 * x/250.0
   Cx = x+Sx
   Cy = Sy-y
   ProjectSource, Sx, Sy, d, r, alpha0, q, doff, Cx, Cy, alpha, P1, P2
   print, x,p,a
   printf, 1, Cx, Cy, alpha, p1, p2
   alpha = (alpha + 133.33333333333) mod 360
  endfor
 endfor
endfor
close,1
print, "Finished."
end


