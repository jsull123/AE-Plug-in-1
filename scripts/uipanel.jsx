if (typeof JSON !== "object") {
    JSON = {};
}

(function () {
    "use strict";

    var rx_one = /^[\],:{}\s]*$/;
    var rx_two = /\\(?:["\\\/bfnrt]|u[0-9a-fA-F]{4})/g;
    var rx_three = /"[^"\\\n\r]*"|true|false|null|-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?/g;
    var rx_four = /(?:^|:|,)(?:\s*\[)+/g;
    var rx_escapable = /[\\"\u0000-\u001f\u007f-\u009f\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g;
    var rx_dangerous = /[\u0000\u00ad\u0600-\u0604\u070f\u17b4\u17b5\u200c-\u200f\u2028-\u202f\u2060-\u206f\ufeff\ufff0-\uffff]/g;

    function f(n) {
        // Format integers to have at least two digits.
        return (n < 10)
            ? "0" + n
            : n;
    }

    function this_value() {
        return this.valueOf();
    }

    if (typeof Date.prototype.toJSON !== "function") {

        Date.prototype.toJSON = function () {

            return isFinite(this.valueOf())
                ? (
                    this.getUTCFullYear()
                    + "-"
                    + f(this.getUTCMonth() + 1)
                    + "-"
                    + f(this.getUTCDate())
                    + "T"
                    + f(this.getUTCHours())
                    + ":"
                    + f(this.getUTCMinutes())
                    + ":"
                    + f(this.getUTCSeconds())
                    + "Z"
                )
                : null;
        };

        Boolean.prototype.toJSON = this_value;
        Number.prototype.toJSON = this_value;
        String.prototype.toJSON = this_value;
    }

    var gap;
    var indent;
    var meta;
    var rep;


    function quote(string) {

// If the string contains no control characters, no quote characters, and no
// backslash characters, then we can safely slap some quotes around it.
// Otherwise we must also replace the offending characters with safe escape
// sequences.

        rx_escapable.lastIndex = 0;
        return rx_escapable.test(string)
            ? "\"" + string.replace(rx_escapable, function (a) {
                var c = meta[a];
                return typeof c === "string"
                    ? c
                    : "\\u" + ("0000" + a.charCodeAt(0).toString(16)).slice(-4);
            }) + "\""
            : "\"" + string + "\"";
    }


// This variable is initialized with an empty array every time
// JSON.stringify() is invoked and checked by the str() function. It's
// used to keep references to object structures and capture cyclic
// objects. Every new object is checked for its existence in this
// array. If it's found it means the JSON object is cyclic and we have
// to stop execution and throw a TypeError accordingly the ECMA262
// (see NOTE 1 by the link https://tc39.es/ecma262/#sec-json.stringify).

    var seen;

// Emulate [].includes(). It's actual for old-fashioned JScript.

    function includes(array, value) {
        var i;
        for (i = 0; i < array.length; i += 1) {
            if (value === array[i]) {
                return true;
            }
        }
        return false;
    }


    function str(key, holder) {

// Produce a string from holder[key].

        var i;          // The loop counter.
        var k;          // The member key.
        var v;          // The member value.
        var length;
        var mind = gap;
        var partial;
        var value = holder[key];

// If the value has a toJSON method, call it to obtain a replacement value.

        if (
            value
            && typeof value === "object"
            && typeof value.toJSON === "function"
        ) {
            value = value.toJSON(key);
        }

// If we were called with a replacer function, then call the replacer to
// obtain a replacement value.

        if (typeof rep === "function") {
            value = rep.call(holder, key, value);
        }

// What happens next depends on the value's type.

        switch (typeof value) {
        case "string":
            return quote(value);

        case "number":

// JSON numbers must be finite. Encode non-finite numbers as null.

            return (isFinite(value))
                ? String(value)
                : "null";

        case "boolean":
        case "null":

// If the value is a boolean or null, convert it to a string. Note:
// typeof null does not produce "null". The case is included here in
// the remote chance that this gets fixed someday.

            return String(value);

// If the type is "object", we might be dealing with an object or an array or
// null.

        case "object":

// Due to a specification blunder in ECMAScript, typeof null is "object",
// so watch out for that case.

            if (!value) {
                return "null";
            }

// Check the value is not circular object. Otherwise throw TypeError.

            if (includes(seen, value)) {
                throw new TypeError("Converting circular structure to JSON");
            }

// Keep the value for the further check on circular references.

            seen.push(value);

// Make an array to hold the partial results of stringifying this object value.

            gap += indent;
            partial = [];

// Is the value an array?

            if (Object.prototype.toString.apply(value) === "[object Array]") {

// The value is an array. Stringify every element. Use null as a placeholder
// for non-JSON values.

                length = value.length;
                for (i = 0; i < length; i += 1) {
                    partial[i] = str(i, value) || "null";
                }

// Join all of the elements together, separated with commas, and wrap them in
// brackets.

                v = partial.length === 0
                    ? "[]"
                    : gap
                        ? (
                            "[\n"
                            + gap
                            + partial.join(",\n" + gap)
                            + "\n"
                            + mind
                            + "]"
                        )
                        : "[" + partial.join(",") + "]";
                gap = mind;
                return v;
            }

// If the replacer is an array, use it to select the members to be stringified.

            if (rep && typeof rep === "object") {
                length = rep.length;
                for (i = 0; i < length; i += 1) {
                    if (typeof rep[i] === "string") {
                        k = rep[i];
                        v = str(k, value);
                        if (v) {
                            partial.push(quote(k) + (
                                (gap)
                                    ? ": "
                                    : ":"
                            ) + v);
                        }
                    }
                }
            } else {

// Otherwise, iterate through all of the keys in the object.

                for (k in value) {
                    if (Object.prototype.hasOwnProperty.call(value, k)) {
                        v = str(k, value);
                        if (v) {
                            partial.push(quote(k) + (
                                (gap)
                                    ? ": "
                                    : ":"
                            ) + v);
                        }
                    }
                }
            }

// Join all of the member texts together, separated with commas,
// and wrap them in braces.

            v = partial.length === 0
                ? "{}"
                : gap
                    ? "{\n" + gap + partial.join(",\n" + gap) + "\n" + mind + "}"
                    : "{" + partial.join(",") + "}";
            gap = mind;
            return v;
        }
    }

// If the JSON object does not yet have a stringify method, give it one.

    if (typeof JSON.stringify !== "function") {
        meta = {    // table of character substitutions
            "\b": "\\b",
            "\t": "\\t",
            "\n": "\\n",
            "\f": "\\f",
            "\r": "\\r",
            "\"": "\\\"",
            "\\": "\\\\"
        };
        JSON.stringify = function (value, replacer, space) {

// The stringify method takes a value and an optional replacer, and an optional
// space parameter, and returns a JSON text. The replacer can be a function
// that can replace values, or an array of strings that will select the keys.
// A default replacer method can be provided. Use of the space parameter can
// produce text that is more easily readable.

            var i;
            gap = "";
            indent = "";

// If the space parameter is a number, make an indent string containing that
// many spaces.

            if (typeof space === "number") {
                for (i = 0; i < space; i += 1) {
                    indent += " ";
                }

// If the space parameter is a string, it will be used as the indent string.

            } else if (typeof space === "string") {
                indent = space;
            }

// If there is a replacer, it must be a function or an array.
// Otherwise, throw an error.

            rep = replacer;
            if (replacer && typeof replacer !== "function" && (
                typeof replacer !== "object"
                || typeof replacer.length !== "number"
            )) {
                throw new Error("JSON.stringify");
            }

// Initialize the reference keeper.

            seen = [];

// Make a fake root object containing our value under the key of "".
// Return the result of stringifying the value.

            return str("", {"": value});
        };
    }


// If the JSON object does not yet have a parse method, give it one.

    if (typeof JSON.parse !== "function") {
        JSON.parse = function (text, reviver) {

// The parse method takes a text and an optional reviver function, and returns
// a JavaScript value if the text is a valid JSON text.

            var j;

            function walk(holder, key) {

// The walk method is used to recursively walk the resulting structure so
// that modifications can be made.

                var k;
                var v;
                var value = holder[key];
                if (value && typeof value === "object") {
                    for (k in value) {
                        if (Object.prototype.hasOwnProperty.call(value, k)) {
                            v = walk(value, k);
                            if (v !== undefined) {
                                value[k] = v;
                            } else {
                                delete value[k];
                            }
                        }
                    }
                }
                return reviver.call(holder, key, value);
            }


// Parsing happens in four stages. In the first stage, we replace certain
// Unicode characters with escape sequences. JavaScript handles many characters
// incorrectly, either silently deleting them, or treating them as line endings.

            text = String(text);
            rx_dangerous.lastIndex = 0;
            if (rx_dangerous.test(text)) {
                text = text.replace(rx_dangerous, function (a) {
                    return (
                        "\\u"
                        + ("0000" + a.charCodeAt(0).toString(16)).slice(-4)
                    );
                });
            }

// In the second stage, we run the text against regular expressions that look
// for non-JSON patterns. We are especially concerned with "()" and "new"
// because they can cause invocation, and "=" because it can cause mutation.
// But just to be safe, we want to reject all unexpected forms.

// We split the second stage into 4 regexp operations in order to work around
// crippling inefficiencies in IE's and Safari's regexp engines. First we
// replace the JSON backslash pairs with "@" (a non-JSON character). Second, we
// replace all simple value tokens with "]" characters. Third, we delete all
// open brackets that follow a colon or comma or that begin the text. Finally,
// we look to see that the remaining characters are only whitespace or "]" or
// "," or ":" or "{" or "}". If that is so, then the text is safe for eval.

            if (
                rx_one.test(
                    text
                        .replace(rx_two, "@")
                        .replace(rx_three, "]")
                        .replace(rx_four, "")
                )
            ) {

// In the third stage we use the eval function to compile the text into a
// JavaScript structure. The "{" operator is subject to a syntactic ambiguity
// in JavaScript: it can begin a block or an object literal. We wrap the text
// in parens to eliminate the ambiguity.

                j = eval("(" + text + ")");

// In the optional fourth stage, we recursively walk the new structure, passing
// each name/value pair to a reviver function for possible transformation.

                return (typeof reviver === "function")
                    ? walk({"": j}, "")
                    : j;
            }

// If the text is not JSON parseable, then a SyntaxError is thrown.

            throw new SyntaxError("JSON.parse");
        };
    }
}());

var layerTypes = [
    "Adjustment Layer",
    "Solid Layer",
    "Null Layer"
];

var commandCalls = {
    crunchKF: "crunchKF",
    effectLink: "effectLink"
};

var settingsFile = new File("C:\\Users\\DeMarcus Cousins\\Documents\\ae plugins (dev)\\newest plugin\\settings\\settings.json");
var settings = readJson(settingsFile);

var dialog = createDockableUI(this); 
    dialog.text = "105717"; 
    dialog.preferredSize.width = 199; 
    dialog.preferredSize.height = 81; 
    dialog.orientation = "column"; 
    dialog.alignChildren = ["center","top"]; 
    dialog.spacing = 10; 
    dialog.margins = 16; 

var tpanel1 = dialog.add("tabbedpanel", undefined, undefined, {name: "tpanel1"}); 
tpanel1.alignChildren = "fill"; 
tpanel1.preferredSize.width = 184.156; 
tpanel1.margins = 0; 

var tabNames = [
    "Layers",
    "Markers",
    "Time Remap",
];

// LAYERS TAB
{
    var layersTab = tpanel1.add("tab", undefined, undefined, {name: "layersTab"}); 
        layersTab.text = tabNames[0]; 
        layersTab.orientation = "column"; 
        layersTab.alignChildren = ["left","top"]; 
        layersTab.spacing = 10; 
        layersTab.margins = 10;

    var layersTabDiv1 = layersTab.add("panel", undefined, undefined, {name: "divider1"}); 
        layersTabDiv1.alignment = "fill"; 

    var addLayersLabelText = layersTab.add("statictext", undefined, undefined, {name: "statictext1"}); 
        addLayersLabelText.enabled = false; 
        addLayersLabelText.text = "Add Layers"; 
        addLayersLabelText.justify = "center";
        addLayersLabelText.alignment = ["center","top"]; 

    var layerTypeGroup = layersTab.add("group", undefined, {name: "layerTypeGroup"}); 
        layerTypeGroup.orientation = "row"; 
        layerTypeGroup.alignChildren = ["left","center"]; 
        layerTypeGroup.spacing = 10; 
        layerTypeGroup.margins = 0; 

    var layerTypeText = layerTypeGroup.add("statictext", undefined, undefined, {name: "layerTypeText"}); 
        layerTypeText.text = "Layer Type"; 

    // Layer Type Dropdown
    var layerTypeDropdown = layerTypeGroup.add("dropdownlist", undefined, undefined, {name: "layerTypeDropdown", items: layerTypes}); 
        layerTypeDropdown.selection = settings.layerType; 
        layerTypeDropdown.onChange = function () { 
            settings.layerType = layerTypeDropdown.selection.index;
            writeFile(settingsFile, JSON.stringify(settings));
        };

    var durationGroup = layersTab.add("group", undefined, {name: "durationGroup"}); 
        durationGroup.orientation = "row"; 
        durationGroup.alignChildren = ["left","center"]; 
        durationGroup.spacing = 10; 
        durationGroup.margins = 0; 

    var durationText = durationGroup.add("statictext", undefined, undefined, {name: "durationText"}); 
        durationText.text = "Duration"; 

    // Duration Dropdown
    var durationDropdown_array = ["Default","Custom"]; 
    var durationDropdown = durationGroup.add("dropdownlist", undefined, undefined, {name: "durationDropdown", items: durationDropdown_array}); 
        durationDropdown.selection = Boolean(settings.bCustomDuration); 
        durationDropdown.onChange = function () {
            durationEditText.enabled = true;
            if (durationDropdown.selection.index == 0){
                durationEditText.enabled = false;
            }
            settings.bCustomDuration = Boolean(durationDropdown.selection.index);
            writeFile(settingsFile, JSON.stringify(settings));
        };

    // Duration Edit Text
    var durationEditText = durationGroup.add('edittext {properties: {name: "durationEditText"}}'); 
        durationEditText.text = settings.customDuration; 
        durationEditText.preferredSize.width = 30;
        durationEditText.enabled = Boolean(settings.bCustomDuration);
        durationEditText.onChanging = function () {
            settings.customDuration = parseInt(durationEditText.text);
            writeFile(settingsFile, JSON.stringify(settings));
        };

    var enabledGroup = layersTab.add("group", undefined, {name: "enabledGroup"}); 
        enabledGroup.orientation = "row"; 
        enabledGroup.alignChildren = ["left","center"]; 
        enabledGroup.spacing = 10; 
        enabledGroup.margins = 0; 

    // Add Layers Enabled Checkbox
    var enabledCheckbox = enabledGroup.add("checkbox", undefined, undefined, {name: "enabledCheckbox"}); 
        enabledCheckbox.text = "Enabled"; 
        enabledCheckbox.alignment = ["left","center"];
        enabledCheckbox.value = settings.bAddLayers;
        enabledCheckbox.onClick = function () {
            settings.bAddLayers = enabledCheckbox.value;
            writeFile(settingsFile, JSON.stringify(settings));
        };

    var layersTabDiv1 = layersTab.add("panel", undefined, undefined, {name: "divider2"}); 
        layersTabDiv1.alignment = "fill"; 

    var cascadeButtonGroup = layersTab.add("group", undefined, {name: "group4"}); 
        cascadeButtonGroup.orientation = "row"; 
        cascadeButtonGroup.alignChildren = ["left","center"]; 
        cascadeButtonGroup.spacing = 10; 
        cascadeButtonGroup.margins = 0; 

    var effectLink = cascadeButtonGroup.add("button", undefined, undefined, {name: "fpsRemapButton"}); 
        effectLink.text = "Effect Link";
        effectLink.onClick = function () {
            settings.commandCall = commandCalls.effectLink;
            writeFile(settingsFile, JSON.stringify(settings));
        };
}
// MARKERS TAB
{
    var markersTab = tpanel1.add("tab", undefined, undefined, {name: "markersTab"}); 
    markersTab.text = tabNames[1]; 
    markersTab.orientation = "column"; 
    markersTab.alignChildren = ["left","top"]; 
    markersTab.spacing = 10; 
    markersTab.margins = 10;

    // Marker Snap Enabled Checkbox
    var markerSnapEnabled = markersTab.add("checkbox", undefined, undefined, {name: "checkbox2"}); 
        markerSnapEnabled.text = "Marker Snap Enabled"; 
        markerSnapEnabled.value = settings.bMarkerSnap;
        markerSnapEnabled.onClick = function () {
            settings.bMarkerSnap = markerSnapEnabled.value;
            writeFile(settingsFile, JSON.stringify(settings));
        };

    var markersTabDivider1 = markersTab.add("panel", undefined, undefined, {name: "divider3"}); 
        markersTabDivider1.alignment = "fill"; 

    var addWBslTitle = markersTab.add("statictext", undefined, undefined, {name: "statictext4"}); 
        addWBslTitle.enabled = false; 
        addWBslTitle.text = "Add markers w bsl"; 
        addWBslTitle.preferredSize.width = 118;
        addWBslTitle.preferredSize.height = 21; 
        addWBslTitle.alignment = ["center","top"]; 

    // BSL Markers Enabled Checkbox
    var addWBslEnabled = markersTab.add("checkbox", undefined, undefined, {name: "checkbox3"}); 
        addWBslEnabled.text = "Enabled";
        addWBslEnabled.value = settings.bBSLMarkers;
        addWBslEnabled.onClick = function(){
            settings.bBSLMarkers = addWBslEnabled.value;   
            writeFile(settingsFile, JSON.stringify(settings));
        };

    // Frame Comments Checkbox
    var frameCommentsCheckbox = markersTab.add("checkbox", undefined, undefined, {name: "checkbox4"}); 
        frameCommentsCheckbox.text = "Add Frame Comments";
        frameCommentsCheckbox.value = settings.bBSLComments;
        frameCommentsCheckbox.onClick = function(){
            settings.bBSLComments = frameCommentsCheckbox.value;   
            writeFile(settingsFile, JSON.stringify(settings));
        };
}
// TIME REMAP TAB
{
    var timeRemapTab = tpanel1.add("tab", undefined, undefined, {name: "timeRemapTab"}); 
        timeRemapTab.text = tabNames[2]; 
        timeRemapTab.orientation = "column"; 
        timeRemapTab.alignChildren = ["left","top"]; 
        timeRemapTab.spacing = 10; 
        timeRemapTab.margins = 10;

    var crunchKFButton = timeRemapTab.add("button", undefined, undefined, {name: "crunchKFButton"}); 
        crunchKFButton.text = "Crunch KF";
        crunchKFButton.onClick = function () {
            settings.commandCall = commandCalls.crunchKF;
            writeFile(settingsFile, JSON.stringify(settings));
        };
}   

tpanel1.selection = layersTab;
/*
var tabsArray = [
    layersTab, 
    markersTab, 
    timeRemapTab, 
    renderTab
];

tpanel1.selection = tabsArray[settings.panelSelection];
tpanel1.onChange = function () { 
    settings.panelSelection = find(tabNames, tpanel1.selection.text);
    writeFile(settingsFile, JSON.stringify(settings));
};
*/

showWindow(dialog);

function find(arr, value){
    for (var i = 0; i < arr.length; i++){
        if (arr[i] == value) return i;
    }
    return 0;
}

function readJson(fileObj) {
    
    var currentLine;
    var jsonStuff = [];
    fileObj.open("r");
        while(!fileObj.eof) {
                currentLine = fileObj.readln();
                jsonStuff.push(currentLine);
            }
        fileObj.close();
        jsonStuff = jsonStuff.join("");
        var parsedJson = JSON.parse(jsonStuff);
        
        return parsedJson;      
}

function readFile(fileObj, encoding){
    
    encoding = encoding || "utf-8";

    fileObj = (fileObj instanceof File) ? fileObj : new File(fileObj);

    fileObj.encoding = encoding;

    fileObj.open("r");

    var contents = fileObj.read();

    fileObj.close();

    return contents;
}

function writeFile(fileObj, fileContent, encoding) {
    encoding = encoding || "utf-8";

    fileObj = (fileObj instanceof File) ? fileObj : new File(fileObj);

    var parentFolder = fileObj.parent;

    if (!parentFolder.exists && !parentFolder.create())

        throw new Error("Cannot create file in path " + fileObj.fsName);

    fileObj.encoding = encoding;

    fileObj.open("w");

    fileObj.write(fileContent);

    fileObj.close();

    return fileObj;
}

function canWriteFiles() {

    if (isSecurityPrefSet()) return true;

    alert("Plugin requires access to write files.\nEnable \"Allow Scripts to Write Files and Access Network\" in the Scripting & Expressions preferences.");

    app.executeCommand(2359);

    return isSecurityPrefSet();

    function isSecurityPrefSet() {

        return app.preferences.getPrefAsLong(

            "Main Pref Section",

            "Pref_SCRIPTING_FILE_NETWORK_SECURITY"

        ) === 1;

    }
}

function createDockableUI(thisObj){
    var dialog = thisObj instanceof Panel ? thisObj : 
    new Window("window", undefined, undefined, {resizeable: true});

    dialog.onResizing = dialog.onResize = function () {
        this.layout.resize();
    }
    return dialog;
}

function showWindow(window){
    if (window instanceof Window){
        window.center();
        window.show();
    }
    if (window instanceof Panel){
        window.layout.layout(true);
        window.layout.resize();
    }
}
