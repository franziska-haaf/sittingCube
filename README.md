successful# Sitting Cube
A prototype in the context of my master thesis "Exploration von interaktiven und unaufdringlichen Interfaces in einer semi-public Meetup Situation" (en. "Exploration of interactive and unobtrusive interfaces in a semi-public meetup situation") WS19/20 got created.
The prototype consists of an cube, made out of plastic and containing LEDs, which serves as a stool. Users can change the color of it using an ipad which, is showing a web interface hosted by an ESP8266.

The thesis examines the use of unobtrusive, interactive interfaces in a semi-public meetup situation. The situation "person does not integrate" was selected on the basis of a room situation analysis. Based on this analysis, different prototypes will be iteratively tested in a real meetup situation.

The file uploaded on the micro controller is https://github.com/binarykittenDE/sittingCube/blob/master/sittingCubeWebinterface/sittingCubeWebinterface.ino

# Hardware
- Microcontroller "Geekcreit NodeMcu Lua ESP8266 ESP-12E WIFI" [Amazon](https://www.amazon.de/gp/product/B018E741G4/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1)
- IR Transmitter and IR Receiver [Amazon](https://www.amazon.de/gp/product/B07BFNGF53/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)
- LED sitting cube 40x40 with build in LEDs [Shop](https://www.pkgreen.de/products/40cm-led-wurfel-sitz-hocker)

# Component communication
![component overview graphic](img/sitting_cube_communication_schaubild-01.png)

# Future iterations
After successful user tests, there could be thought about making a more sophisticated web interface, making it more fun and playful for the users. One problem with the cube currently is, that the options for colors are very limited. 
To change that, it would be necessary to add a micro controller to the cube, to give more freedom changing the colors of the leds. Currently, the codes send to the cube encode the position of the buttons on the remote control supplied, not the colours itself. To find new codes/colors seeems not possible like that.
