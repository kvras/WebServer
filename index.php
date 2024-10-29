<?php
// Get the query string
$query_string = $_SERVER['QUERY_STRING'];

// Get the raw POST body
$raw_body = file_get_contents('php://input');

// Display the results
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI Test</title>
</head>
<body>
    <h1>CGI Test</h1>
    <h2>Query String</h2>
    <pre><?php echo htmlspecialchars($query_string); ?></pre>
    
    <h2>Request Body</h2>
    <pre><?php echo htmlspecialchars($raw_body); ?></pre>
</body>
</html>
