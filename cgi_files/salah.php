<?php
// The URL of the CGI script
$url = "http://localhost:1313/cgi_files/salah.php";

// Initialize a cURL session
$ch = curl_init($url);

// Set cURL options to use HTTP/1.1
curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);  // Force HTTP/1.1
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);  // Return the response as a string

// Execute the cURL request and get the response
$response = curl_exec($ch);

// Check if there was an error
if(curl_errno($ch)) {
    echo 'cURL Error: ' . curl_error($ch);
} else {
    // Print the response from the CGI script
    echo $response;
}

// Close the cURL session
curl_close($ch);
?>
