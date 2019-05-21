function GenerateDiv(element){
	
	var info = document.createElement('li');
	info.setAttribute('id', "li_clsfy");
	var textnode = document.createTextNode(element);
	info.appendChild(textnode);
	document.getElementById('clsfy').appendChild(info);
}
function DeleteDiv(){
	var elem = document.getElementById('li_clsfy');
	elem.remove();
}

