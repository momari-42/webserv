#!/usr/bin/env python3

import os

# Define possible error codes and messages
ERROR_MESSAGES = {
    "400": "Bad Request",
    "403": "Forbidden",
    "404": "Not Found",
    "500": "Internal Server Error",
}

# Get the error code from QUERY_STRING (GET request)
query_string = os.environ.get("QUERY_STRING", "")
error_code = "500"  # Default error code
if "error_code=" in query_string:
    error_code = query_string.split("error_code=")[-1]

# Validate the error code
if error_code not in ERROR_MESSAGES:
    error_code = "500"

# Print HTTP headers
# print(f"Status: {error_code} {ERROR_MESSAGES[error_code]}")
# print("Content-Type: text/html")
# print()  # Blank line to separate headers from body

print(f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{error_code} {ERROR_MESSAGES[error_code]}</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            text-align: center;
            padding: 50px;
            background-color: #f8d7da;
        }}  
        h1 {{
            color: #721c24;
        }}
        p {{
            color: #721c24;
        }}
    </style>
</head>
<body>
    <h1>Error {error_code} - {ERROR_MESSAGES[error_code]}</h1>
    <p>Sorry, something went wrong.</p>
</body>
</html>
""")

# Print HTML error page
