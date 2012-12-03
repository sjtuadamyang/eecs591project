<?php
include("dataop.php");
$values=array("attr1"=>"value1","attr2"=>"value2","attr3"=>"value3");
echo prep_put("blog","dozenow_blog",$values,"handler1");
?>
