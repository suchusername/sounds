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
		<td class = "el_1_1" width = "20%">
			Files
		</td>
		<td class = "el_1_2">
			Something else
		</td>
		<td  class = "el_1_3">
			Something else(2)
		</td>
	</tr>
	<tr class = "row_2" height = "300px">
		<td  class = "el_2_1">
			<form action="upload.php" enctype="multipart/form-data" method="POST">
 				
  				<p>  Drag your files here or click in this area. </p>
  				<input name="uploadfile" type="file" multiple>
  				<button type="submit">Upload</button>
			</form>
			<script src='http://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js'></script>
			<script  src="js/index.js"></script>
		</td>
		<td class = "el_2_2">
		</td>
		<td class = "el_2_3">
		</td>
	</tr>

</table>
</body>
</html>
