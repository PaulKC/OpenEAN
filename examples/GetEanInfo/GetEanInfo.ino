#include <OpenEan.h>

void setup() {
  Serial.begin(115200);

  OpenEan connector("400000000");

  OpenEanItem item;
  int error = connector.getProduct(&item,"4060800148834");

  Serial.println("Item:");
  Serial.println(item.name);
  Serial.println(item.detailname);
  Serial.println(item.maincat);
  Serial.println(item.subcat);
}

void loop() {
  // put your main code here, to run repeatedly:

}
