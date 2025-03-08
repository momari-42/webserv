POST / HTTP/1.1
User-Agent: PostmanRuntime/7.33.0
Accept: */*
Postman-Token: c41682f0-ece0-4271-b9c5-c6cc69d755e1
Host: localhost:1334
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Content-Type: multipart/form-data; boundary=--------------------------401558992028219571836587
Content-Length: 5098

----------------------------401558992028219571836587
Content-Disposition: form-data; name="a"; filename="plan.md"
Content-Type: text/markdown

Your project involves building a configuration file parser for a web server inspired by Nginx. Here's a structured approach to designing such a parser:

---

### 1. **Configuration File Structure**
Design a configuration file format similar to Nginx. It should be human-readable and follow a hierarchical structure with blocks and directives. Example:

```nginx
server {
    listen 80;
    host 127.0.0.1;

    server_name example.com www.example.com;

    error_page 404 /404.html;

    client_max_body_size 1M;

    location / {
        root /var/www/html;
        index index.html;
    }

    location /upload {
        root /var/www/uploads;
        upload_dir /var/uploads/files;
        allow_methods POST GET;
    }

    location /api {
        cgi_pass /usr/bin/python3;
        allowed_methods GET POST;
    }
}
```

---

### 2. **Features Breakdown**

#### Server-Level Configuration:
- **`listen`**: Define the port (e.g., `listen 80;`).
- **`host`**: Define the host IP address (e.g., `host 127.0.0.1;`).
- **`server_name`**: List of server names (e.g., `server_name example.com;`).
- **Default Server**: The first server block for a `host:port` combination is the default.

#### Global Configurations:
- **`error_page`**: Set up default error pages for HTTP codes (e.g., `error_page 404 /404.html;`).
- **`client_max_body_size`**: Limit the size of client body (e.g., `client_max_body_size 1M;`).

#### Route-Level Configuration:
- **`location`**: Define a route with multiple configurations.
    - **Root**: Specify the root directory or file.
    - **Directory Listing**: Enable or disable directory listing.
    - **Default File**: Specify a default file for directories (e.g., `index index.html;`).
    - **HTTP Methods**: Define allowed HTTP methods (e.g., `allow_methods GET POST;`).
    - **Redirection**: Redirect routes (e.g., `return 301 /new-path;`).
    - **CGI Execution**: Define CGI handlers with extensions (e.g., `.php`) and specify the path to the CGI binary.
    - **File Upload**: Enable uploads and configure directories.

---

### 3. **Implementation Plan**

#### Parser Design:
1. **Tokenize Configuration**:
   - Use a lexer to break the configuration into tokens (e.g., keywords, strings, braces, semicolons).
2. **Grammar Parsing**:
   - Implement a recursive-descent parser to handle blocks and directives.
   - Validate the syntax and store parsed values in structured objects (e.g., dictionaries, classes).

#### Data Structures:
- **Server Class**:
  ```python
  class Server:
      def __init__(self, host, port, server_name=None, error_pages=None, client_max_body_size=None):
          self.host = host
          self.port = port
          self.server_name = server_name or []
          self.error_pages = error_pages or {}
          self.client_max_body_size = client_max_body_size
          self.routes = []
  ```
- **Route Class**:
  ```python
  class Route:
      def __init__(self, path, root=None, index=None, allow_methods=None, cgi_pass=None, upload_dir=None):
          self.path = path
          self.root = root
          self.index = index
          self.allow_methods = allow_methods or []
          self.cgi_pass = cgi_pass
          self.upload_dir = upload_dir
  ```

#### Validation:
- Validate mandatory fields (`listen`, `host`, etc.).
- Ensure routes are unique and properly nested.

---

### 4. **CGI Handling**
- **Input Parsing**:
  - Read the request body, unchunk if necessary.
  - Pass the request data to the CGI executable as `stdin`.
  - Set `PATH_INFO` and `QUERY_STRING` as environment variables.
- **Output Parsing**:
  - Parse CGI output headers.
  - 