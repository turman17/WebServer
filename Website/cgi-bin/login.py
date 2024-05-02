import cgi
import cgitb
cgitb.enable()  # Enable debugging

def main():
    print("Content-type: text/html")  # Make sure this is the first output
    print()  # Ends the header section
    form = cgi.FieldStorage()
    
    if "email" in form and "password" in form:
        email = form.getvalue("email")
        password = form.getvalue("password")
        
        if email == "user@example.com" and password == "secure123":
            print('<html>')
            print('<head>')
            print('<meta http-equiv="refresh" content="0;url=/path/to/success_page.html" />')
            print('</head>')
            print('<body>')
            print('<p>Redirecting...</p>')  # Optional
            print('</body>')
            print('</html>')
        else:
            print("<h1>Invalid email or password.</h1>")
    else:
        print("<h1>Error: Please enter both email and password.</h1>")

if __name__ == "__main__":
    main()
