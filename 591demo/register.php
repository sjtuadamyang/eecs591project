<!DOCTYPE html>

<?php
  		include("dataop.php");
	
		$username = $_GET['username'];
		$password = $_GET['password'];

        $values=array("password"=>$password);
        echo prep_put("user", $username, $values, "null");
?>

<html lang="en">
<head>
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<script type="text/javascript" src="bootstrap/js/bootstrap.min.js"></script>
<link href="bootstrap/css/bootstrap.min.css" rel="stylesheet" media="screen">
<title> Register New User </title>
</head>

<body>

<form name="register" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
	<table id = "register">
	<tr>
		<td> User Name: </td>
		<td> <input type="text" name="username" /> </td>
	</tr>

	<tr>
		<td> Password: </td>
		<td><input type="password" name="password" /> </td>
	</tr>

	<tr>
		<td> Email: </td>
		<td><input type="email" name="email" /> </td>
	</tr>

	<tr>
		<td><input type="submit" name="submit" value="Login" onclick="return loginValidate()"/> </td>
	</tr>
	</table>
</form>


</body>
</html>
