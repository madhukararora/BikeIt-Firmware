Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**Note: All average currents should be taken at a time scale of 200mS/div.**

**1. Fill in the below chart based on currents measured in each energy mode, replacing each TBD with measured values.  Use the [Selecting Ranges](https://www.silabs.com/documents/public/user-guides/ug343-multinode-energy-profiler.pdf) feature of the profiler to select the appropriate measurement range.  Your measurements should be accurate to 10%**

Energy Mode | Period average current (uA) | Average Current with LED off (uA) | Average Current with LED On (uA)
------------| ----------------------------|-----------------------------------|-------------------------
EM0         |            4.21 mA          |           4.17 mA                 |           4.67 mA
EM1         |            3.12 mA          |           3.09 mA                 |           3.58 mA
EM2         |           44.37 uA          |           4.18 uA                 |         508.13 uA
EM3         |           44.60 uA          |           5.40 uA                 |         499.97 uA

**2. Fill in the below chart based on time measured period and LED on time measurements for EM2 and EM3 modes, replacing each TBD with measured values.  Use the [Selecting Ranges](https://www.silabs.com/documents/public/user-guides/ug343-multinode-energy-profiler.pdf) feature of the profiler to select the appropriate measurement range.  When measuring time, measure from peak current to peak current.  Your LED on time measurement should be accurate to 1ms and your period measurement should be accurate to within 10ms**

Energy Mode | Period (ms)| LED On Time (ms) |
------------| -----------|-------------------
EM2         |   2250 ms  |        175 ms
EM3         |   2230 ms  |        176 ms


**3. ScreenShots**  

***EM0***  
Period average current    
![em0_avg_current_period][em0_avg_current_period]  
Average Current with LED ***off***  
![em0_avg_current_ledoff][em0_avg_current_ledoff]  
Average Current with LED ***on***  
![em0_avg_current_ledon][em0_avg_current_ledon]  

***EM1***  
Period average current    
![em1_avg_current_period][em1_avg_current_period]  
Average Current with LED ***off***  
![em1_avg_current_ledoff][em1_avg_current_ledoff]  
Average Current with LED ***on***  
![em1_avg_current_ledon][em1_avg_current_ledon]  

***EM2***  
Period average current  
![em2_avg_current_period][em2_avg_current_period]  
Average Current with LED ***off***  
![em2_avg_current_ledoff][em2_avg_current_ledoff]  
Average Current with LED ***on***  
![em2_avg_current_ledon][em2_avg_current_ledon]   
LED measurement - Period   
![em2_led_period][em2_led_period]  
LED measurement - LED on time   
![em2_led_ledOnTime][em2_led_ledOnTime]  

***EM3***  
Period average current    
![em3_avg_current_period][em3_avg_current_period]  
Average Current with LED ***off***  
![em3_avg_current_period][em3_avg_current_ledoff]   
Average Current with LED ***on***  
![em3_avg_current_period][em3_avg_current_ledon]   
LED measurement - Period   
![em3_led_period][em3_led_period]  
LED measurement - LED on time   
![em3_led_ledOnTime][em3_led_ledOnTime]  

[em0_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em0_avg_current_period.PNG "em0_avg_current_period"
[em0_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em0_avg_current_ledoff.PNG "em0_avg_current_ledoff"
[em0_avg_current_ledon]:  https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em0_avg_current_ledon.PNG   "em0_avg_current_ledon"

[em1_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em1_avg_current_period.PNG "em1_avg_current_period"
[em1_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em1_avg_current_ledoff.PNG "em1_avg_current_ledoff"
[em1_avg_current_ledon]:  https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em1_avg_current_ledon.PNG  "em1_avg_current_ledon"

[em2_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em2_avg_current_period.PNG "em2_avg_current_period"
[em2_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em2_avg_current_ledoff.PNG "em2_avg_current_ledoff"
[em2_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em2_avg_current_ledon.PNG   "em2_avg_current_ledon"
[em2_led_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em2_led_period.jpg                 "em2_led_period"
[em2_led_ledOnTime]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em2_led_ledOnTime.jpg           "em2_led_ledOnTime"

[em3_avg_current_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em3_avg_current_period.PNG "em3_avg_current_period"
[em3_avg_current_ledoff]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em3_avg_current_ledoff.PNG "em3_avg_current_ledoff"
[em3_avg_current_ledon]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em3_avg_current_ledon.PNG   "em3_avg_current_ledon"
[em3_led_period]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em3_led_period.jpg "em3_led_period"
[em3_led_ledOnTime]: https://github.com/CU-ECEN-5823/ecen5823-assignment2-madhukararora/blob/master/questions/Assignment%202-%20screenshots-Madhukar/em3_led_ledOnTime.jpg "em3_led_ledOnTime"
