var segLength = 10;
var folder;
var hasSelected = false;

// DIALOG
// ======
var dialog = new Window("dialog", undefined, undefined, {resizeable: true}); 
    dialog.text = "Dialog"; 
    dialog.preferredSize.width = 235; 
    dialog.preferredSize.height = 78; 
    dialog.orientation = "column"; 
    dialog.alignChildren = ["center","top"]; 
    dialog.spacing = 10; 
    dialog.margins = 16;

// GROUP1
// ======
var group1 = dialog.add("group", undefined, {name: "group1"}); 
    group1.orientation = "row"; 
    group1.alignChildren = ["left","center"]; 
    group1.spacing = 10; 
    group1.margins = 0; 

var edittext1 = group1.add('edittext {properties: {name: "edittext1"}}'); 
    edittext1.text = "Selected Folder Location"; 
    edittext1.preferredSize.width = 155;

var button1 = group1.add("button", undefined, undefined, {name: "button1"}); 
    button1.text = "File..."; 
    button1.onClick = function(){
        //file = file.openDlg("Select Folder", "Acceptable Files:*.dir");
        folder = Folder.selectDialog("Select Folder");
        //edittext1.text = file.fsName;
        if (folder == null){
            return;
        }
        hasSelected = true;
        edittext1.text = folder.fsName;
    };

// GROUP2
// ======
var group2 = dialog.add("group", undefined, {name: "group2"}); 
    group2.orientation = "row"; 
    group2.alignChildren = ["left","center"]; 
    group2.spacing = 10;
    group2.margins = 0;

var statictext1 = group2.add("statictext", undefined, undefined, {name: "statictext1"}); 
    statictext1.text = "Segment Length"; 

var edittext2 = group2.add('edittext {properties: {name: "edittext2"}}'); 
    edittext2.text = "10";
    edittext2.preferredSize.width = 25;
    edittext2.onChanging = function() {
        segLength = parseInt(edittext2.text);
    };

var statictext2 = group2.add("statictext", undefined, undefined, {name: "statictext2"}); 
    statictext2.text = "(Frames)"; 

// GROUP3
// ======
var group3 = dialog.add("group", undefined, {name: "group3"}); 
group3.orientation = "row"; 
group3.alignChildren = ["left","center"]; 
group3.spacing = 10;
group3.margins = 0; 

var button2 = group3.add("button", undefined, undefined, {name: "button2"}); 
button2.text = "Done";
button2.onClick = function(){
    // Add to queue
    if (!hasSelected){
        alert("Please select a folder");
        return;
    }
    if (!folder.exists){
        alert("Could not find folder \""+folder.fsName+"\"");
        return;
    }

    if (app.project.activeItem == null){
        alert("Could not find active composition");
        dialog.close();
        return;
    }
    var comp = app.project.activeItem;
    if (comp.typeName != "Composition"){
        alert("Could not find active composition");
        dialog.close();
        return;
    }

    var segDuration = comp.frameDuration*segLength;
    var startPoint = comp.workAreaStart;
    var n = 0;

    while (startPoint+segDuration <= comp.workAreaStart+comp.workAreaDuration){     
        var rqItem = app.project.renderQueue.items.add(comp);
        rqItem.timeSpanStart = startPoint;
        rqItem.timeSpanDuration = segDuration;
    
        var ns = "";
        for (var i = 0; i < 2-n.toString().length; i++){
            ns+="0";
        }
        ns+=n;
        if (rqItem.numOutputModules <= 0) continue;
        rqItem.outputModules[1].file = new File(folder.fsName+"\\"+comp.name+"_"+ns+".mov");
        startPoint += segDuration;
        n++;
    }
    
    var a = (comp.workAreaStart+comp.workAreaDuration).toFixed(4);
    var b = startPoint.toFixed(4);
    if (a > b){
        var rqItem = app.project.renderQueue.items.add(comp);
        rqItem.timeSpanStart = startPoint;
        rqItem.timeSpanDuration = comp.workAreaStart+comp.workAreaDuration-startPoint;
        
        var ns = "";
        for (var i = 0; i < 2-n.toString().length; i++){
            ns+="0";
        }
        ns+=n;
        if (rqItem.numOutputModules > 0){
            rqItem.outputModules[1].file = new File(folder.fsName+"\\"+comp.name+"_"+ns+".mov");
        }
    }

    dialog.close();
}; 

var button3 = group3.add("button", undefined, undefined, {name: "button3"}); 
button3.text = "Cancel"; 
button3.onClick = function(){
    dialog.close();
};

dialog.show();