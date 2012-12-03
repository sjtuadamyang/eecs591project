function showFace(btnId,offx,offy)
{
	var x,y;
	var numargs   =   arguments.length;
	if(numargs == 1)
	{
			x = 0;
			y = 0;
	}
	else if(numargs == 2)
	{
			x = arguments[1];
			y = 0;
	}else if(numargs == 3)
	{
			x = arguments[1];
			y = arguments[2];
	}
	var offset = $("#"+btnId).offset();
	var left = offset.left;
	var top = offset.top;
	
	$("#comment_face_menu").css("left",left-200+x);
	$("#comment_face_menu").css("top",top-10+y);
	$("#comment_face_menu").css("display","block");
}

function insertFace(textId,index)
{
	if(index <=0 || index>30)	//一共30个表情
		return;
	var oldContent = $("#"+textId).val();

	//在文本框中添加 [fc1] [fc2] ...
	var newContent = oldContent  + "[fc"+index+"]";
	
	$("#"+textId).text(newContent);	
	$("#comment_face_menu").css("display","none");
}

// [fc1]-> <img />
function decodeFace(str)
{
	if(str.length ==0 || str == null)
		return;
	var tmp;
	do
	{
		tmp = str;
		for(var i=1;i<=30;i++)
		{
			str = str.replace("[fc"+i+"]","<img src='image/face/"+i+".gif' />");
		}
	}while(tmp!=str);
	return str;
}
