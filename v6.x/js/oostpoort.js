function hiddenInput(formname, name, value, action)
{
	var form = document.forms[formname];
	var newInput = document.createElement('input');
	newInput.setAttribute('type','hidden');
	newInput.setAttribute('id',name);
	newInput.setAttribute('name',name);
	newInput.setAttribute('value',value);
	form.appendChild(newInput);		
	
	var newInput2 = document.createElement('input');
	newInput2.setAttribute('type','hidden');
	newInput2.setAttribute('id','Action');
	newInput2.setAttribute('name','Action');
	newInput2.setAttribute('value',action);
	form.appendChild(newInput2);		
	
	form.submit();
}
