
var fileId = 0; // used by the addFile() function to keep track of IDs
function addElement(parentId, elementTag, elementId, html) {
    // Adds an element to the document
    var p = document.getElementById(parentId);
    var newElement = document.createElement(elementTag);
    newElement.setAttribute('id', elementId);
    newElement.innerHTML = html;
    p.appendChild(newElement);
}

function removeElement(elementId) {
    // Removes an element from the document
    var element = document.getElementById(elementId);
    element.parentNode.removeChild(element);
}

function addFile() {
    fileId++; // increment fileId to get a unique ID for the new element
    var html = '<input type="file" name="uploaded_files[]" multiple = "true"/> ' + 
		'<button type = "button" onclick="removeElement(\'file-'+fileId+'\')\">Remove</button>';
    addElement('files', 'p', 'file-' + fileId, html);
}

