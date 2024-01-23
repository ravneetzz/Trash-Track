# Trash-Track: Real-Time Waste Management System

TrashTrack is an advanced open-source project designed to revolutionize waste management by implementing real-time monitoring of waste bin levels, providing accurate data on fill status, and optimizing collection schedules to minimize environmental impact. This GitHub repository serves as the collaborative platform for developers, environmentalists, and communities to contribute to the evolution of smart waste management solutions.

## Key Features:

### Real-Time Monitoring:
• TrashTrack employs cutting-edge technologies to enable real-time monitoring of waste bin levels, ensuring up-to-the-minute data on fill status.
• This feature facilitates proactive waste management by allowing authorities to allocate resources efficiently based on current demand.

### Alerts and Notifications:
• Integrated alerting mechanisms provide timely notifications to waste management teams and relevant stakeholders.
• Prevent overflows and optimize collection schedules, reducing the risk of environmental hazards and improving the overall efficiency of waste collection processes.

### User-Friendly Interface:
• TrashTrack boasts a user-friendly interface that makes it easy for users to view and contribute to waste tracking data.
• Citizens can actively participate by reporting bin status or suggesting optimal collection times, fostering a sense of community engagement in waste management efforts.

### Environmental Impact:
• The project emphasizes the crucial role of timely waste disposal in minimizing adverse environmental impact.
• By preventing overflows and optimizing collection schedules, TrashTrack contributes to a cleaner, healthier environment while promoting sustainable waste management practices.

## • Softwares needed:
    Arduino IDE: For code logic
    MQTT (Broker):  For communication between devices
    MQTTAnalyzer (App): For receiving notification on mobile
## • Hardware:
    Esp 32: For real-time sensing
    Ultrasonic sensor: For measuring filled level
## • Others:
    Wi-fi: For communication
    Dustbin: For attaching sensor

## Steps to run this project in your machine:

1) Install the arduino IDE, MQTTX, mosquitto [https://mosquitto.org/download/](url) and MQTTAnalyzer (on your mobile).
2) Add mymosquitto.conf file attached in the following location (C:\Program Files\mosquitto) 
3) Open MQTTX and create the connection as Trash_Track with the following topics ultrasonic/notification , ultrasonic/distance. Add the port as 1883 and your username, password, host which needs to be updated in the arduino code as well.
4) Open Windows PowerShell and execute the following command:  C:\Program Files\mosquitto> .\mosquitto.exe -c .\mymosquitto.conf -v
5) Make changes in the arduino code (trash_track.ino file). Add your wifi name, password, ip address, mqtt username and mqtt password.
6) Make connection of ultrasonic sensor with esp32 as shown in attached image.
7) Attach the ultrasonic sensor to dustbin covering.
8) Connect esp32 with laptop and upload the code written in trash_track.ino file.
9) Connect the phone with the same wifi where MQTTAnalyzer app is installed. You will receive notification in this app under ultrasonic/notification topic when dustbin is filled more than 75%.
