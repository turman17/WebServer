#!/usr/bin/env python3
import cgi

def main():
    print("HTTP/1.1 200 OK")
    print("Content-type: text/html\n")
    form = cgi.FieldStorage()  # Assuming there's some form data to handle, like a username

    username = form.getvalue("username", "Guest")  # Get a username from the form data or default to 'Guest'

    # HTML content with dynamic greetings based on the username
    print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Welcome Page</title>
</head>
<body>
    <h1>Welcome, {username}!</h1>
    <p>Thank you for logging in. We're glad to see you back!</p>
</body>
</html>
""")

if __name__ == "__main__":
    main()
