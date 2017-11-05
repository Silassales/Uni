<?php 
	$Sender = $_POST['Sender'];
	$ID = $_POST['ID'];
	$stream = $_POST['stream'];
	$addAuthor = $_POST['addauthor'];
	$addStream = $_POST['addstream'];
	$addremove = $_POST['addremove'];

	if($Sender == "Submit") {
		if($addremove !== "add" && $addremove !== "remove") {
			echo "<SCRIPT> alert('Either enter add or remove in the add/remove text box') </SCRIPT>";
		} else {
			$command =  "./addauthor \"".$addAuthor."\" \"".$addStream."\" \"".$addremove."\"";
	 		exec($command);
		}
	}

	$output = array();
    exec("./configfile addauthor.wpml", $output);

   	foreach($output as $line) {
   		if(strpos($line, 'view.php') !== false) {
   			echo $line;
   			echo "<input type=\"hidden\" name=\"ID\" value=\"".$ID."\"/>";
   			echo "<input type=\"hidden\" name=\"stream\" value=\"".$stream."\"/>";
   		} else {
   			echo $line;
   		}
   	}

?>