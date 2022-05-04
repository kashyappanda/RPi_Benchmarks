import json

with open('qmkl_500_watts.json') as json_file:
    data = json.load(json_file)

for index in data:
    print(data[index]['watts'])