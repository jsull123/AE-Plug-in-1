#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "AutoLayers.h"

// Effect Link

static const A_char* effectLinkScript = (A_char*)R""""(
function main(){
    if (app.project.activeItem == null){
        return;
    }
    var comp = app.project.activeItem;
    if (comp.typeName != "Composition"){
        return;
    }
    var selectedLayers = comp.selectedLayers;
    if (selectedLayers.length < 1){
        alert("Select at least 1 layer");
        return;
    }

   for (var i = 0; i < selectedLayers.length; i++){
        var newLayer = selectedLayers[i].duplicate();
        var newLayerEffects = newLayer.property("Effects");
        for (var j = 1; j <= newLayerEffects.numProperties; j++){
            var currEffect = newLayerEffects.property(j);
            for (var k = 1; k <= currEffect.numProperties; k++){
                var currProperty = currEffect.property(k);
                if (currProperty.canSetExpression){
                    var expression = "comp(\""+comp.name+"\").layer(\""+selectedLayers[i].name+"\").effect(\""+currEffect.name+"\")(\""
                    +currProperty.name+"\").valueAtTime((time-thisLayer.inPoint)+comp(\""+comp.name+"\").layer(\""+selectedLayers[i].name+"\").inPoint)";
                    currProperty.expressionEnabled = true;
                    currProperty.expression = expression;
                }
            }
        }
   }
}
main();

)"""";

// Doesn't make comment
static const A_char* bslMarkerScript = (A_char*)R""""(
function main(){
    if (app.project.activeItem == null){
        return;
    }
    var comp = app.project.activeItem;
    if (comp.typeName != "Composition"){
        return;
    }
    var selectedLayers = comp.selectedLayers;
    if (selectedLayers.length <= 0){
        return;
    } 
    var layer = selectedLayers[0];
    var time = comp.time - layer.inPoint;
    if (time < 0 || comp.time > layer.outPoint) {
        return;
    }
    var frame = Math.round(time / comp.frameDuration);
    var marker = new MarkerValue("");
    layer.property("Marker").setValueAtTime(comp.time, marker);
}
main();
)"""";

// Makes Comment
static const A_char* frameNumMarkerScript = (A_char*)R""""(
function main(){
    if (app.project.activeItem == null){
        return;
    }
    var comp = app.project.activeItem;
    if (comp.typeName != "Composition"){
        return;
    }
    var selectedLayers = comp.selectedLayers;
    if (selectedLayers.length <= 0){
        return;
    } 
    var layer = selectedLayers[0];
    var time = comp.time - layer.inPoint;
    if (time < 0 || comp.time > layer.outPoint) {
        return;
    }
    var frame = Math.round(time / comp.frameDuration);
    var marker = new MarkerValue(frame);
    layer.property("Marker").setValueAtTime(comp.time, marker);
}
main();
)"""";

#endif // !SCRIPTS_H

