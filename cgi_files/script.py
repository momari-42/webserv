
import subprocess

subprocess.run('curl --http1.1 -X GET -H "Authorization: Bearer YOUR_TOKEN" -d "{\"key\": \"value\"}" http://localhost:1313/cgi_files/script.py', shell=True)
