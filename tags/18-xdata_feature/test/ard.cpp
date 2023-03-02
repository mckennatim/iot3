#include <ArduinoJson.h>

void creaJson(prg_t& p, char* astr){
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> root;
  root["id"]= p.sr;
  root["aid"] = p.aid;
  root["ev"] = p.ev;
  root["numdata"] = p.numdata;
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