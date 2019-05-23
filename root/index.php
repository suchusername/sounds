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
	<script src="js/slider.js"></script>
	<link rel="stylesheet" type="text/css" href="css/style.css">
</head>

<body>
<table>
<tr>
<td width = "25%" valign="top">
<form id="" class = "uploadform" action="" enctype="multipart/form-data" method="POST">
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
	if (!is_dir($uploaddir)) mkdir($uploaddir);
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
		if (($type &#8733;!= "audio/mpeg") && ($type != "audio/wav")) {
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

	function remove_ext($file){
		return explode('.', $file)[0];
	}
	

	if(isset($_POST['btn_vol'])){
		increase_volume();	
	}

	function increase_volume(){
		
		$file = $_POST['file_radio'];
		$filename = strtok($file, '.');
		$extension = strtok('.');
		$k = ((double)$_POST['vol_mult'])/100;
		//echo $_POST['vol_mult'];

		sounds_volume(session_id().'/'.$file, session_id().'/'.$filename.'_louder.'.$extension, $k);
		
	}

	if(isset($_POST['btn_spd'])){
		increase_speed();
	}

	function increase_speed(){
		$file = $_POST['file_radio'];
		$filename = strtok($file, '.');
		$extension = strtok('.');
		$mult = (double)$_POST['text_spd'];
		sounds_speed(session_id().'/'.$file, session_id().'/'.$filename.'_sped_up.'.$extension, $mult);
	}

	if(isset($_POST['btn_cut'])){
		crop();
	}

	function crop(){
		$file = $_POST['file_radio'];
		$filename = remove_ext($file);
		$extension = 'wav';
		$l_border = (int)$_POST['text_cut_left'];
		$r_border = (int)$_POST['text_cut_right'];
		sounds_crop(session_id().'/'.$file, session_id().'/'.$filename.'_cropped.'.$extension, $l_border, $r_border);
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


	
	function delete_file($file){
		//$file = $_POST['file_radio'];
		//print($file);
		$uploaddir = '../Audios/Archive/'.session_id().'/';
		unlink($uploaddir.$file);//or die("Error while deleting");
	}
	?>
	<?php
	
	$current_dir = '../Audios/Archive/'.session_id().'/';

	//echo "<p>Каталог загрузки: $current_dir</p>";
	/*if(isset($_POST['btn_del_changed_bayan.wav'])){
		print("KEK");
		delete_file();
	}*/
	
	if (is_dir($current_dir)) {
		$dir = opendir($current_dir);
		echo "<table>";
		while ($file = readdir($dir)) {
			//echo 'btn_del_'.$file.'<br>';
			if($file != '.' && $file  != '..' ) {
				if(isset($_POST['btn_del_'.remove_ext($file)])){
					//print("KEK");
					delete_file($file);
				}			
			}
		}		
		closedir($dir);
	}
	
	
	
	echo 'Your files:<ul>';
	if (is_dir($current_dir)) {
		$dir = opendir($current_dir);
		echo "<table>";
		while ($file = readdir($dir)) {
			if($file != '.' && $file  != '..' ) {
				echo "<tr>";
				echo "<td width = '300px'> <input type='radio' id='$file' name='file_radio' value=$file> <a class = 'link2button' title='Click to download'  href='download.php?file=$file'> $file </a>  </td>";  //Radio
				
				echo "<td> <button title='Info' class = 'list_btn'> <b> &#63; </b> </button> </td>"; //Info
				
				//echo "<form action = 'download.php?file=$file'>";
				//echo "<td> <button type='submit' class = 'list_btn'> &#11015;   </button>   </td>"; //Download
				//echo "</form>";
				//echo "<td> <a class = 'link2button' href='download.php?file=$file'> &#11015; </a> </td>";
				
				
				echo "<td> <button class = 'list_btn' name = 'btn_del_".remove_ext($file)."' title='Delete file'>  &#10006; </button>  </td>"; //Delete
				
				echo "</tr>";
				//echo htmlspecialchars('download.php?file=$file');
				
				//echo "<td> <a href='/proj/sounds/Audios/Archive/".session_id()."/bayan.wav'>Download</a> </td>";
			}
		}
		closedir($dir);
	}

	echo "</table>";
	echo '</ul>';
	
	

	?>
	<p>
	<button id="btn_clsfy" name="btn_clsfy">Classify &#8733;</button>
	
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
		/*
		$probs = classify();
		if($probs[0]==-1) {
			echo "classify error!";
		} else {
			$instruments = array('accordion', 'piano', 'violin', 'guitar', 'flute');
			for($i=0; $i<5; $i++){
				echo "<script>GenerateDiv('$instruments[$i]', '$probs[$i]');</script>";
			}
		}
		*/

		$instruments = array('accordion', 'piano', 'violin', 'guitar', 'flute');
		for($i=0; $i<5; $i++){
			echo "<script>GenerateDiv('$instruments[$i]', 'vvv');</script>";
		}
		
	} 
	?>
	<p>
	<button id="btn_vol" name="btn_vol">Increase volume &#9836;</button>

	<input class="slider" type="range" id="vol_slider" value="100" min="0" max="500" step="5">
	New volume: <span id="vol_slider_out" > </span> %
	<input type="hidden" id="vol_hidden" name = "vol_mult" >
    
	<script>
		var vol_slider = document.getElementById("vol_slider");
		var vol_out = document.getElementById("vol_slider_out");
		var hidden_out = document.getElementById("vol_hidden");
		vol_out.innerHTML = vol_slider.value;
		hidden_out.value = vol_slider.value;

		vol_slider.oninput = function() {
			vol_out.innerHTML = this.value;
			hidden_out.value = vol_slider.value;
		}		 

	</script>	
	
	</p>
	
	<p>
	<button id="btn_spd" name="btn_spd">Increase speed &#8623;</button>

	<input type="text" class="text_inputs" name="text_spd" id="text_spd" pattern="\d+(\.\d{1,})?" title = "Increase by ... times" size ="3">
	

	</p>
	
	<p>
	<button id="btn_cut" name="btn_cut">Crop &#9988;</button>

	<input type="text" class="text_inputs" name="text_cut_left" id="text_cut_left" pattern = "^[ 0-9]+$" title = "Left border (in ms), 0 for minimum border" size ="3">
	<input type="text" class="text_inputs" name="text_cut_right" id="text_cut_right" pattern = "^[ 0-9]+$" title = "Right border (in ms), -1 for maximum border" size ="3">

	</p>	
	
	<p>
	<button id="btn_rename" name="btn_rename">Rename &#9997;</button>
	<input type="text" class="text_inputs" name="text_rename" id="text_rename" title = "Input your new filename" size ="10" value = "new_filename">.wav	
	</p>

</form>
</td>
</tr>
</table>
</body>
</html>

	

