<!DOCTYPE html>
<html lang="en" >

<head>
  	<meta charset="UTF-8">
 	<title>Cool site</title>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
	<script src="script.js"></script>
		
	
</head>

<body>
<script  src="js/index.js"></script>
<form enctype="multipart/form-data" action="upload.php" method="post">
    	<p>Upload file(s)</p>
    	<div id="files">
		<p><input type="file" name="uploaded_file[]" multiple = "true"/></p>
    	</div>
    	<p><input type="button" value="Add File" onclick="addFile()" /></p>
	<button type="submit">Upload</button>
</form>	
</body>
</html>
