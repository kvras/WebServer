<?php
if ($_SERVER["REQUEST_METHOD"] == "GET") {
    $name = htmlspecialchars($_GET['name']);
    $age = (int)$_GET['age'];
    $currentYear = date("Y");
    $birthYear = $currentYear - $age;
    echo "Hi $name, you were born in $birthYear.";
}
?>