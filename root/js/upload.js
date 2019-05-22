function GenerateDiv(element1, elemet2){
	
	var info = document.createElement('li');
	info.setAttribute('id', "li_clsfy");
	var textnode = document.createTextNode(element1 + ": " + elemet2);
	info.appendChild(textnode);
	document.getElementById('clsfy').appendChild(info);
}
function DeleteDiv(){
	var elem = document.getElementById('li_clsfy');
	elem.remove();
}

