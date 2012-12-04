<?php
  include("dataop.php");
?>

<?php
  $allowedExts = array("jpg", "jpeg", "gif", "png");
  $extension = end(explode(".", $_FILES["file"]["name"]));
  if ((($_FILES["file"]["type"] == "image/gif")  || ($_FILES["file"]["type"] == "image/jpeg") 
  || ($_FILES["file"]["type"] == "image/png") || ($_FILES["file"]["type"] == "image/pjpeg")) 
  && ($_FILES["file"]["size"] < 100000) && in_array($extension, $allowedExts)) {
        
    if ($_FILES["file"]["error"] > 0) {
      echo "Return Code: " . $_FILES["file"]["error"] . "<br>";
    }
    else {
      echo "Upload: " . $_FILES["file"]["name"] . "<br>";
      echo "Type: " . $_FILES["file"]["type"] . "<br>";
      echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
      echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br>";

      if (file_exists("upload/" . $_FILES["file"]["name"])) {
        echo $_FILES["file"]["name"] . " already exists. ";
      }
      else {
        $filename = $_FILES["file"]["name"];
        move_uploaded_file($_FILES["file"]["tmp_name"], "upload/" . $filename);
        echo "Stored in: " . "upload/" . $filename;

        //Call Hyperdex Put
        $image = file_get_contents("upload/" . $filename);
        $imagedata = base64_encode($image);

        $photoid   = uniqid();
        $username  = $_GET['username'];
        $currtime  = time() % 3600;

        echo "<br /><br />";
        echo "Photo Id: " . $photoid . "<br />";
        echo "Username: " . $username . "<br />";
        echo "Current Time: " . $currtime . "<br />";
        echo "Image Content: " . $imagedata . "<br />";

        $values=array("username"=>$username, "time"=>$currtime);
        echo prep_put("photoinfo", $photoid, $values, "null"); //should be tri_put

        $values=array("content"=>$imagedata);
        echo prep_put("photo_l", $photoid, $values, "null"); //should be tri_put
       
        /*
        $values=array("username"=>$username, "content"=>$imagedata, "time"=>$currtime);
        echo prep_put("photo_l", $photoid, $values, "handler"); //should be tri_put
        */
      }
    }
  }
  else {
    echo "Invalid file";
  }
?>