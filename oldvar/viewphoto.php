<html>
<head></head>
<body>
<div>
<?php
include("dataop.php");
$content=direct_get("photo",$_GET['id']);
list($des,$photo)=split("@",$content);
?>
Description:<?php echo($des); ?> <br/>
Content:<?php echo($photo); ?>
</div>
</body>
</html>
