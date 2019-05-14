<?php

// Проверим, успешно ли загружен файл
//echo basename($_FILES['uploadfile']['tmp_name']);
if(!is_uploaded_file($_FILES['uploadfile']['tmp_name'])) {
  echo "Upload error (1)!";
  exit;
}

// Каталог, в который мы будем принимать файл:
$uploaddir = '../Audios/Archive/';
$uploadfile_name = $_FILES['uploadfile']['name'];
$uploadfile = $uploaddir.basename($_FILES['uploadfile']['name']); //разобраться с русскими буквами!

//проверим на допустимость расширения файла, mime-типа и размера
$blacklist = array(".php", ".phtml", ".php3", ".php4", ".html", ".htm");
foreach ($blacklist as $item)
  if(preg_match("/$item\$/i", $uploadfile)){
    echo "File type forbidden!";
    exit;
  }
$type = $_FILES['uploadfile']['type'];
$size = $_FILES['uploadfile']['size'];
//if (($type != "audio/mpeg") && ($type != "audio/wav")) exit;
//if ($size > 102400) exit; //размер не более 100кб

// Копируем файл из каталога для временного хранения файлов:
if (copy($_FILES['uploadfile']['tmp_name'], $uploadfile))	{
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
for($i = 0; $i < 6; $i++) {
    echo round($audio_info[$i], 2);
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
}*/
echo sounds_speed('alltta_bucket.wav', 'whew.wav', 1.5);
echo "<br>";
echo "<br>";

/*$arr_test = sounds_test(5);
echo $arr_test[0];
echo "<br>";
echo $arr_test[1];
echo "<br>";
echo $arr_test[2];
echo "<br>";*/

//Вставить код обработки файла
echo "ArcType: ".$_POST['ArcType']."<BR>";
switch($_POST['ArcType']){
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
*/
echo "<BR>";
echo "<A href=" . $uploadfile . ">Download file</A>";

?>
