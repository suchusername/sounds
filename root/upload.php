<?php

// Проверим, успешно ли загружен файл
//echo basename($_FILES['uploadfile']['tmp_name']);
for($i=0;$i<count($_FILES['uploadfile']['name']);$i++) {
	if(!is_uploaded_file($_FILES['uploadfile']['tmp_name'][$i])) {
	  echo "Upload error (1)!";
	  exit;
	}

	// Каталог, в который мы будем принимать файл:
	$uploaddir = '../Audios/Archive/';
	$uploadfile_name = $_FILES['uploadfile']['name'][$i];
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

function delete(){
	$file = $_POST['file_radio'];
	unlink($file) or die("Error while deleting");
}
function increase_volume(){
	$file = $_POST['file_radio'];
	$k = (float)$_POST['text_vol'];
	//вставить функцию
}

?>

<form action="upload.php" enctype="multipart/form-data" method="POST">
	<?php
	$current_dir = '../Audios/Archive/';
	$dir = opendir($current_dir);

	echo "<p>Каталог загрузки: $current_dir</p>";
	echo '<p>Содержимое каталога:</p><ul>';
	while ($file = readdir($dir)) {
		if($file != '.' && $file  != '..') {
			echo "<li>$file <input type='radio' id='$file' name='file_radio' value=$file></li>";
		}
	}
	echo '</ul>';
	closedir($dir);
	?>
	<p>
	<button id="btn_vol" name="btn_vol" onClick="foo('volume')">Increase volume</button>

	<input type="text" name="text_vol" id="text_vol" pattern="\d+(\.\d{1,})?" title = "Sample text" size ="3">

	</p>
	<p>
	<button id="btn_del" name="btn_del" onClick="foo('delete')">Delete</button>
	</p>

	<script>
		function foo(option){
			if (option == 'delete'){
				alert("<?php delete(); ?>");}
			else if (option == 'volume'){
				alert("<?php increase_volume(); ?>");}	
			}
	</script>

</form>	


	

