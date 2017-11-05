<?php

  $Sender = $_POST['Sender'];
	$ID = $_POST['ID'];
	$stream = $_POST['stream'];
  if (isset($_POST['curpos'])) {
    $cur_print_pos = $_POST['curpos'];  
  } else {
    $cur_print_pos = 0;
  }
  
  $py_output = array();

  if ($Sender == "Sort Toggle") {
    $py_output = array();
    $command = "./view.py \"".$ID."\" \"".$stream."\" \"O\" ".$cur_print_pos;
    exec($command, $py_output);

    $file = fopen("currentPost.txt", "w");
    foreach($py_output as $py_line) {
      if(strpos($py_line, 'CUR_POS:') !== false) {
        $temp = explode (":", $py_line);
        $cur_print_pos = $temp[1];
      } else {
        fwrite($file, $py_line."\n");    
      }
    }

    fclose($file);
  } elseif ($Sender == "Mark All") {
    $command = "./view.py \"".$ID."\" \"all\" \"M\" ".$cur_print_pos;
    exec($command, $py_output);
  } elseif ($Sender == "Check For New") {
    $py_output = array();
    $command = "./view.py \"".$ID."\" \"".$stream."\" \"C\" ".$cur_print_pos;
    exec($command, $py_output);

    $file = fopen("currentPost.txt", "w");
    foreach($py_output as $py_line) {
      if(strpos($py_line, 'CUR_POS:') !== false) {
        $temp = explode (":", $py_line);
        $cur_print_pos = $temp[1];
      } else {
        fwrite($file, $py_line."\n");    
      }
    }

    fclose($file);
  } elseif ($Sender == "Forward") {
    $command = "./view.py \"".$ID."\" \"".$stream."\" \"Forward\" ".$cur_print_pos;
    exec($command, $py_output);
    $file = fopen("currentPost.txt", "w+");
    foreach($py_output as $py_line) {
      if(strpos($py_line, "CUR_POS:") !== false) {
        $temp = explode (":", $py_line);
        $cur_print_pos = $temp[1];
      } else {
        fwrite($file, $py_line."\n");    
      }  
    }

    fclose($file);
  } elseif ($Sender == "Back") {
    $command = "./view.py \"".$ID."\" \"".$stream."\" \"Backwards\" ".$cur_print_pos;
    exec($command, $py_output);
    $file = fopen("currentPost.txt", "w+");
    foreach($py_output as $py_line) {
      if(strpos($py_line, 'CUR_POS:') !== false) {
        $temp = explode (":", $py_line);
        $cur_print_pos = $temp[1];
      } else {
        fwrite($file, $py_line."\n");    
      }
    }

    fclose($file);
  } else {
    $command = "./view.py \"".$ID."\" \"".$stream."\" \"null\" ".$cur_print_pos;
    exec($command, $py_output);
    $file = fopen("currentPost.txt", "w");
    foreach($py_output as $py_line) {
      if(strpos($py_line, 'CUR_POS:') !== false) {
        $temp = explode (":", $py_line);
        $cur_print_pos = $temp[1];
      } else {
        fwrite($file, $py_line."\n");    
      }
    }

    fclose($file);
  }

  $output = array();
  exec("./configfile view.wpml", $output);

 	foreach($output as $line) {
 		if(strpos($line, 'post.php') !== false) {
 			echo $line;
 			echo "<input type=\"hidden\" name=\"ID\" value=\"".$ID."\"/>";
 			echo "<input type=\"hidden\" name=\"stream\" value=\"".$stream."\"/>";
 		} elseif(strpos($line, 'addauthor.php') !== false) {
      echo $line;
      echo "<input type=\"hidden\" name=\"ID\" value=\"".$ID."\"/>";
      echo "<input type=\"hidden\" name=\"stream\" value=\"".$stream."\"/>";
    } elseif(strpos($line, 'view.php') !== false) {
      echo $line;
      echo "<input type=\"hidden\" name=\"ID\" value=\"".$ID."\"/>";
      echo "<input type=\"hidden\" name=\"stream\" value=\"".$stream."\"/>";
      echo "<input type=\"hidden\" name=\"curpos\" value=\"".$cur_print_pos."\"/>";
    } else{
      echo $line;
    }		
 	}


?>