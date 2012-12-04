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
			<form class="form" name="uploadform" action="upload.php?username=Andy" method="post" enctype="multipart/form-data">
			<label for="file">Filename:</label>
			<input type="file" name="file" id="file"><br>
			<input type="submit" class="btn btn-success" name="submit" value="Upload">
			</form>

	    </div>
	    <div class="span6 mainbody well">
	      <!--Body content-->
	      <?php
	      	//tri_put version: triget and foreach get
	      	$photolist = triget("user", "Andy", "getpid");
	      	$photo = explode("@", $photolist);
	      	foreach ($photo as $p) {
	      		$imagedata = direct_get("photo_l", $p);
	      		echo "<img src='data:image/png;base64, " . $imagedata . "'>";
	      	}

	      	//noraml put: call search and foreach get
	      	/*
	      	$searchval = "11";
	      	$photolist = search("photoinfo", "time", $searchval);

	      	//loop
	      	$photot_id = $photolist[0]["photoid"];
	      	$imagedata = get("photo_l", $photot_id);
	      	echo "<img src='data:image/png;base64, " . $imagedata . "'>";
	        echo "<p>@User1 uploaded a photo</p>";
            */
	      ?>
	      <!--
		  <img src="data:image/png;base64, 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">
		  <img src="data:image/png;base64, iVBORw0KGgoAAAANSUhEUgAAABwAAAASCAMAAAB/2U7WAAAABlBMVEUAAAD///+l2Z/dAAAASUlEQVR4XqWQUQoAIAxC2/0vXZDrEX4IJTRkb7lobNUStXsB0jIXIAMSsQnWlsV+wULF4Avk9fLq2r8a5HSE35Q3eO2XP1A1wQkZSgETvDtKdQAAAABJRU5ErkJggg==">
		  -->
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
	      	//get a photo from database 
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
