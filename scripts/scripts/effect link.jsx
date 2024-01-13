function main(){
    if (app.project.activeItem == null){
        return;
    }
    var comp = app.project.activeItem;
    if (comp.typeName != "Composition"){
        return;
    }
    var selectedLayers = comp.selectedLayers;
    if (selectedLayers.length < 2){
        alert("Select at least 2 layers");
        return;
    }

   // thisComp.layer("layer 2").effect("Gaussian Blur")("Blurriness").valueAtTime((time-thisLayer.inPoint)+thisComp.layer("layer 2").inPoint)

    var numProperties = selectedLayers[0].property("Effects").numProperties;
    for (var i = 1; i <= numProperties; i++){
        for (var j = 1; j < selectedLayers.length; j++){
            var newEffect = selectedLayers[j].property("Effects").addProperty(selectedLayers[0].property("Effects").property(i).name);
            for (var k = 1; k <= newEffect.numProperties; k++){
                if (newEffect.property(k).canSetExpression){
                    var layerName = selectedLayers[0].name;
                    var effectName = newEffect.name;
                    var propertyName = newEffect.property(k).name;         
                    var expression = "thisComp.layer(\""+layerName+"\").effect(\""+effectName+"\")(\""
                    +propertyName+"\").valueAtTime((time-thisLayer.inPoint)+thisComp.layer(\""+layerName+"\").inPoint)";
                    newEffect.property(k).expressionEnabled = true;
                    newEffect.property(k).expression = expression;
                }
            }
        }
    }
}
main();
