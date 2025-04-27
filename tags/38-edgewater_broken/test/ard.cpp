#include <ArduinoJson.h>

char* astr;
void creaJson(char* astr){
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> root;
  root["id"]= 0;
  root["aid"] = 255;
  root["ev"] = 2;
  root["numdata"] = 3;
  serializeJsonPretty(root, Serial);
  // JsonArray& pro = root.createNestedArray("pro");
  // for(int i=0;i<p.ev;i++){
  //   JsonArray& data = pro.createNestedArray();
  //   for (int j=0;j<p.numdata + 2;j++){
  //     data.add(p.prg[i][j]);
  //   }
  // }
  // char ast[200];
  // root.printTo(ast, sizeof(ast));
  // Serial.println(ast);
  // strcpy(astr,ast);
}

int main()   // define the main function  
{  
    printf("Welcome to JavaTpoint \n"); 

}  