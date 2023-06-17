# K.U.T.F.E.U.T.E.N.
Kies Uw Terreur Forceer Een Ultiem Trauma En Narigheid, a.k.a. K.U.T.F.E.U.T.E.N. contains the development base for a rigged wheel of fortune meant for punishing prospective board members by forcing them to do tasks they don't like.

Help, it's not working, I can't find the hotspot!

--Yeah, this happens, plug in the power connector and make sure LED's start lightning up if you plug it in. It should take about 10 seconds for the hotspot to appear if LED's are burning. If nothing is still happening try resetting (by pressing the button on the wemos) a couple times and turning it off and on again.

Still no good?

--Don't despair, there's still one thing you can try:  First move the micro servo so that the arm hits the wheel, a servo signal begins only after the hotspot has been created. It moving means a hotspot has been created. Then spin the wheel whilst resetting. Somehow this almost always fixes the problem.

Still no good?

--Plug in usb and try again. If a hotspot now appears it is a power issue. Establish that the voltage is around 5 volts without usb, you'll have to remove the board.

It also doesn't work with USB? 

--Re-upload this code, it might not run this program at all. 

Still no good? 

--Get a new Wemos D1 Mini (V3 pinout) and re upload this code.
