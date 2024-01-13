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
