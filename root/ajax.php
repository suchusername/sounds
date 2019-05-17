<?php
echo "<script> alert('HELLO');</script>";
$text = "Какой-то текст";
 
// открываем файл, если файл не существует,
//делается попытка создать его
$fp = fopen("file.txt", "w");
 
// записываем в файл текст
fwrite($fp, $text);
 
// закрываем
fclose($fp);
if(isset($_POST['btn_vol'])){
	
	increase_volume();	
}

function increase_volume(){
	echo "hello";
	$file = $_POST['file_radio'];	
	$k = (double)$_POST['text_vol'];
	sounds_volume($file, 'changed_'.$file, $k);
}

if(isset($_POST['btn_del'])){
	delete();
}

function delete(){
	$file = $_POST['file_radio'];
	unlink($file) or die("Error while deleting");
}
?>

<script src="http://code.jquery.com/jquery-1.9.1.js"></script>
<script>
	$(function () {
		$('form').on('submit', function (event) {
			event.preventDefault();// using this page stop being refreshing 

			$.ajax({
				type: "POST",
				url: "ajax.php",
				data: $('form').serialize(),
				success: function () {
					alert('form was submitted');
				}
			});

		});
	});
</script>
