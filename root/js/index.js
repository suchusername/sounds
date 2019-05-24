var j = 1;

function generateForm() {
	if (j > 10) {
		return;
	}
	
	var d = document.createElement("div");
	d.setAttribute('id', "div"+j);
	var i = document.createElement("input"); //input element, text
	i.setAttribute('class',"inputfile");
	i.setAttribute('type',"file");
	i.setAttribute('name',"uploadfile[]");
	i.setAttribute('id',"uploadfile");
	i.setAttribute('multiple', "true");

	var r = document.createElement("button");
	r.setAttribute('onclick',"deleteEl(\'"+"div"+j+"\')");
	r.setAttribute('type',"button");
	r.innerHTML = "delete";


	//and some more input elements here
	//and dont forget to add a submit button

	//document.getElementsByTagName('form')[0].appendChild(i);
	//document.getElementsByTagName('form')[0].appendChild(r);
	d.appendChild(i);
	d.appendChild(r);
	//document.getElementsByTagName('div')[0].appendChild(d);
	document.getElementById('form_div').appendChild(d);
	j++;
}
generateForm();

function deleteEl(elementID) {
	var elem = document.getElementById(elementID);
	elem.remove();
	j--;
}

