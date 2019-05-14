<!DOCTYPE html>
<html lang="en" >
<head>
  	<meta charset="UTF-8">
 	<title>Cool site</title>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
	<script src="script.js"></script>
		
	
</head>

<body>
<table width = 100%>
	<tr  class = "row_1"> 
		<td class = "el_1_1" width = "30%">
			Files
		</td>
		<td class = "el_1_2">
			What to do
		</td>
	</tr>
	<tr class = "row_2" height = "300px">
		<td  class = "el_2_1">
	<form id="form1" action="upload.php" enctype="multipart/form-data" method="POST">
		<div id = "div"> </div>

		<button type="submit">Upload</button>
	</form>		
	<script  src="js/index.js"></script>

	<button onclick="generateForm()">Add more files</button>
		</td>
		<td class = "el_2_2">
		</td>
	</tr>


</table>

</body>
</html>

