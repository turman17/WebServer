
import cgi
import os

def main():
	form = cgi.FieldStorage()
	upload_directory = os.environ['UPLOAD_DIR']
	domain = "http://" + os.environ['DOMAIN']
	if 'filename' in form:
		fileitem = form['filename']
		if fileitem.filename:
			if not upload_directory.endswith('/'):
				upload_directory += '/'
			file_path = upload_directory + fileitem.filename
			with open(file_path, 'wb') as file:
				file.write(fileitem.file.read())


	print("HTTP/1.1 200 OK")
	print("Content-type: text/html\n")
	print("<html>")
	print("<head>")
	print("<title>Hello, World!</title>")
	print("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-QWTKZyjpPEjISv5WaRU9OFeRpok6YctnYmDr5pNlyT2bRjXh0JMhjY6hW+ALEwIH\" crossorigin=\"anonymous\">")
	print("</head>")
	print("<body>")
	print("<div class=\"container col-xl-10 col-xxl-8 px-4 py-5 text-center\">")
	print("<h2>")
	if 'filename' in form and fileitem.filename:
		print(f'File was saved at <a href="{domain + "/" + upload_directory}" style="text-decoration: underline;"> {domain + "/" + upload_directory} </a>')
	else:
		print("No file was submited")
	print("</h2>")
	print("</body>")
	print("</html>")

main()	
