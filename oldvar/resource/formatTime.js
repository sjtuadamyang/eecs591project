//得到回复发布的时间距离现在的时间差Json返回时间
function getTimeJson(date) {
	var nowdate = new Date();
	var strYear = date.substring(0, 4);
	var strMonth = date.substring(5, 7);
	var strDay = date.substring(8, 10);
	var strHours = date.substring(11, 13);
	var strMinutes = date.substring(14, 16);
	//月份是从0-11
	jsdate = new Date(strYear, strMonth - 1, strDay, strHours, strMinutes);
	var dateM = nowdate - jsdate;
	var MinMilli = 1000 * 60; // 初始化变量。  
	var HrMilli = MinMilli * 60;
	var DyMilli = HrMilli * 24;
	var days = Math.floor(dateM / (DyMilli));
	//计算出小时数  
	var leave1 = dateM % (DyMilli); //计算天数后剩余的毫秒数  
	var hours = Math.floor(leave1 / (HrMilli));
	//计算相差分钟数  
	var leave2 = leave1 % (HrMilli); //计算小时数后剩余的毫秒数  
	var minutes = Math.floor(leave2 / (MinMilli));
	//计算相差秒数  
	var leave3 = leave2 % (MinMilli); //计算分钟数后剩余的毫秒数  
	var seconds = Math.round(leave3 / 1000);
	//当天的事件  
	if (days == "0") {
		//一个小时之内的时间
		if (hours == "0") {
			//一分钟之内的算作一分钟
			if (minutes == "0") {
				return 1 + " minutes ago";
			} else {
				return minutes + " minutes ago";
			}
			//一天之内的小时数
		} else {
			return hours + " hours ago";// + minutes + " minutes ago"
		}
	} else if (days == "1") {
		return "Yesterday at " + strHours + ":" + strMinutes;
	} else {
		return days + " days ago at " + strHours + ":" + strMinutes;
	}
}

//得到回复发布的时间距离现在的时间差Json返回时间
function getTime(date){
			var nowdate = new Date();
			var a=date.split(" ");
			var b=a[0].split("-");
			var c=a[1].split(":");
			//Date(b[0],b[1]-1,b[2],c[0],c[1],c[2]);
			var strYear=b[0];  
			strYear = "20" + strYear;
			var strMonth=b[1];
			var strDay=b[2];
			var strHours=c[0];  
			var strMinutes=c[1];  
			//月份是从0-11
			jsdate = new Date(strYear,strMonth - 1,strDay,strHours,strMinutes);
			//alert(jsdate);
			var dateM = nowdate - jsdate;
			var MinMilli = 1000 * 60;         // 初始化变量。  
		    var HrMilli = MinMilli * 60;  
		    var DyMilli = HrMilli * 24;  
		    var days=Math.floor(dateM/(DyMilli)); 
		    //计算出小时数  
		    var leave1=dateM%(DyMilli); //计算天数后剩余的毫秒数  
		    var hours=Math.floor(leave1/(HrMilli));  
		    //计算相差分钟数  
		    var leave2=leave1%(HrMilli);        //计算小时数后剩余的毫秒数  
		    var minutes=Math.floor(leave2/(MinMilli));  
		    //计算相差秒数  
		    var leave3=leave2%(MinMilli);      //计算分钟数后剩余的毫秒数  
		    var seconds=Math.round(leave3/1000);
		    //当天的事件  
		    if(days == "0")
		    {
			    //一个小时之内的时间
				if(hours == "0"){
					//一分钟之内的算作一分钟
					if(minutes == "0"){
						return 1 + " minutes ago";
					}else{
						return minutes + " minutes ago";
					}
					//一天之内的小时数
				}else{
					return hours + " hours ago";// + minutes + " minutes ago"
				}
			}else if(days == "1"){
				return "Yesterday at " + strHours + ":" + strMinutes;
			}else{
				//return days + " days ago at " + strHours + ":" + strMinutes;
				return strYear + "-" + strMonth + "-" + strDay + " " + strHours + ":" + strMinutes;
			}
		}

function formatDate(date)
{alert(date);
	var allStr=date.split(" ")
	var dateStr = allStr[0];
	var strYear=dateStr[0];  
	strYear = "20" + strYear;
	var strMonth=dateStr[1];
	var strDate=dateStr[2];
	
	return strYear+"-"+strMonth+"-"+strDate; 
}

function formatHourMinute(hour,minute)
{
	var hourStr,minuteStr;
	if(hour<10)
		hourStr = "0"+hour;
	else
		hourStr = hour;
	
	if(minute<10)
		minuteStr = "0"+minute;
	else
		minuteStr = minute;
	return hourStr+" : "+minuteStr;
}

function formatDateHourMinute(date,hour,minute)
{
	var dateStr = formatDate(date);
	var hourStr,minuteStr;
	if(hour<10)
		hourStr = "0"+hour;
	else
		hourStr = hour;
	
	if(minute<10)
		minuteStr = "0"+minute;
	else
		minuteStr = minute;
		
	return dateStr+"  "+hourStr+" : "+minuteStr;
}