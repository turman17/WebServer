import cgi

def main():

	#while True:
		
	form = cgi.FieldStorage()
	print("HTTP/1.1 200 OK")
	print("Content-type: text/html\n")
	print("<html>")
	print("<head>")
	print("<title>Hello, World!</title>")
	# print("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH\" crossorigin=\"anonymous\">")
	print("</head>")
	print("<body>")
	print("<div class=\"container col-xl-10 col-xxl-8 px-4 py-5 text-center\">")
	print("<h1 class=\"h3 m-3 fw-normal\">Login Sucessfull</h1>")
	print("<p>")
	print("Email: " + form['email'].value + "<br>")
	print("Password: " + form['password'].value + "<br>")
	print("</p>")
	print("</div>")
	print("</body>")
	print("</html>")

main()	
