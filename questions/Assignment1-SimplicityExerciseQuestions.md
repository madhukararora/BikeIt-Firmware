Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**Q1: How much current does the system draw (instantaneous measurement) when a single LED is on with the GPIO pin set to StrongAlternateStrong?**

4.96 mA at 3.32 V
![system current with single LED with GPIO Pin set to StrongAlternateStrong](https://github.com/CU-ECEN-5823/ecen5823-assignments-madhukararora/blob/master/questions/Assignment%201-%20screenshots-Madhukar/a1%20led1%20strong%20strong%20instant%20current.PNG)



**Q2: How much current does the system draw (instantaneous measurement) when a single LED is on with the GPIO pin set to WeakAlternateWeak?**
     
5.22 mA at 3.32 V    
![system current with single LED with GPIO Pin set to WeakAlternateWeak](https://github.com/CU-ECEN-5823/ecen5823-assignments-madhukararora/blob/master/questions/Assignment%201-%20screenshots-Madhukar/a1%20led1%20weak%20weak%20instant%20current.PNG)     

**Q3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, 
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

There is not much difference between the above two values of current consumption of the system. The Advanced Energy Monitor is capable of detecting changes in current consumption as small as 100 nA. To notice any significant change in current consumption of the device, a larger load than an LED might be required. 

**Q4: With the WeakAlternateWeak drive strength setting, what is the average current for 1 complete on-off cycle for 1 LED with an on-off duty cycle of 50% (approximately 1 sec on, 1 sec off)?**

4.97 mA.
![Average current for one complete cycle with WeakAlternateWeak for a single LED](https://github.com/CU-ECEN-5823/ecen5823-assignments-madhukararora/blob/master/questions/Assignment%201-%20screenshots-Madhukar/weak%20drive%20led%201%20avg%20current.PNG)

**Q5: With the WeakAlternateWeak drive strength setting, what is the average current for 1 complete on-off cycle for 2 LEDs (both on at the time same and both off at the same time) with an on-off duty cycle of 50% (approximately 1 sec on, 1 sec off)?**

5.15 mA. 
![Average current for one complete cycle with WeakAlternateWeak for both the LEDs](https://github.com/CU-ECEN-5823/ecen5823-assignments-madhukararora/blob/master/questions/Assignment%201-%20screenshots-Madhukar/weak%20drive%20led%201%20led%200%20avg%20current.PNG)
