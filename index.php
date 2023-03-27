<?php
// Set the URL of the website you want to download chief
$url = 'http://website-to-download.com';

// Initialize cURL
$ch = curl_init();

// Set the options for cURL
curl_setopt($ch, CURLOPT_URL, $url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);

// Execute the cURL request and get the response
$response = curl_exec($ch);

// Check for errors
if(curl_errno($ch)) {
    echo 'Error: ' . curl_error($ch);
}

// Close the cURL session
curl_close($ch);

// Save the response to a file
$file = 'example.html';
file_put_contents($file, $response);
?>
