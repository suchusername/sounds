<?php
	session_start();
?>
<!DOCTYPE html>
<html lang="en" >

<head>
  	<meta charset="UTF-8">
 	<title>Cool site</title>
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
	<script src="script.js"></script>
</head>

<body>
<table>
<tr>
<td width = "25%" valign="top">
<form id="" action="" enctype="multipart/form-data" method="POST">
		<div id = "div"> </div>
		<button type="button" onclick="generateForm()">Add more</button>
		<button type="submit">Upload</button>
	</form>		
	<script  src="js/index.js"></script>

	
</form>

<?php
// Проверим, успешно ли загружен файл
//echo basename($_FILES['uploadfile']['tmp_name']);
if (!empty($_FILES)){
	
	$uploaddir = '../Audios/Archive/'.session_id().'/';
	mkdir($uploaddir);//Надо ли сначала проверить наличие папки?
	for($i=0;$i<count($_FILES['uploadfile']['name']);$i++) {
		if(!is_uploaded_file($_FILES['uploadfile']['tmp_name'][$i])) {
		  echo "Upload error (1)!";
		  break;
		  //exit;
		}

		// Каталог, в который мы будем принимать файл:
		//$uploaddir = '../Audios/Archive/Sounds/';
		$uploadfile_name = './'.session_id().'/'.$_FILES['uploadfile']['name'][$i];
		$uploadfile = $uploaddir.basename($_FILES['uploadfile']['name'][$i]); //разобраться с русскими буквами!
		
		//проверим на допустимость расширения файла, mime-типа и размера
		$blacklist = array(".php", ".phtml", ".php3", ".php4", ".html", ".htm");
		foreach ($blacklist as $item)
		  if(preg_match("/$item\$/i", $uploadfile)) {
			echo "File type forbidden!";
			exit;
		  }
		$type = $_FILES['uploadfile']['type'];
		$size = $_FILES['uploadfile']['size'];
		/*
		if (($type != "audio/mpeg") && ($type != "audio/wav")) {
			echo "Unsupported file";
			exit;
		}
		*/
		//if ($size > 102400) exit; //размер не более 100кб

		// Копируем файл из каталога для временного хранения файлов:
		if (copy($_FILES['uploadfile']['tmp_name'][$i], $uploadfile)) {
			//echo "<h3>Upload success!!!</h3>";
			$upload_files[$i] = $uploadfile_name;

		} else {
			$errors = error_get_last();
			echo "<h3>Upload error (2)!</h3> Error type: ".$errors['type']. ". Message: " .$errors['message'];
			exit;
		}
		
	}
}

?>


</td>
<td valign="top">
<form action="" enctype="multipart/form-data" method="POST">
	<?php

	if(isset($_POST['btn_vol'])){
		increase_volume();	
	}

	function increase_volume(){
		$file = $_POST['file_radio'];
		$file_dir = session_id().'/'.$file;
		$uploadfile = './'.session_id().'/changed_'.$file;
		$k = (double)$_POST['text_vol'];
		sounds_volume($file_dir, $uploadfile, $k);
	}

	if(isset($_POST['btn_spd'])){
		increase_speed();
	}

	function increase_speed(){
		$file = $_POST['file_radio'];
		$filename = strtok($file, '.');
		$extension = strtok('.');
		$mult = (double)$_POST['text_spd'];

		sounds_speed(session_id().'/'.$file, session_id().'/'.$filename.'(1).'.$extension, $mult);
	}

	if(isset($_POST['btn_cut'])){
		crop();
	}

	function crop(){
		$file = $_POST['file_radio'];
		$filename = strtok($file, '.');
		$extension = strtok('.');
		$l_border = (int)$_POST['text_cut_left'];
		$r_border = (int)$_POST['text_cut_right'];
		sounds_crop(session_id().'/'.$file, session_id().'/'.$filename.'_cutted.'.$extension, $l_border, $r_border);
	}

	if(isset($_POST['btn_rename'])){
		rename_file();
	}

	function rename_file(){
		$file = $_POST['file_radio'];
		$uploaddir = '../Audios/Archive/'.session_id().'/';
		//$filename = substr(strrchr($file, "/"), 1);
		$newname = $uploaddir .$_POST['text_rename'].'.wav';
		rename($uploaddir .$file, $newname);	
	}

	if(isset($_POST['btn_del'])){
		delete_file();
	}

	function delete_file(){
		$file = $_POST['file_radio'];
		$uploaddir = '../Audios/Archive/'.session_id().'/';
		unlink($uploaddir.$file);//or die("Error while deleting");
	}
	?>
	<?php
	
	$current_dir = '../Audios/Archive/'.session_id().'/';
	
	$dir = opendir($current_dir);

	//echo "<p>Каталог загрузки: $current_dir</p>";
	echo 'Your files:<ul>';
	while ($file = readdir($dir)) {
		if($file != '.' && $file  != '..' ) {
			echo "<ul> <input type='radio' id='$file' name='file_radio' value=$file> $file </ul>";
			echo "<a href='download.php?file=$file'>Download</a>";
		}
	}
	echo '</ul>';
	closedir($dir);
	
	?>
	<p>
	<button id="btn_clsfy" name="btn_clsfy">Classify</button>
	
	<div id="clsfy"></div>
	</p>
	<script src='js/upload.js'></script>
	<?php
	function classify(){
		$path = session_id().'/'.$_POST['file_radio'];
		$probs = sounds_classify($path);
		return $probs;
	}
	if(isset($_POST['btn_clsfy'])){
		$probs = classify();
		if($probs[0]==-1) {
			echo "classify error!";
		}
		$instruments = array('accordion', 'piano', 'violin', 'guitar', 'flute');
		for($i=0; $i<5; $i++){
			echo "<script>GenerateDiv('$instruments[$i]', 'vvv');</script>";
		}
		
	} 
	?>
	<p>
	<button id="btn_vol" name="btn_vol">Increase volume</button>

	<input type="text" name="text_vol" id="text_vol" pattern="\d+(\.\d{1,})?" title = "Increase by ... times" size ="3">

	</p>
	
	<p>
	<button id="btn_spd" name="btn_spd">Increase speed</button>

	<input type="text" name="text_spd" id="text_spd" pattern="\d+(\.\d{1,})?" title = "Increase by ... times" size ="3">

	</p>
	
	<p>
	<button id="btn_cut" name="btn_cut">Cut</button>

	<input type="text" name="text_cut_left" id="text_cut_left" pattern = "^[ 0-9]+$" title = "Left border (in ms), 0 for minimum border" size ="3">
	<input type="text" name="text_cut_right" id="text_cut_right" pattern = "^[ 0-9]+$" title = "Right border (in ms), -1 for maximum border" size ="3">

	</p>
	
	<p>
	<button id="btn_del" name="btn_del">Delete</button>
	</p>
	
	<p>
	<button id="btn_rename" name="btn_rename">Rename</button>
	<input type="text" name="text_rename" id="text_rename" title = "Input your new filename" size ="10" value = "new_filename">.wav	
	</p>

</form>
</td>
</tr>
</table>
</body>
</html>

	

