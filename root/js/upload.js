function GenerateDiv(element1, element2){
	
	var info = document.createElement('li');
	info.setAttribute('id', "li_clsfy");
	var textnode = document.createTextNode(element1 + ": " + element2 + " %");
	info.appendChild(textnode);
	document.getElementById('clsfy').appendChild(info);
}
function DeleteDiv(){
	var elem = document.getElementById('li_clsfy');
	elem.remove();
}

