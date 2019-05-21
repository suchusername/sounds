<?php

// Проверим, успешно ли загружен файл
//echo basename($_FILES['uploadfile']['tmp_name']);
for($i=0;$i<count($_FILES['uploadfile']['name']);$i++) {
	if(!is_uploaded_file($_FILES['uploadfile']['tmp_name'][$i])) {
	  echo "Upload error (1)!";
	  exit;
	}

	// Каталог, в который мы будем принимать файл:
	$uploaddir = './sound/';
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


if($_GET['btn_f1']){send_file('function_1');}

if($_GET['btn_f2']){send_file('function_2');}


/*
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
*/

echo $_POST['file_radio'];

?>

<form action="" enctype="multipart/form-data" method="POST">
	<?php
	$current_dir = './sound/';
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
	<button id="btnfun1" name="btn_vol" onClick='location.href="?btn_f1=1"'>Increase volume</button>

	<input type="text" name="text_vol" id="text_vol" pattern="[0-9].[0-9].{3,}" title = "Sample text">

	</p>
	<p>
	<button id="btnfun2" name="btnfun2" onClick='location.href="?btn_f2=1"'>functon_2</button>
	</p>

</form>	

