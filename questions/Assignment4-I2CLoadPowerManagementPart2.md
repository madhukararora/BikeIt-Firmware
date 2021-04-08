Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements in the "Default" configuration of the profiler to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See [Shared document](https://docs.google.com/document/d/1Ro9G2Nsr_ZXDhBYJ6YyF9CPivb--6UjhHRmVhDGySag/edit?usp=sharing) for instructions.* 

1. What is the average current per period?
   
Answer: 13.77 uA.
   <br>Screenshot:  
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/ecen5823-assignment4-madhukararora/blob/master/questions/Assignment%204-screenshots-Madhukar/AVERAGE.PNG)  

2. What is the average current when the Si7021 is Load Power Management OFF?
   
Answer: 2.80 uA.[NOTE : This value is when the logging is disabled]
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-madhukararora/blob/master/questions/Assignment%204-screenshots-Madhukar/LPM_OFF%20%20Logging%20Off.PNG)  

3. What is the average current when the Si7021 is Load Power Management ON?

Answer: 343.14 uA.
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-madhukararora/blob/master/questions/Assignment%204-screenshots-Madhukar/LPM_ON.PNG)  

4. How long is the Si7021 Load Power Management ON for 1 temperature reading?
   
Answer:
   <br>Screenshot:  97.20 ms
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/ecen5823-assignment4-madhukararora/blob/master/questions/Assignment%204-screenshots-Madhukar/LPM_ON.PNG)  

5. What is the total operating time of your design for assignment 4 in hours assuming a 1000mAh supply?
   
Answer:
	ampere hours = averge current * time
	
	time = 1000mAh/13.77 uA = 72,621.6 hours of usage.

6. How has the power consumption performance of your design changed since the previous assignment?
   
Answer:
	Power consumption has reduced as compared to the previous assignment.The average current consumption for the whole period previously was 140.61 uA compared to 13.77 uA in this assignment.

7. Describe how you have tested your code to ensure you are sleeping in EM1 mode during I2C transfers.
   
Answer:
	The code includes Sleep Block Begin and Sleep Block End functions at the beginning and end of the I2C transfers. 
	This ensures that the system is sleeping in mode EM1. Also, the current observed in the Energy Profiler shows 
	values expected of the system in Mode EM1. 