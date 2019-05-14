<?php

// Проверим, успешно ли загружен файл
if(  !is_uploaded_file($_FILES['uploadfile']['tmp_name'])  ){
  echo "Upload error (1)!";
  exit;
}

// Каталог, в который мы будем принимать файл:
$uploaddir = './sound/';
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
	echo "<h3>Upload success!!!</h3>";
} else {
	$errors = error_get_last();
	echo "<h3>Upload error (2)!</h3> Error type: ".$errors['type']. ". Message: " .$errors['message'];
	//exit;
}


//Вставить код обработки файла

// Выводим информацию о загруженном файле:
echo "<h3>File information: </h3>";
echo "<p><b>Original file name: ".$_FILES['uploadfile']['name']."</b></p>";
echo "<p><b>Mime-type: ".$_FILES['uploadfile']['type']."</b></p>";
echo "<p><b>File size: ".$_FILES['uploadfile']['size']."</b></p>";
echo "<p><b>Temporary name: ".$_FILES['uploadfile']['tmp_name']."</b></p>";
echo "<p><b>Error code: ".$_FILES['uploadfile']['error']."</b></p>";

echo "<BR>";
echo "<A href=" . $uploadfile . ">Download file</A>";

?>
