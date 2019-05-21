<!DOCTYPE html>
<html lang="en" >

<head>
  <meta charset="UTF-8">
  <title>Cool site</title>

	<link rel="stylesheet" href="css/style.css">
	
</head>

<body>
<table width = 100%>
	<tr  class = "row_1"> <!-- названия -->
		<td class = "el_1_1" width = "30%">
			Files
		</td>
		<td class = "el_1_2">
			What to do
		</td>
	</tr>
	<tr class = "row_2" height = "300px">
		<td  class = "el_2_1">

   
	<form action="upload.php" enctype="multipart/form-data" method="POST">
 				
		<p>  Drag your files here or click in this area. </p>
		<input name='uploadfile' type="file" namultiple>
		<button type="submit">Upload</button>
	</form>		</td>
		<td class = "el_2_2">
		</td>
	</tr>

</table>
<script  src="js/index.js"></script>
</body>
</html>

