# ofxRemoteCV

OpenFrameworks Application to capture remote footage from a SRI ip camera and process the image using openCV

## Dependencies:

The application was developed under of_v0.9.3_linux64_release and tested on ubuntu-gnome 15.10

The following oF addons sould be installed:

+ ofxJSON
+ ofxOpenCv
+ ofxSimpleTimer

## Use:

After compilation, go to the bin/data folder and open the config.json file:

		{
			"url" : "YOUR-URL-GOES-HERE",
			"user" : "USERNAME",
			"password" : "PASSWORD"
		}

Replace "url", "user" and "password" to fit the needs of your camera.