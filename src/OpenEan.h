/*
  OpenEan.h - Library to fetch EAN information from http://opengtindb.org.
*/
#ifndef OpenEan_h
#define OpenEan_h

#include "Arduino.h"

class OpenEanItem
{
public:
  char name[30];
  char detailname[500];
  char vendor[200];
  char maincat[100];
  char subcat[100];
  char descr[1000];
  char origin[100];
  char validated[10];

  boolean lactosefree;
  boolean coffeefree;
  boolean dietary;
  boolean glutenfree;
  boolean fructosefree;
  boolean organic;
  boolean fairtrade;
  boolean vegatarian;
  boolean vegan;
  boolean microplastics;
  boolean mineraloil;
  boolean nicotin;

  boolean p_platic;
  boolean p_composite;
  boolean p_paper;
  boolean p_glass;
  boolean p_metal;
  boolean p_reusable;
  boolean p_plasticfree;
  boolean unpacked;
  boolean overlypacked;
  boolean reasonablypacked;
};
class OpenEan
{
  public:
    OpenEan(const char* userId);
    int getProduct(OpenEanItem* resultItem,const char* ean);
  private:
    const char* _userId;
};

#endif
