/**The MIT License (MIT)

Contributors:
    Stefano Chizzolini

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include "ElementPath.h"

int ElementSelector::getIndex() const {
  return index;
}
    
const char* ElementSelector::getKey() const {
  return key;
}

bool ElementSelector::isObject() const {
  return index < 0;
}

void ElementSelector::reset() {
  index = -1;
  key[0] = '\0';
}

void ElementSelector::set(int index) {
  this->index = index;
  this->key[0] = '\0';
}

void ElementSelector::set(char* key) {
  strcpy(this->key, key);
  this->index = -1;
}

void ElementSelector::step() {
  index++;
}

int ElementSelector::toString(char* buffer, int maxLength) const {
  int len = strlen(key);
  if (index >= 0) {
    len += 3;
    if (len >= maxLength - 1) {
      return -1;
    }
    snprintf(buffer, maxLength, "%s[%d]", buffer, index);
    return strlen(buffer);
  } else {
    if (len >= maxLength - 1) {
      return -1;
    }
    strcpy(buffer, key);
    return len;
  }
}

const ElementSelector* ElementPath::get(int index) const {
  if (index >= count
      || (index < 0 && (index += count - 1) < 0))
    return NULL;

  return &selectors[index];
}

int ElementPath::getIndex() const {
  return getIndex(current);
}

int ElementPath::getIndex(int index) const {
  return getIndex(get(index));
}

int ElementPath::getIndex(const ElementSelector* selector) {
  return selector != NULL ? selector->index : -1;
}

const char* ElementPath::getKey() const {
  return current != NULL ? current->key : "\0";
}

const char* ElementPath::getKey(int index) const {
  return getKey(get(index));
}

const char* ElementPath::getKey(const ElementSelector* selector) {
  return selector != NULL ? selector->key : "\0";
}

const ElementSelector* ElementPath::getParent() const {
  return get(-1);
}

void ElementPath::pop() {
  if(count > 0) {
    current = --count > 0 ? &selectors[count - 1] : NULL;
  }
}

void ElementPath::push() {
  (current = &selectors[count++])->reset();
}

int ElementPath::toString(char* buffer, int maxLength) const {
  int pos = 0;
  buffer[0] = 0;
  if (count <= 0)
    return 0;

  for(int index = 0; index < count; index++) {
    if(index > 0 && selectors[index].isObject()) {
      if (pos >= maxLength - 1) {
        return -1;
      }
      strcat(buffer, ".");
      pos++;
    }
    printf("EP toStr %d %d\n", pos, maxLength - pos);
    int len = selectors[index].toString(buffer + pos, maxLength - pos);
    if (len == -1) {
      return -1;
    } else {
      pos += len;
    }
  }
  return pos;
}
