import json

with open('frequencies/pi3/openblas/openblas_1400_watts.json') as json_file:
    data = json.load(json_file)

for index in data:
    print(data[index]['watts'])