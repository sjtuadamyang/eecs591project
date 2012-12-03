<?php
include("dataop.php");
put("photo",$_POST['photoid'],$_POST['description']."@".$_POST['photo'],"newfeeds");
?>
