import cgi

def main():
    form = cgi.FieldStorage()  # Create instance to hold form data

    # Print HTTP headers
    print("HTTP/1.1 200 OK")
    print("Content-type: text/html\n")

    # Start HTML
    print("<html>")
    print("<head>")
    print("<title>Hello, World!</title>")
    print("</head>")
    print("<body>")
    print("<p>Hello, World!</p>")

    # Iterate through form data and print
    for key in form.keys():
        print(key + ": " + form[key].value)

    # End HTML
    print("</body>")
    print("</html>")

main()
