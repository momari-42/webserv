<?php
$queryString = getenv("QUERY_STRING");

if ($queryString) {
    parse_str($queryString, $params);

    if (isset($params['name']) && isset($params['age'])) {
        $name = $params['name'];
        $age = $params['age'];

        echo "Name: " . htmlspecialchars($name) . "<br>";
        echo "Age: " . htmlspecialchars($age) . "<br>";
    } else {
        echo "Name and age not provided in the query string.";
    }
} else {
    echo "QUERY_STRING is not set.";
}
?>
