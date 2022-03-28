//wheel to motor
outerDiameter = 20;
width = 1.5;
motorShaftDiameter = 1.5;
tireWidth = 0.5;
tireDepth = 0.5;

$fn=300;

difference() {
    cylinder(d=outerDiameter, h=width, center=true);
    union(){
        //motor shaft
        cylinder(d=motorShaftDiameter, h=width*1.8, center=true);
        //rubber outline
        difference() {
            cylinder(d=outerDiameter*1.2, h=tireWidth, center=true);
            cylinder(d=outerDiameter-tireDepth, h=tireWidth, center=true);
        }
    }
}