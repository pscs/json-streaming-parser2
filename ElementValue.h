#include <Arduino.h>

union Variant {
  bool boolValue;
  float numValue;
  const char* stringValue;
};

struct ElementValue {
  private:
    static const int Type_Null = 0;
    static const int Type_Int = 1;
    static const int Type_Float = 2;
    static const int Type_String = 3;
    static const int Type_Bool = 4;

    Variant data;
    int type;

  public:
    ElementValue with(float value) {
      data.numValue = value;
      type = Type_Float;
      return *this;
    }

    ElementValue with(long value) {
      data.numValue = value;
      type = Type_Int;
      return *this;
    }
    
    ElementValue with(bool value) {
      data.boolValue = value;
      type = Type_Bool;
      return *this;
    }

    ElementValue with(const char* value) {
      data.stringValue = value;
      type = Type_String;
      return *this;
    }

    ElementValue with() {
      type = Type_Null;
      return *this;
    }
    
    bool getBool() const {
      return data.boolValue;
    }

    const char* getString() const {
      return data.stringValue;
    }

    float getFloat() const {
      return data.numValue;
    }

    long getInt() const {
      return (long)data.numValue;
    }

    bool isInt() const {
      return type == Type_Int;
    }

    bool isFloat() const {
      return type == Type_Float;
    }

    bool isString() const {
      return type == Type_String;
    }

    bool isBool() const {
      return type == Type_Bool;
    }

    bool isNull() const {
      return type == Type_Null;
    }
    
    char* toString(char* buffer, int maxLength) {
      if(isInt()) {
        snprintf(buffer, maxLength, "%ld", getInt()); // long int
      } else if(isFloat()) {
        snprintf(buffer, maxLength, "%f", getFloat());
      } else if(isString()) {
        snprintf(buffer, maxLength, "\"%s\"", getString()); // Note: This adds "'s (quote symbols) either side of the actual string value.
      } else if(isBool()) {
        strcpy(buffer, getBool() ? "true" : "false");
      } else if(isNull()) {
        strcpy(buffer, "null");
      } else {
        strcpy(buffer, "?");
      }
      return buffer;
    }
};