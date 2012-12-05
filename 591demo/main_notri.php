<!DOCTYPE html>

<?php
  include("dataop.php");
  $username = $_GET['username'];
  if (!empty($_POST)) {
	$blogpost = $_POST['blogpost'];
  }
?>

<html lang="en">

<head>
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
<link href="bootstrap/css/bootstrap.min.css" rel="stylesheet" media="screen">
<link href="css/main.css" rel="stylesheet" media="screen">
<title> Main Page </title>
</head>

<body>
	<!-- Page Navigation -->
 	<div class="navbar navbar-inverse navbar-fixed-top">
      <div class="navbar-inner">
        <div class="container-fluid">
          <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </a>
          <a class="brand" href="#">EECS 591 Demo</a>
          <div class="nav-collapse collapse">
            <p class="navbar-text pull-right">
			<?php
				echo "<a href='#' class='navbar-link'>" . "Logged in as " . $username . "</a>";
			?>            	
            </p>
            <ul class="nav">
              <li class="active"><a href="#">Home</a></li>
              <li><a href="#about">Albums</a></li>
              <li><a href="#about">Friends</a></li>
              <li><a href="#about">Share</a></li>
              <li><a href="#about">About</a></li>
              <li><a href="#contact">Contact</a></li>
            </ul>
          </div><!--/.nav-collapse -->
        </div>
      </div>
    </div>

	<div class="container-fluid">
      <?php
	    echo "<p id='welcome'>Welcome back, " . $username . "</p>";
	  ?>
	</div>
    
    <!-- Page Header -->
	<div class="container-fluid">
	  <div class="row-fluid">    
	    <div class="span12">
		  <div class="well">
		    <img id="profile" src="img/wuyanzu.png" width=60 height=50 >
			"Personal Status Update" and Advertisement <br />
			<form class="form-inline" name="blogform" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
			<lable> I am thinking ... </lable> <input class="input-xlarge" type="text" name="blogpost" />&nbsp
			<button type="submit" class="btn btn-info">Post Status</button>
			</form>
	      </div>
	    </div>
	  </div>
	</div>

	<!-- Page Columns -->
	<div class="container-fluid">
	  <div class="row-fluid">
	    <div class="span3 leftsidebar">
	      <!--Sidebar content-->
			Upload a Photo <br /><br />
			<form class="form" name="uploadform" action="<?php echo 'upload.php?username=' . $username; ?>" method="post" enctype="multipart/form-data">
			<label for="file">Filename:</label>
			<input type="file" name="file" id="file"><br>
			<input type="submit" class="btn btn-success" name="submit" value="Upload">
			</form>

	    </div>
	    <div class="span6 mainbody well">
	      <!--Body content-->
	      <?php
	      	//tri_put version
	      	/*
	      	$photolist = get(prep_get("user", $username, "getpid"));
	      	//echo "-----" . $photolist . "-----" . "<br /";
            $photo = explode("@", $photolist);

            $waste = 0;
            for ($i=1; $i<10000; $i++) {
            	$waste = $waste + 1;
  			}

	      	foreach ($photo as $p) {
	      		//echo "---" . $p . "<br />";
                $imagedata = direct_get("photo_l", $p, "null");
	      		echo "<img src='data:image/png;base64, " . $imagedata . "'>";
	      	}
			*/
	      	//noraml put: call search and foreach get
	      	$searchval = "somename";
	      	$photolist = search("photoinfo", "username", $searchval);
            $photo = explode("@", $photolist);
            $waste = 0;
            for ($i=1; $i<10000; $i++) {
            	$waste = $waste + 1;
  			}
  			$id = rand(0, count($photo)) % count($photo);
  			$imagedata = direct_get("photo_l", $photo[$id], "null");
	      	echo "<img src='data:image/png;base64, " . $imagedata . "'>";

	      	/*
	      	foreach ($photo as $p) {
                $imagedata = direct_get("photo_l", $p, "null");
	      		echo "<img src='data:image/png;base64, " . $imagedata . "'>";
	      	}
	      	*/
	      ?>

		  <img src="img/1.jpg">
		  <p>@User1 uploaded a photo</p>
		  <img src="img/2.jpg">
		  <p>@User2 uploaded a photo</p>
		  <img src="img/3.jpg">
		  <p>@User3 uploaded a photo</p>
		  <img src="img/4.jpg">
		  <p>@User4 uploaded a photo</p>
		  <img src="img/5.jpg">
		  <p>@User5 uploaded a photo</p>

	    </div>
	    <div class="span3 rightsidebar">
	      <!--Sidebar content-->
	      My Photos <br />
	      <?php
	      	foreach ($photo as $p) {
                $imagedata = direct_get("photo_s", $p, "null");
	      		echo "<img src='data:image/png;base64, " . $imagedata . "', width=90>";
	      	}
	      ?>
	      <img src="img/wuyanzu.png" width=90>
	      <img src="img/wuyanzu.png" width=90>
	      <img src="img/wuyanzu.png" width=90>

	    </div>
	  </div>

      <!-- Page Footer -->
	  <hr />
	  <footer>
		<p>&copy; EECS 591 Fall 2012</p>
	  </footer>  
	</div>

</body>
</html>
