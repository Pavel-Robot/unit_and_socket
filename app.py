# /opt/webapp/app.py
import os
import logging
import requests
from datetime import datetime

url = 'http://127.0.0.1:8888/status'

file_name = 'status.log' 
dir = "/opt/webapp"
path_log = dir + '/' + file_name

logging.basicConfig(level=logging.INFO, format='%(levelname)s:%(message)s', filename=path_log, filemode="w")
print(path_log)

try:
    response = requests.get(url) 
    if response.status_code == 200: # Если код 200 это ОК
        result = response.text
        if result == 'Status is running\n':
            logging.info('SUCCESS: ' + str(datetime.now()) + ' Описание: получен ответ ОК от сервера и полезная информация получена.' + ' result:' + result)
        else:
            logging.error(str(datetime.now() + ' Описание: получен ответ от ОК сервера, но информации полезной неполучено' + ' result:' + result))
    else:

        logging.error(str(datetime.now()) + ' Описание: получен код не 200 ОК,а код: ' + str(response.status_code))
except requests.exceptions.RequestException as e:
    logging.error(str(datetime.now()) + ' Описание: не удалось выполнить запрос на сервер: ' + url)

print("Работа скрипта app.py завершена")
