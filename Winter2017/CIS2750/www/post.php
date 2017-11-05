<?php 
	$Sender = $_POST['Sender'];
	$ID = $_POST['ID'];
	$stream = $_POST['stream'];
	$text = $_POST['post'];

	if($Sender == "Submit") {
		if($stream == "all") {
			echo "<SCRIPT> alert('You cant write to more then one stream (current: all)') </SCRIPT>";
		} else {
			$res = array();
			$command =  "./post \"".$ID."\" \"".$stream."\" \"".$text."\n\"";
			exec($command, $res);
			if($res[0] == "CANT_WRITE") {
				echo "author ".$ID." stream ".$stream;
				echo "<SCRIPT> alert('You cant write to this stream') </SCRIPT>";
			}
		}
	}

	$output = array();
    exec("./configfile post.wpml", $output);

   	foreach($output as $line) {
   		if(strpos($line, 'post.php') !== false) {
   			echo $line;
   			echo "<input type=\"hidden\" name=\"ID\" value=\"".$ID."\"/>";
   			echo "<input type=\"hidden\" name=\"stream\" value=\"".$stream."\"/>";
   		} else if(strpos($line, 'view.php') !== false) {
   			echo $line;
   			echo "<input type=\"hidden\" name=\"ID\" value=\"".$ID."\"/>";
   			echo "<input type=\"hidden\" name=\"stream\" value=\"".$stream."\"/>";
   		} else {
   			echo $line;
   		}
   		
   	}

?>