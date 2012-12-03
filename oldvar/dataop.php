<?php
//flush now
function flush_now(){
	ob_flush();
	flush();
}
function direct_get($keystore,$key,$handler){
        //first get the id of receive queue
        $message_queue=msg_get_queue(20000,0666);
        msg_receive($message_queue,1,$message_type,32,$id,false);
        $id=intval($id);
	//get id ok
	echo $id;
        $message_queue=msg_get_queue(20123,0666);
        //create a receive queue
        $rec_queue=msg_get_queue($id,0666);
        //send the message to request something
	$content=$id.":g:".$keystore.":".$key.":".$handler;
	echo $content;
        msg_send($message_queue,1,$id.":g:".$keystore.":".$key.":".$handler);
        //receive the return message
        msg_receive($rec_queue,1,$message_type,102400,$message,false);
        //remove the message queue
        msg_remove_queue($rec_queue);
        return $message;
}

function prep_get($keystore,$key,$handler){
	//first get the id of receive queue
	$message_queue=msg_get_queue(20000,0666);
	msg_receive($message_queue,1,$message_type,32,$id,false);
	//get id ok
	$message_queue=msg_get_queue(20123,0666);
        $id=intval($id);
	//create a receive queue
	$rec_queue=msg_get_queue($id,0666);
	//send the message to request something
	msg_send($message_queue,1,$id.":g:".$keystore.":".$key.":".$handler);
	return $rec_queue;
	//receive the return message
	//msg_receive($rec_queue,1,$message_type,102400,$message,false);
	//remove the message queue
	//msg_remove_queue($rec_queue);
	//return $message;
}
function get($rec_queue){
	//receive the return message
	msg_receive($rec_queue,1,$message_type,102400,$message,false);
	//remove the message queue
	msg_remove_queue($rec_queue);
	return $message;
}
function get_nowait($rec_queue, &$errorcode){
	//receive the return message
        msg_receive($rec_queue,1,$message_type,102400,$message,false,MSG_IPC_NOWAIT,$errorcode);
	if($errorcode!=MSG_ENOMSG)
        //remove the message queue
	        msg_remove_queue($rec_queue);
        return $message;
}

function prep_put($keystore,$key,$value,$handler){
	//first get the id of receive queue
	$message_queue=msg_get_queue(20000,0666);
	msg_receive($message_queue,1,$message_type,32,$id,false);
	//get id ok
        $id=intval($id);
	$message_queue=msg_get_queue(20123,0666);
	//create a receive queue
	$rec_queue=msg_get_queue($id,0666);
	//send the message to request something
	//construct the values
	$values="";
	foreach($value as $attr=>$v){
		$values.=$attr."@".$v."@";
	}
	msg_send($message_queue,1,$id.":p:".$keystore.":".$key.":".$values.":".$handler);
	//receive the return message
	msg_receive($rec_queue,1,$message_type,8,$message,false);
	//remove the message queue
	msg_remove_queue($rec_queue);
	return $message;
}
?>
