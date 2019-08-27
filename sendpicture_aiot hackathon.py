
# coding: utf-8

# In[1]:


import requests
import os
import re
import json
import time
import configparser 

config = configparser.ConfigParser()
config.read('E:/jupeter notebook/cht.conf')
projectKey = config.get('device-key', 'projectKey')
deviceId   = config.get('device-key', 'deviceId')
cameraId   = config.get('device-key', 'cameraId')

apiURL = 'http://iot.cht.com.tw/iot/v1/device/' + deviceId + '/snapshot'

headers = { 
    "CK":projectKey,
    "accept":"application/json",
}

time.sleep(30)    
os.system('cd /home/pi')
os.system('echo "cd folder" > /tmp/status.txt')
os.system('DATE=$(date +"%Y-%m-%d_%H%M") ; /usr/bin/raspistill -vf -hf -q 30 -co 10 -br 60 -o /tmp/$DATE.jpg')
os.system('echo "camera.sh script done" > /tmp/status.txt')

for dirPath, dirNames,fileNames in os.walk("/tmp"):
    pat=".jpg"
    for f in fileNames:
        pattern=re.search(pat, os.path.join(dirPath, f))
        if pattern!= None:
            picture_path = os.path.join(dirPath, f)  # need change to the picture path
            break

files = {"img": ("test", open(picture_path, "rb"), "image/jpeg"), "meta":(None, json.dumps({"id":cameraId,"value":["Raspberry pi camera"]}), 'application/json')}

response = requests.post(apiURL, files=files, headers=headers)
print(response.text)


# In[ ]:




