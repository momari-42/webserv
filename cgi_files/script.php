#!/usr/bin/php-cgi
<?php
// Set the Content-Type header
header("Content-Type: text/html");

// Get the query string
$query_string   = getenv("QUERY_STRING");
$method         = getenv("METHOD");
// Parse query parameters
parse_str($query_string, $params);

// Display response
echo "<html><body>";
echo "<h1>PHP CGI " . $method  . " Request</h1>";
echo "<p>Name: " . ($params['name'] ?? 'Guest') . "</p>";
echo "<p>Age: " . ($params['age'] ?? 'Unknown') . "</p>";
echo "</body></html>";
?>
