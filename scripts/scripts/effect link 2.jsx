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
