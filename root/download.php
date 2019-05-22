<?php
session_start();
$path = '../Audios/Archive/'.session_id().'/';
ini_set('include_path', $path);
//set_include_path(get_include_path() . PATH_SEPARATOR . $path);
header('Content-Type: text/html; charset=utf-8');
if (isset($_GET['file']) and preg_match("/.wav$/", $_GET['file']) and file_exists($path.$_GET['file'])) {
    $content = file_get_contents($_GET['file'], TRUE);
    header('Content-Type: '.$ctype.'; charset=utf-8');
    header("Content-Disposition: attachment; filename=".$_GET['file']);
    ob_end_clean();
    ob_start();
    echo $content;
    ob_end_flush();
    exit();
} 
else {	
	echo $_GET['file'];
	echo "<br>file not found";
	echo "<script> function back() {history.back();}</script>";
	echo "<button onclick='back();'>back</button>";
	exit();
}
?>
