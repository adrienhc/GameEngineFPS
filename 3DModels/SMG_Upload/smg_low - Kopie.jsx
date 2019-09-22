#target AfterEffects

/**************************************
Scene : Scene
Resolution : 1200 x 1000
Duration : 10.416667
FPS : 24.000000
Date : 2016-03-30 04:23:02.245566
Exported with io_export_after_effects.py
**************************************/



function compFromBlender(){

var compName = prompt("Blender Comp's Name \nEnter Name of newly created Composition","BlendComp","Composition's Name");
if (compName){
var newComp = app.project.items.addComp(compName, 1200, 1000, 1.000000, 10.416667, 24.000000);
newComp.displayStartTime = 0.083333;


// **************  CAMERA 3D MARKERS  **************


// **************  OBJECTS  **************


var _smg_low = newComp.layers.addNull();
_smg_low.threeDLayer = true;
_smg_low.source.name = "_smg_low";
_smg_low.property("position").setValue([600.000000,500.000000,0.000000],);
_smg_low.property("orientation").setValue([-90.000000,-0.000000,0.000000],);
_smg_low.property("scale").setValue([100.000000,100.000000,100.000000],);


// **************  LIGHTS  **************


// **************  CAMERAS  **************



}else{alert ("Exit Import Blender animation data \nNo Comp's name has been chosen","EXIT")};}


app.beginUndoGroup("Import Blender animation data");
compFromBlender();
app.endUndoGroup();


