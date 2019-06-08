# fanfriend

## What is this?

Okay...

So, I came across an old HP ProLiant DL380 Gen6 server.  Thus far, my homelab has largely consisted of junky consumer-grade cases and regular old off-the-shelf hardware.  It's been fine.

I wanted to use this machine because of its density (2U instead of the 4U my existing server took up, which makes a big difference on a 12U quarter rack).  I also wanted the remote management niceties that iLO offers.  I especially like iLO's SSH interface over Dell DRACs and whatever Supermicro puts on their BMCs.

Being in a home environment, however, the thing is a bit loud.  This is expected, because how much noise a server makes is the absolute _last_ thing you care about in a datacenter.  But at home, having this metal box with fans whining in the ~40 megawatt~ 1200Hz range is a nuisance.

Turns out, the wonderful folks at HP® provide a fantastic suite of low-level tools that allow you to build a totally custom cooling profile for your hardware while still making use of their "Sea of Sensors" that offer accurate, real-time thermal monitoring of all system components.

Sike.  I literally have to hack the fans and fake the signals going to them and back to the motherboard because THIS IS WHAT CONSUMER EMPOWERMENT LOOKS LIKE IN 20xx.

HP maintains an iron grip over even the most basic mechanisms to control fan speed in any way at all, and furthermore, has changed the system firmware in a number of ways over the years that _appear_ to cause the fans to spin up wildly in the presence of _non-HP branded PCI cards_.

Read into that whatever narrative you wish.

So this project is my take on the work of others who have decided to intercept the physical fan wiring on the board for the purpose of feeding it into an [ATMega32u4 microcontroller](https://www.adafruit.com/product/296), and then having that microcontroller modulate the rotational speed of the fans entirely outside of the HP firmware's control.  This isn't _exactly_ what I had in mind for this server, but whatever, it's a good learning opportunity.

Anyway, this project is the source code for what goes on that microcontroller.

### NOTE

This code is specific to this Adafruit ATMega32, but adapting it to other Atmel chips/Arduino boards should be relatively straightforward.

### BONUS LEARNING OPPORTUNITY FOR THE CURIOUS OR STUBBORN

This project is ALSO structured to build and deploy entirely via the command line using the included Makefile.  This is my preferred mode of working because when I build code I want to see the whites of my compiler's eyes.  If you, too, are interested in what happens when you click 'Upload' in the Arduino IDE, then take a gander.
