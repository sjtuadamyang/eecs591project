<?php
  include("dataop.php");

  $username = $_POST['username'];
  $photoid  = uniqid();
  $imageraw = $_POST['imageraw'];

  $currtime  = time() % 3600;
  $imagedata = $imageraw;

  $values=array("username"=>$username, "time"=>$currtime);
  echo prep_put("photoinfo", $photoid, $values, "null"); //should be tri_put

  $values=array("content"=>$imagedata);
  echo prep_put("photo_l", $photoid, $values, "null"); //should be tri_put  

/*
  $values=array("username"=>$username, "content"=>$imagedata, "time"=>$currtime);
  echo prep_put("photoL", $photoid, $values, "handler");
*/  
?>
