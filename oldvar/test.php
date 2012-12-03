<html>
<head>
<!--this script is for loading pagelet--!>
<script type='text/javascript'>
function show(id,text){
	document.getElementById(id).innerHTML=text;
}
</script>
</head>
<body>
<?php
include("dataop.php");
//prepare for the message get
$content1=prep_get("phonebook","jsmith1","testtrigger");
$content2=prep_get("blog","2","handler2");
$content3=prep_get("user","1","handler1");
?>
<div>phonebook_before Preare<div id="phonebook"></div></div>
<div>blog before <div id="blog"></div></div>
<div>user before <div id="user"></div></div>
<?php
//first flush 
flush_now();
//begin to poll the status of each content
echo "start poll..";
$content=array($content1,$content2,$content3);
$pagelet_name=array("phonebook","blog","user");
$complete=array(false,false,false);
$i=0;
while($i<3){
	$j=0;
	while($j<3){
		if($complete[$j]==false){
			$errorcode=0;
			$message=get_nowait($content[$j],$errorcode);
			if($errorcode!=MSG_ENOMSG){
				++$i;
				$complete[$j]=true;
				//LOAD THE PAGELET
				echo "start loading the pagelet.";
				echo "<script>show('",$pagelet_name[$j],"','",$message,"')</script>";
				echo "get the message from queue",$j;
				flush_now();
			}
		}
		++$j;
	}
}
?>
</body>
</html>
