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
<script type='text/javascript'>
function show(id,text){
        document.getElementById(id).innerHTML=text;
}
</script>
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
<link href="bootstrap/css/bootstrap.min.css" rel="stylesheet"
	media="screen">
<link href="css/main.css" rel="stylesheet" media="screen">
<title>Main Page</title>
</head>

<?php
//prepare for the message get
$content1=prep_get("user", $username, "dummy");
$content2=prep_get("user", $username, "dummyS");
?>

<body>
	<!-- Page Navigation -->
	<div class="navbar navbar-inverse navbar-fixed-top">
		<div class="navbar-inner">
			<div class="container-fluid">
				<a class="btn btn-navbar" data-toggle="collapse"
					data-target=".nav-collapse"> <span class="icon-bar"></span> <span
					class="icon-bar"></span> <span class="icon-bar"></span>
				</a> <a class="brand" href="#">EECS 591 Demo</a>
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
				</div>
				<!--/.nav-collapse -->
			</div>
		</div>
	</div>

	<div class="container-fluid">
		<?php
		echo "<p id='welcome'>Welcome back, " . $username . "</p>";
		flush_now();
		?>
	</div>

	<!-- Page Header -->
	<div class="container-fluid">
		<div class="row-fluid">
			<div class="span12">
				<div class="well">
					<img id="profile" src="img/wuyanzu.png" width=60 height=50>
					"Personal Status Update" and Advertisement <br />
					<form class="form-inline" name="blogform"
						action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
						<lable> I am thinking ... </lable>
						<input class="input-xlarge" type="text" name="blogpost" />&nbsp
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
				Upload a Photo <br /> <br />
				<form class="form" name="uploadform"
					action="<?php echo 'upload.php?username=' . $username; ?>"
					method="post" enctype="multipart/form-data">
					<label for="file">Filename:</label> <input type="file" name="file"
						id="file"><br> <input type="submit" class="btn btn-success"
						name="submit" value="Upload">
				</form>

			</div>
			<div id="large" class="span6 mainbody well">
				<!--Body content-->

			</div>
			<div id="small" class="span3 rightsidebar">
				<!--Sidebar content-->
				My Photos <br />


			</div>
		</div>

		<!-- Page Footer -->
		<hr />
		<footer>
			<p>&copy; EECS 591 Fall 2012</p>
		</footer>
	</div>
	<?php
	//first flush
	flush_now();
	//begin to poll the status of each content
	$content=array($content1,$content2);
	$pagelet_name=array("large","small");
	$complete=array(false,false);
	$i=0;
	while($i<2){
        $j=0;
        while($j<2){
                if($complete[$j]==false){
                        $errorcode=0;
                        $message=get_nowait($content[$j],$errorcode);
                        if($errorcode!=MSG_ENOMSG){
                                ++$i;
                                $complete[$j]=true;
                                //LOAD THE PAGELET
                                //echo "start loading the pagelet.";
                                echo "<script>show('",$pagelet_name[$j],"','<img src=\"data:image/png;base64, ",$message,"\">')</script>";
                                //e//cho "get the message from queue",$j;
                                flush_now();
                                }
                                }
                                ++$j;
                     }
        }
?>


</body>
</html>
