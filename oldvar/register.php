<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
<meta http-equiv="Content-Language" content="zh-cn" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题 1</title>
</head>

<body>
<?php
//flush now
function flush_now(){
	ob_flush();
	flush();
}
function get($keystore,$key){
	//first get the id of receive queue
	$message_queue=msg_get_queue(20000,0666);
	msg_receive($message_queue,1,$message_type,32,$id,false);
	//get id ok
	$message_queue=msg_get_queue(20123,0666);
	//create a receive queue
	$rec_queue=msg_get_queue($id,0666);
	//send the message to request something
	msg_send($message_queue,1,$id.":g:".$keystore.":".$key);
	//receive the return message
	msg_receive($rec_queue,1,$message_type,102400,$message,false);
	//remove the message queue
	msg_remove_queue($rec_queue);
	return $message;
}
function put($keystore,$key,$value){
	//first get the id of receive queue
	$message_queue=msg_get_queue(20000,0666);
	msg_receive($message_queue,1,$message_type,32,$id,false);
	//get id ok
	$message_queue=msg_get_queue(20123,0666);
	//create a receive queue
	$rec_queue=msg_get_queue($id,0666);
	//send the message to request something
	msg_send($message_queue,1,$id.":p:".$keystore.":".$key.":".$value);
	//receive the return message
	msg_receive($rec_queue,1,$message_type,8,$message,false);
	//remove the message queue
	msg_remove_queue($rec_queue);
	return $message;
}
?>
<?php
echo put("user",$_POST['username'],$_POST['password']."@".$_POST['gender']."@".$_POST['birth']);
echo put("photo",$_POST['username'."_0"],$_POST['photo']);
?>
<div>
	<form method="get" action="register.php">
		用户名：<input name="username" type="text" /><br />
		密码：<input name="password" type="text" /><br />
		性别：<input name="gender" type="text" /><br />
		出生日期：<input name="birth" type="text" /><br />
		<input name="submit" type="submit" value="确定" /></form>
</div>

</body>

</html>
