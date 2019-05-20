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
	/*$audio_probs = sounds_classify($uploadfile_name);
	$audio_tags = array('accordion', 'piano', 'violin', 'guitar', 'noise');
	echo "probabilities: <br>";
	for ($i = 0; $i < 5; $i++) {
		echo $audio_tags[$i];
		echo " ";
		echo $audio_probs[$i];
		echo "<br>";
	}
	echo sounds_speed('alltta_bucket.wav', 'whew.wav', 1.5);
	echo "<br>";
	echo "<br>";
	$arr_test = sounds_test(5);
	echo $arr_test[0];
	echo "<br>";
	echo $arr_test[1];
	echo "<br>";
	echo $arr_test[2];
	echo "<br>";*/
}



/*
echo "<BR>";
//Вставить код обработки файла
echo "ArcType: ".$_POST['ArcType']."<BR>";
switch($_POST['ArcType']) {
	case 0:
		echo "0";
		break;
	case 1:
		echo "1";
		break;
	case 2:
		echo "2";
		break;
	case 3:
		echo "3";
		break;
	case 4:
		echo "4";
		break;
}
/*exec('ls -l', $output);
foreach ($output as $item) echo $item."<BR>";

echo "<BR>";
echo "<A href=" . $uploadfile . ">Download file</A>";
*/

/*
if($_GET['btn_f1']){send_file('function_1');}

if($_GET['btn_f2']){send_file('function_2');}



function send_file($option){
	$out = [];	
	$radios = document.getElementsByName('file_radio');
	for($i = 0; $i < $radios.length; $i++){
		if($radios[$i].checked){
			$fname = $radios[i].id;
			$out = [$fname, $option];
			
			echo '<script language="javascript">';
			echo 'alert("('.$out[0].', '.$out[1].')")';
			echo '</script>';	
		}
	}
}

function weird_flex(){
	echo $_POST['text_vol'];
	}
*/


?>
<body>
<?php


function classify(){
	$path = 'Sounds/'.$_POST['file_radio'];
	$probs = sounds_classify($path);
	$instruments = array('accordion', 'piano', 'violin', 'guitar', 'smth else');
	
	
	return $probs;
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
	
	//echo "<A href=" . $uploadfile . ">Download file</A>";
}

if(isset($_POST['btn_spd'])){
	increase_speed();
}

function increase_speed(){
	$file = $_POST['file_radio'];
	$filename = strtok($file, '.');
	$extension = strtok('.');
	$mult = (double)$_POST['text_spd'];

	sounds_speed('Sounds/'.$file, 'Sounds/'.$filename.'(1).'.$extension, $mult);
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
	sounds_crop('Sounds/'.$file, 'Sounds/'.$filename.'_cutted.'.$extension, $l_border, $r_border);
}

if(isset($_POST['btn_rename'])){
	rename_file();
}

function rename_file(){
	$file = $_POST['file_radio'];
	//$filename = substr(strrchr($file, "/"), 1);
	$newname = '../Audios/Archive/Sounds/'.$_POST['text_rename'].'.wav';
	rename('../Audios/Archive/Sounds/'.$file, $newname);	
}

if(isset($_POST['btn_del'])){
	delete_file();
}

function delete_file(){
	$file = $_POST['file_radio'];
	unlink('../Audios/Archive/Sounds/'.$file) or die("Error while deleting");
}
?>


<form action="" enctype="multipart/form-data" method="POST">

	<?php
	
	$current_dir = '../Audios/Archive/Sounds/';
	
	$dir = opendir($current_dir);

	//echo "<p>Каталог загрузки: $current_dir</p>";
	echo '<p>Your files:</p><ul>';
	while ($file = readdir($dir)) {
		if($file != '.' && $file  != '..' ) {
			echo "<ul> <input type='radio' id='$file' name='file_radio' value=$file> $file </ul>";
		}
	}
	echo '</ul>';
	closedir($dir);
	
	/*
	for($i=0; $i<count($upload_files); $i++){
		echo "<li>$upload_files[$i] <input type='radio' id='$upload_files[$i]' name='file_radio' value='$upload_files[$i]'></li>";
		//echo "<li>$upload_files[0] <input type='radio' id='$upload_files[0]' name='file_radio' value='$upload_files[0]'></li>";
		//echo count($upload_files);
	}
	*/
	?>
	<p>
	<button id="btn_clsfy" name="btn_clsfy">Classify</button>
	
	<div id="clsfy"></div>
	</p>
	<script src='js/upload.js'></script>
	<?php
	if(isset($_POST['btn_clsfy'])){
		//$probs[] = classify();
		$instruments = array('accordion', 'piano', 'violin', 'guitar', 'smth else');
		for($i=0; $i<5; $i++){
			echo "<script>GenerateDiv('$instruments[$i]');</script>";
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
</body>

	

