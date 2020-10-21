// Lumix G9 Access point credentials
const char* ssid = "MADG9";
const char* password =  "";

HTTPClient http;
String httpReq = "";
int httpCode = 404;

void wifiInit()
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
 
  Serial.println("Connected to the WiFi network MADG9");  
}

int wifiConnectToCamera()
{
    httpReq = ("http://192.168.54.1/cam.cgi?mode=accctrl&type=req_acc&value=4D454930");
    http.begin(httpReq); //Specify the URL
    httpCode = http.GET();                                     

    http.end(); //Free the resources
    
    //Check for the returning code
    if (httpCode == 200) 
    { 
       return 0;
    }
    else
    {
      return httpCode;
    }
}

int pingCamera()
{
    httpReq = ("http://192.168.54.1/cam.cgi?mode=getstate");
    http.begin(httpReq); //Specify the URL
    httpCode = http.GET();                                     

    http.end(); //Free the resources
    
    //Check for the returning code
    if (httpCode == 200) 
    { 
       return 0;
    }
    else
    {
      return httpCode;
    }
}

int takeSingleShot()
{
    httpReq = ("http://192.168.54.1/cam.cgi?mode=camcmd&value=capture");
    http.begin(httpReq); //Specify the URL
    httpCode = http.GET();                                     

    http.end(); //Free the resources
    
    //Check for the returning code
    if (httpCode == 200) 
    { 
       return 0;
    }
    else
    {
      return httpCode;
    }
}


void takeLongExposureShot(int sec)
{

}
