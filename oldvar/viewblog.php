<html>
<head></head>
<body>
<div>
<?php
include("dataop.php");
$content=search("phonebook","first","adam");
foreach ($content as $cont)
    echo($cont);
?>
</div>
</body>
</html>
