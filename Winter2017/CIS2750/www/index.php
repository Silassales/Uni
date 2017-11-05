<?php
    $output = array();
    exec("./configfile index.wpml", $output);

   	foreach($output as $line) {
   		echo $line;
   	}
?>