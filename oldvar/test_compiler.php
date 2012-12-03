<html>
<head><script type='text/javascript'>function show(id,text){document.getElementById(id).innerHTML=text;}</script></head>
<body><?php include("dataop.php");$content0=prep_get("phonebook","jsmith1","testtrigger");$content1=prep_get("blog","2","handler2");$content2=prep_get("user","1","handler1");?>
<div>phonebook_before Preare
<div id="phonebook"></div>
</div>
<div>blog before 
<div id="blog"></div>
</div>
<div>user before 
<div id="user"></div>
</div>
<?php flush_now();function get0($message){return "name:".$message."test:</br>";}function get1($message){}function get2($message){}function selectfunction($message,$index){switch($index){case 0:return get0($message);break;case 1:return get1($message);break;case 2:return get2($message);break;}}$content=array($content0,$content1,$content2);$pagelet_name=array("phonebook","blog","user");$complete=array(false,false,false);$i=0;while($i<3){$j=0;while($j<3){if($complete[$j]==false){$errorcode=0;$message=get_nowait($content[$j],$errorcode);if($errorcode!=MSG_ENOMSG){++$i;$complete[$j]=true;echo "<script>show('",$pagelet_name[$j],"','",selectfunction($message,$j),"')</script>";flush_now();}}++$j;}}?>
</body>
</html>
