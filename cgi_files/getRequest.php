<?php
// Simulating the QUERY_STRING environment variable using $_SERVER
// putenv("QUERY_STRING=name=momari&age=9090");  // Set QUERY_STRING through the environment

// Access the QUERY_STRING environment variable
$queryString = getenv("QUERY_STRING");

// Check if the query string is set
if ($queryString) {
    // Parse the query string
    parse_str($queryString, $params);
    
    // Check if 'name' and 'age' exist in the query string
    if (isset($params['name']) && isset($params['age'])) {
        // Get name and age from the parsed query string
        $name = $params['name'];
        $age = $params['age'];
        
        // Print the name and age
        echo "Name: " . htmlspecialchars($name) . "<br>";
        echo "Age: " . htmlspecialchars($age) . "<br>";
    } else {
        echo "Name and age not provided in the query string.";
    }
} else {
    echo "QUERY_STRING is not set.";
}
?>
