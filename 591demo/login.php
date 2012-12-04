<!DOCTYPE html>

<?php
  if (!empty($_POST)) {
	$username = $_POST['username'];
	$password = $_POST['password'];
	echo $username;

	//get("user","userid")
  }
?>
<html lang="en">

<head>
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
<link href="bootstrap/css/bootstrap.min.css" rel="stylesheet" media="screen">
<title> Login Page </title>
</head>


<body>
<form name="loginform" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
	<table id = "LoginTable">
	<tr>
		<td> User Name: </td>
		<td> <input type="text" name="username" /> </td>
	</tr>

	<tr>
		<td> Password: </td>
		<td><input type="password" name="password" /> </td>
	</tr>

	<tr>
		<td><input type="submit" name="submit" value="Login" onclick="return loginValidate()"/> </td>
	</tr>
	</table>
</form>

<a href="register.php"> Click Here to Sign Up </a>


</body>
</html>
