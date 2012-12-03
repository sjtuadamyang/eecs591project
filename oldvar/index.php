<html>
<head>
<style>
#header_in { margin: 0 0 20px 0; width:100%;height:44px; background: #3B5999;}
  .headerwarp_in { margin: 0 auto; padding: 0 0px; width: 1024px; height: 44px; background: url("image/header_in.gif") no-repeat 0 0 ; color: #3B5999; }
    .headerwarp_in a { color: #FFF; }
      .headerwarp_in .logo { float: left; margin: 0 30px 0 0; }
      .headerwarp_in .globalTip{height:26px;display:inline-block;padding:0px 0px 0px 0px;width:60px;}
      		.headerwarp_in .globalTip .notificationTip{background: url("image/tip/notification.gif") no-repeat top left;width:22px;height:20px;margin-top:0px;display:inline-block;text-align:center;cursor:pointer;border:1px solid #3b5999;font-size:8px;}
      		.headerwarp_in .globalTip .inMailTip{background: url("image/tip/inmail.gif") no-repeat top left;width:22px;height:20px;margin-top:0px;display:inline-block;text-align:center;cursor:pointer;border:1px solid #3b5999;font-size:8px;}
      		.headerwarp_in .globalTip .friendRequestTip{background: url("image/tip/friendreq.gif") no-repeat top left;width:22px;height:20px;margin-top:0px;display:inline-block;text-align:center;cursor:pointer;border:1px solid #3b5999;font-size:8px;}
      .headerwarp_in .globalSearch{height:28px;display:inline-block;padding:0px 0px 0px 0px;}

#header { margin: 0 0 20px 0;; width:100%; background: #3B5999;}
  .headerwarp { margin: 0 auto; padding: 0 20px; width: 1024px; height: 50px; background: url("image/header.gif") no-repeat 0 0 ; color: #FFF; }
    .headerwarp a { color: #FFF; }
      .headerwarp .logo { float: left; margin: 0 30px 0 0; }
      .headerwarp .globalTip{height:40px;display:inline-block;padding:6px 8px 0px 0px;width:60px;}
      		.headerwarp .globalTip .notificationTip{color:red;background: url("image/tip/notification.gif") no-repeat;width:24px;height:20px;display:inline-block;text-align:center;}
      		.headerwarp .globalTip .inMailTip{color:red;background: url("image/tip/inmail.gif") no-repeat;width:24px;height:20px;display:inline-block;text-align:center;}
      		.headerwarp .globalTip .friendRequestTip{color:red;background: url("image/tip/friendreq.gif") no-repeat;width:24px;height:20px;display:inline-block;text-align:center;}
      .headerwarp .globalSearch{height:40px;display:inline-block;padding:6px 8px 0px 0px;font-size:11px;}
.numberTip{color:white;background-color:red;font-size:10px;line-height:10px;height:10px;width:12px;display:none;padding-bottom:2px;}
*html .numberTip{color:white;background-color:red;font-size:10px;line-height:10px;height:10px;width:12px;display:none;padding-bottom:2px;margin-left:-12px;} 
.menu { float: left; height: 20px; font-size: 11px; font-weight: bold; padding:0px 0px 0px 0px;}
  .menu li { float: left; padding: 5px ; line-height: 20px; vertical-align: top; }
    .menu .edit { padding-left: 4px; color: #FFF; font-size: 12px; font-weight: normal; }

.nav_account { float: right; width: 200px; white-space:nowrap; overflow: hidden; }
  .login_thumb { float: left; margin: 6px 6px 0 0; padding: 3px 0 0 3px; width: 24px; height: 24px; background: url(image/thumb20bg.gif) no-repeat; display: block; }
  .login_thumb img{ width: 20px; height: 20px; }
    li.notify { margin: 9px 0 0; padding: 0 0 0 18px; width: 72px; height: 20px; background: url(image/notify_bg.gif); color: #FFF; font-size: 12px; font-weight: normal; }

.menu li.dropmenu { margin: 0 10px 0 0; padding: 10px 15px 10px 10px; height: 20px; background: url(image/triangle.gif) no-repeat right center; overflow: hidden; }
  .dropmenu_drop { margin: -1px 0 0; padding: 0 10px 10px; width: 150px; border: 1px solid #E9E9E9; background: #FFF; z-index: 200; }
      .dropmenu_drop li { padding: 5px 10px; border-bottom: 1px solid #EEE; }
        .dropmenu_drop li a { color: #2C629E; font-size: 12px; font-weight: normal; }
		.dropmenu_drop .active { font-weight: bold; }
		
.menu li.topdropmenu { margin: 0 5px 0 0; padding: 5px 15px 5px 5px; height: 20px; background: url(image/triangle.gif) no-repeat right center; overflow: hidden; }
  .topdropmenu_drop { margin: -1px 0 0; padding: 0 10px 10px; width: 140px; border: 1px solid #3B5999; background: #FFFFFF; z-index: 200; }
      .topdropmenu_drop li {float: left; padding: 5px 10px; border-bottom: 1px dashed #BBB; text-aglin:left; width:110px; }
        .topdropmenu_drop li a { color: #2C629E; font-size: 12px; font-weight: normal; }
        .topdropmenu_drop li a:hover{text-decoration:none;}
		.topdropmenu_drop .active { font-weight: bold; }
		.topdropmenu_drop li.lastlist{border:0px; padding-bottom:0px;}

</style>

</head>
<body>

<div id="header_in">
			<div class="headerwarp_in">
			<div style="width:60%; height:13px;"><a><span>&nbsp;</span></a></div>
			 
			 <div style="height: 30px;">
				<div style="float: left; width: 450px;height:20px;">
					<div class="globalSearch" style="float: right;">
						<form action="resource/search">
							<input type="text" id="globalSearch" name="globalSearch" class="t_input" style="border: 1px solid #3B5999; color: #CCC; height:17px;font-color:lightgray;" value="Search Person">
							<input type="submit" value="Search" class="submit" style="margin-left: -5px;height:25px;font-size:10px; font-weight: bolder;">
								<input type="hidden" name="struts.token.name" value="struts.token">
<input type="hidden" name="struts.token" value="P121QNBY80DBZI6KJ64MIMX85A15O9DT">
						</form>
					</div>
					
						<div class="globalTip" style="float: right;width: 80px;">
						<table cellspacing="0px" cellpadding="0px">
						<tbody><tr><td style="padding-top:0;">
						<div class="notificationTip" title="Notification" style="background-image: url(resource/image/tip/notification.gif); background-attachment: initial; background-origin: initial; background-clip: initial; background-color: initial; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; border-top-style: solid; border-right-style: solid; border-bottom-style: solid; border-left-style: solid; border-top-color: rgb(59, 89, 153); border-right-color: rgb(59, 89, 153); border-bottom-color: rgb(59, 89, 153); border-left-color: rgb(59, 89, 153); background-position: initial initial; background-repeat: no-repeat no-repeat; ">
								<div class="numberTip" style="display: none; "></div>
						</div>
						</td>
						
						<td style="padding-top:0;">
						<div class="inMailTip" title="Mail" style="background-image: url(resource/image/tip/inmail.gif); background-attachment: initial; background-origin: initial; background-clip: initial; background-color: initial; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; border-top-style: solid; border-right-style: solid; border-bottom-style: solid; border-left-style: solid; border-top-color: rgb(59, 89, 153); border-right-color: rgb(59, 89, 153); border-bottom-color: rgb(59, 89, 153); border-left-color: rgb(59, 89, 153); background-position: initial initial; background-repeat: no-repeat no-repeat; ">
								<div class="numberTip" style="display: none; "></div>
						</div>
						</td>
						
						<td style="padding-top:0;">
						<div class="friendRequestTip" title="Connection Request" style="background-image: url(resource/image/tip/friendreqH.gif); background-attachment: initial; background-origin: initial; background-clip: initial; background-color: initial; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; border-top-style: solid; border-right-style: solid; border-bottom-style: solid; border-left-style: solid; border-top-color: rgb(59, 89, 153); border-right-color: rgb(59, 89, 153); border-bottom-color: rgb(59, 89, 153); border-left-color: rgb(59, 89, 153); background-position: initial initial; background-repeat: no-repeat no-repeat; ">
								<div class="numberTip" style="display: block; ">1</div>
						</div>
						</td></tr>
					</tbody></table>
					</div>
					<div id="noUse" style="float: right;width: 40px;height:20px;">
						
					</div>
					<div id="toHome" style="float: right;width: 100px;height:20px;">
						
					</div>
					</div>
					
					<div style="float: right;height:20px;">
						<ul class="menu">
							<li>
								<a href="home.htm">Home</a>
							</li>
							<li>
								<a href="resource/profile.action">Profile</a>
							</li>
							<li>
								<a href="resource/myGroup.action" target="_blank">Group</a>
							</li>
							<li class="topdropmenu" id="topmenu" onmouseover="showMenu(this.id)">
								<a>Account</a>
							</li>
						</ul>
					</div>
				</div>
				
				<ul id="topmenu_menu" class="topdropmenu_drop" style="position: absolute; z-index: 50; left: 765px; top: 40px; clip: rect(auto, auto, auto, auto); display: none;width:130px;">
					<li style="padding-left: 5px;">
						<table>
							<tbody><tr>
								<td align="left">
									<a href="resource/setting/avatar.jsp">
										<img width="32px;" height="32px;" id="avatar" src="./resource/249_mini.jpg" onerror="this.onerror=&#39;&#39;;this.src=&#39;image/avatar/noavatar_mini.jpg&#39;">
									</a>
								</td>
								<td>
								<span style="color:#3B5999;">
									Jackie HAN
								</span>	
								</td>
							</tr>
						</tbody></table>
						
					</li>
					<li>
						<a href="resource/inboxInMail.action">Mail</a>
					</li>
					<li>
						<a href="resource/inboxNotification.action">Notification</a>
					</li>
					<li>
						<a href="resource/getUserInfo.action">Setting</a>
					</li>
					<li>
						<a href="resource/certificate/request.jsp">Certificate</a>
					</li>
					<li class="lastlist">
						<a href="resource/logout.action">Log Out</a>
					</li>
				</ul>
			</div>


		</div>

<?php 
include("dataop.php");
//prepare for get
//for debug
$_GET['id']=1;
$div1=direct_get("user",$_GET['id'],"handler1");
$div3=prep_get("friends",$_GET['id']);
flush_now();
?>

<div id="user_info" style="float:left;margin-left:10%;width:10%;">
<?php
list($username,$password,$gender,$birth)=split("@",get("user",$_GET['id']));
?>
	<br />
	UserInfo<br />
	Name: <?php echo($username); ?><br />
	Gender: <?php echo($gender); ?><br />
	Birth: <?php echo($birth); ?><br />
	<br />
</div>
<div id="new_feeds" style="float:left;width:40%;">Feeds:</div>
<div id="friends" style="float:right;margin-right:10%;width:10%;">Friends:
<?php echo($div3); ?>
</div>

<script>
</script>
</body>
</html>
	
