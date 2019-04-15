<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=Windows-1251">
<link href="index.css" rel="stylesheet" type="text/css" />
<title>File upload test</title>  
</head>
<body>

<?php
	header("Content-Type: text/html; charset=Windows-1251");
?>



<form action=upload.php enctype='multipart/form-data' method='POST'>
	File upload: <input name='uploadfile' type='file' />
		<ul>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=0 CHECKED /><label for='ArcType'>truncate</label><BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=1 CHECKED /><label for='ArcType'>enhance</label><BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=2 CHECKED /><label for='ArcType'>format</label><BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=3 CHECKED /><label for='ArcType'>merge</label><BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=4 CHECKED /><label for='ArcType'>frequencies</label>
		</ul>
		<button type='submit' name='submit'>Upload</button>
</form>

</body>
</html>
