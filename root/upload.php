<?php

// Проверим, успешно ли загружен файл
//echo basename($_FILES['uploadfile']['tmp_name']);
for($i=0;$i<count($_FILES['uploadfile']['name']);$i++) {
	
	if(!is_uploaded_file($_FILES['uploadfile']['tmp_name'][$i])) {
	  echo "Upload error (1)!";
	  exit;
	}

	// Каталог, в который мы будем принимать файл:
	$uploaddir = '../Audios/Archive/Sounds/';
	$uploadfile_name = './Sounds/'.$_FILES['uploadfile']['name'][$i];
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
	//if (($type != "audio/mpeg") && ($type != "audio/wav")) exit;
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
	echo "<BR>";
	echo $uploadfile;
	echo "<br>";
	echo $uploadfile_name;
	echo "<br>";
	$audio_info = sounds_info($uploadfile_name);
	for($j = 0; $j < 6; $j++) {
		echo round($audio_info[$j], 2);
		echo "<br>";
	}
	echo "<br>";
}

if(isset($_POST['btn_vol'])){
	increase_volume();	
}

function increase_volume(){
	$file = $_POST['file_radio'];
	$file_dir = 'Sounds/'.$file;
	$uploadfile = './Sounds/changed_'.$file;
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
	sounds_speed($file, $filename.'(1).'.$extension);
}

if(isset($_POST['btn_del'])){
	delete_file();
}

function delete_file(){
	$file = $_POST['file_radio'];
	unlink($file) or die("Error while deleting");
}

	$current_dir = '../Audios/Archive/Sounds/';
	
	$dir = opendir($current_dir);

	echo "<p>Каталог загрузки: $current_dir</p>";
	echo '<p>Содержимое каталога:</p><ul>';
	while ($file = readdir($dir)) {
		if($file != '.' && $file  != '..' ) {
			echo "<lu> <input type='radio' id='$file' name='file_radio' value=$file> $file </lu>";
		}
	}
	echo '</ul>';
	closedir($dir);
?>
<form action="" enctype="multipart/form-data" method="POST">

	<p>
	<button id="btn_vol" name="btn_vol">Increase volume</button>

	<input type="text" name="text_vol" id="text_vol" pattern="\d+(\.\d{1,})?" title = "Sample text" size ="3">

	</p>
	
	<p>
	<button id="btn_spd" name="btn_spd">Increase speed</button>

	<input type="text" name="text_spd" id="text_spd" pattern="\d+(\.\d{1,})?" title = "Sample text" size ="3">

	</p>
	
	<p>
	<button id="btn_del" name="btn_del">Delete</button>
	</p>

</form>	


	

