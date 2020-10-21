-- set up some DHT22 things from https://github.com/javieryanez/nodemcu-modules/tree/master/dht22
PIN = 4 --  data pin, GPIO2
dht22 = require("dht22_min")
dht22.read(PIN)
t = dht22.getTemperature()
h = dht22.getHumidity()