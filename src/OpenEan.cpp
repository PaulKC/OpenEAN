/*
  OpenEan.cpp - Library to fetch EAN information from https://opengtindb.org.
*/
#include "Arduino.h"
#include "OpenEan.h"
#include <ESP8266HTTPClient.h>

OpenEan::OpenEan(int userId)
{
  _userId = userId;
}

/*
  Searches for the ean in https://opengtindb.org.
  Returns 0 if it was successfull otherwise the following errors are returned:
  0 - OK - Operation war erfolgreich
  1 - not found - die EAN konnte nicht gefunden werden
  2 - checksum - die EAN war fehlerhaft (Checksummenfehler)
  3 - EAN-format - die EAN war fehlerhaft (ungültiges Format / fehlerhafte Ziffernanzahl)
  4 - not a global, unique EAN - es wurde eine für interne Anwendungen reservierte EAN eingegeben (In-Store, Coupon etc.)
  5 - access limit exceeded - Zugriffslimit auf die Datenbank wurde überschritten
  6 - no product name - es wurde kein Produktname angegeben
  7 - product name too long - der Produktname ist zu lang (max. 20 Zeichen)
  8 - no or wrong main category id - die Nummer für die Hauptkategorie fehlt oder liegt außerhalb des erlaubten Bereiches
  9 - no or wrong sub category id - die Nummer für die zugehörige Unterkategorie fehlt oder liegt außerhalb des erlaubten Bereiches
  10 - illegal data in vendor field - unerlaubte Daten im Herstellerfeld
  11 - illegal data in description field - unerlaubte Daten im Beschreibungsfeld
  12 - data already submitted - Daten wurden bereits übertragen
  13 - queryid missing or wrong - die UserID/queryid fehlt in der Abfrage oder ist für diese Funktion nicht freigeschaltet
  14 - unknown command - es wurde mit dem Parameter "cmd" ein unbekanntes Kommando übergeben
  99 - error connecting to https://opengtindb.org
*/
int OpenEan::getProduct(OpenEanItem *resultItem, char ean[])
{
  HTTPClient https;
  char requestUrl[100];
  sprintf(requestUrl, "http://opengtindb.org/?ean=%s&cmd=query&queryid=%i", ean, _userId);
  if (https.begin(requestUrl))
  { // HTTPS
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String payload = https.getString();
        int errorCode = payload[payload.indexOf("error=") + 6] - '0';
        if (errorCode != 0)
        {
          https.end();
          return errorCode;
        }
        else
        {
          String message = payload.substring(payload.indexOf("---") + 3);
          String firstItem = message.substring(0, message.indexOf("---"));
          char *ptr = NULL;
          char *itemBody = const_cast<char *>(firstItem.c_str());
          ptr = strtok(itemBody, "\n");
          while (ptr != NULL)
          {
            if ((strncmp("name=", ptr, 5) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->name, value, strlen(value) + 1);
            }
            else if ((strncmp("detailname=", ptr, 11) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->detailname, value, strlen(value) + 1);
            }
            else if ((strncmp("vendor=", ptr, 7) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->vendor, value, strlen(value) + 1);
            }
            else if ((strncmp("maincat=", ptr, 8) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->maincat, value, strlen(value) + 1);
            }
            else if ((strncmp("subcat=", ptr, 7) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->subcat, value, strlen(value) + 1);
            }
            else if ((strncmp("descr=", ptr, 6) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->descr, value, strlen(value) + 1);
            }
            else if ((strncmp("origin=", ptr, 7) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              strncpy(resultItem->origin, value, strlen(value) + 1);
            }
            else if ((strncmp("contents=", ptr, 9) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              int intValue = atoi(value);
              resultItem->lactosefree = intValue & 1;
              resultItem->coffeefree = intValue & 2;
              resultItem->dietary = intValue & 4;
              resultItem->glutenfree = intValue & 8;
              resultItem->fructosefree = intValue & 16;
              resultItem->organic = intValue & 32;
              resultItem->fairtrade = intValue & 64;
              resultItem->vegatarian = intValue & 128;
              resultItem->vegan = intValue & 256;
              resultItem->microplastics = intValue & 512;
              resultItem->mineraloil = intValue & 1024;
              resultItem->nicotin = intValue & 2048;
            }
            else if ((strncmp("pack=", ptr, 9) == 0))
            {
              char *value = strstr(ptr, "=") + 1;
              int intValue = atoi(value);
              resultItem->p_platic = intValue & 1;
              resultItem->p_composite = intValue & 2;
              resultItem->p_paper = intValue & 4;
              resultItem->p_glass = intValue & 8;
              resultItem->p_metal = intValue & 16;
              resultItem->unpacked = intValue & 32;
              resultItem->p_plasticfree = intValue & 64;
              resultItem->overlypacked = intValue & 128;
              resultItem->reasonablypacked = intValue & 256;
              resultItem->p_reusable = intValue & 512;
            }
            ptr = strtok(NULL, "\n");
          }
        }
      }
    }
    else
    {
      https.end();
      return 99;
    }
    https.end();
  }
  else
  {
    return 99;
  }
  return 0;
}
