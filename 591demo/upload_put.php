<?php
  include("dataop.php");

  $username = $_POST['username'];
  $photoid  = uniqid();
  $imageraw = $_POST['imageraw'];
  //$imageraw = 'iVBORw0KGgoAAAANSUhEUgAAABwAAAASCAMAAAB/2U7WAAAABl'
    //          . 'BMVEUAAAD///+l2Z/dAAAASUlEQVR4XqWQUQoAIAxC2/0vXZDr'
      //        . 'EX4IJTRkb7lobNUStXsB0jIXIAMSsQnWlsV+wULF4Avk9fLq2r'
        //      . '8a5HSE35Q3eO2XP1A1wQkZSgETvDtKdQAAAABJRU5ErkJggg==';

  $currtime  = time() % 3600;
  $imageraw  = base64_decode($imageraw);
  $image = imagecreatefromstring($imageraw);


  //put the original image in database
  $imagedata = base64_encode($imageraw);

  $values=array("username"=>$username, "time"=>$currtime);
  echo prep_put("photoinfo", $photoid, $values, "null");
  $values=array("content"=>$imagedata);
  echo prep_put("photo_l", $photoid, $values, "null");

/*   
  $values=array("username"=>$username, "content"=>$imagedata, "time"=>$currtime);
  echo prep_put("photoL", $photoid, $values, "null"); //should be put (not tri_put)
*/

  if ($image !== false) {
    header('Content-Type: image/png');
	//imagepng($image);
	//get new dimensions
	$width = imagesx($image);
	$height = imagesy($image);

	$percent = 0.5;
	$new_width = 100; //$width * $percent;
	$new_height = 100; //$height * $percent;

	//resample
	$newimg = imagecreatetruecolor($new_width, $new_height);
	imagecopyresampled($newimg, $image, 0, 0, 0, 0, $new_width, $new_height, $width, $height);

	//output
	//imagepng($newimg);

	//start buffering
	ob_start();
	imagepng($newimg);
	$contents = ob_get_contents();
	ob_end_clean();
	$imagedata = base64_encode($contents);
	echo "---";
	echo $imagedata;	//this contains the 100x100 image base64 string


	$values=array("username"=>$username, "time"=>$currtime);
  echo prep_put("photoinfo", $photoid, $values, "null");
	$values=array("content"=>$imagedata);
	echo prep_put("photo_s", $photoid, $values, "null");

  /*
	//put the resized image in database
	$values=array("username"=>$username, "content"=>$imagedata, "time"=>$currtime);
	echo prep_put("photo_s", $photoid, $values, "null"); //should be put (not tri_put)
  */	
  }
  else {
    echo 'An error occurred.';
  }

?>
