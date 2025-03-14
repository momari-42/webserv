Welcome in this little webserver tester.
Passing the test here is the minimum before going to an evaluation.

press enter to continue
THIS TEST IS NOT MEANT TO BE THE ONLY TEST IN THE EVALUATION!!!
	
press enter to continue
Before starting please follow the next few steps (files content can be anything and will be shown to you by the test):
- Download the cgi_test executable on the host
- Create a directory YoupiBanane with:
	-a file name youpi.bad_extension
	-a file name youpi.bla
	-a sub directory called nop
		-a file name youpi.bad_extension in nop
		-a file name other.pouic in nop
	-a sub directory called Yeah
		-a file name not_happy.bad_extension in Yeah
press enter to continue
Setup the configuration file as follow:
- / must answer to GET request ONLY
- /put_test/* must answer to PUT request and save files to a directory of your choice
- any file with .bla as extension must answer to POST request by calling the cgi_test executable
- /post_body must answer anything to POST request with a maxBody of 100
- /directory/ must answer to GET request and the root of it would be the repository YoupiBanane and if no file are requested, it should search for youpi.bad_extension files

press enter to continue
Before starting please verify that the server is launched
press enter to continue

Test GET http://localhost:1334/
content returned: <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Attack on Ports!</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <header>
        <div class="container">
            <h1>Welcome to <span class="highlight">Attack on Ports!</span></h1>
            <p>Your gateway to mastering web servers and port security.</p>
        </div>
    </header>

    <section class="about">
        <div class="container">
            <h2>About the Project</h2>
            <p>This project is a collaborative effort by <span class="contributor">Momari</span> and <span class="contributor">Zaelarb</span> to explore the intricacies of web servers, port management, and cybersecurity. Join us on this journey to fortify your knowledge and skills!</p>
        </div>
    </section>

    <section class="features">
        <div class="container">
            <h2>Project Features</h2>
            <div class="feature-grid">
                <div class="feature-item">
                    <h3>Server Hardening</h3>
                    <p>Discover techniques to make your server impenetrable.</p>
                </div>
                <div class="feature-item">
                    <h3>Server Hardening</h3>
                    <p>Discover techniques to make your server impenetrable.</p>
                </div>
                <div class="feature-item">
                    <h3>Server Hardening</h3>
                    <p>Discover techniques to make your server impenetrable.</p>
                </div>
            </div>
        </div>
    </section>

    <footer>
        <div class="container">
            <p>&copy; 2023 <span class="highlight">Attack on Ports!</span> | Created by Momari & Zaelarb</p>
        </div>
    </footer>
</body>
</html>

Test POST http://localhost:1334/ with a size of 0

Test HEAD http://localhost:1334/
