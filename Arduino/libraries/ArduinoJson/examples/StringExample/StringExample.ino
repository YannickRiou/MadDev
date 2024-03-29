// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonBuffer. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.

#include <ArduinoJson.h>

void setup() {
  DynamicJsonDocument doc;

  // You can use a String as your JSON input.
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  // You can use a String to get an element of a JsonObject
  // No duplication is done.
  long time = obj[String("time")];

  // You can use a String to set an element of a JsonObject
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  obj[String("time")] = time;

  // You can get a String from a JsonObject or JsonArray:
  // No duplication is done, at least not in the JsonBuffer.
  String sensor = obj["sensor"];

  // Unfortunately, the following doesn't work (issue #118):
  // sensor = obj["sensor"]; // <-  error "ambiguous overload for 'operator='"
  // As a workaround, you need to replace by:
  sensor = obj["sensor"].as<String>();

  // You can set a String to a JsonObject or JsonArray:
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  obj["sensor"] = sensor;

  // It works with serialized() too:
  obj["sensor"] = serialized(sensor);

  // You can also concatenate strings
  // WARNING: the content of the String will be duplicated in the JsonBuffer.
  obj[String("sen") + "sor"] = String("gp") + "s";

  // You can compare the content of a JsonObject with a String
  if (obj["sensor"] == sensor) {
    // ...
  }

  // Lastly, you can print the resulting JSON to a String
  String output;
  serializeJson(doc, output);
}

void loop() {
  // not used in this example
}

// Visit https://arduinojson.org/v6/example/string/ for more.
