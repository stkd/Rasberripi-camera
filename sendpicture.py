import requests
import os
import re
import time

#return1=os.system('ping 8.8.8.8 -c 2')
#while True:
#    if return1:
#        os.system('echo "1" > /tmp/noip.txt')
#        time.sleep(5)
#    else:
#        os.system('echo "0" > /tmp/noip.txt')
#        break;
time.sleep(60)    
os.system('cd /home/pi')
os.system('echo "cd folder" > /tmp/status.txt')
os.system('DATE=$(date +"%Y-%m-%d_%H%M") ; /usr/bin/raspistill -vf -hf -q 90 -o /tmp/$DATE.jpg')
os.system('echo "camera.sh script done" > /tmp/status.txt')
url = 'https://bigdatalab.niu.edu.tw/picreceive.php'

for dirPath, dirNames,fileNames in os.walk("/tmp"):
    pat=".jpg"
    for f in fileNames:
        pattern=re.search(pat, os.path.join(dirPath, f))
        if pattern!= None:
            picture_path = os.path.join(dirPath, f)  # need change to the picture path
            break

picture_file = open(picture_path, 'rb')
files = {'test': picture_file}

response = requests.post(url, files=files)
picture_file.close()
print(response.__dict__)
if response.status_code == 200:
    print("Sended")
else:
    print("Send Failed")