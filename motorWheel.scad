//wheel to motor
outerDiameter = 50;
width = 12;
motorShaftDiameter = 2.02+0.5; // ignore offset just precise for press fit.
tireWidth = width-width*0.2; //width of tire
tireDepth = 1.5; //depth of tire

//so that the wheel is a bit further away from the motor
wheelOffset = 18; //debt of ofset from motor shaft to wheel
offsetDiampeter = 20; //thickness for offset from motor shaft to wheel


$fn=300; //global resolution
difference() {
    union(){
        cylinder(d=outerDiameter, h=width, center=true);
        translate([0, 0, (width/2)-0.000001]) {
            cylinder(d=offsetDiampeter, h=wheelOffset, center=true);
        }
    }
    union(){
        //motor shaft
        cylinder(d=motorShaftDiameter, h=width+wheelOffset+1, center=true);
        //rubber outline
        difference() {
            cylinder(d=outerDiameter*1.2, h=tireWidth, center=true);
            cylinder(d=outerDiameter-tireDepth, h=tireWidth, center=true);
        }
    }
}
