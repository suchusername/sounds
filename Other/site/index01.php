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
		<DIV id = "">
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=0 CHECKED />truncate<BR>
		</DIV>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=1 />enhance<BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=2 />format<BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=3 />merge<BR>
		<BR>
		<INPUT TYPE=radio NAME='ArcType' ID='ArcType' VALUE=4 />frequencies
		</ul>
		<button type='submit' name='submit'>Upload</button>
</form>

</body>
</html>
