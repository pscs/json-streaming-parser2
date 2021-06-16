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

/*
  Unified element selector.
  Represents the handle associated to an element within either
  an object (key) or an array (index). 
*/
class ElementSelector {
  friend class ElementPath;
  friend class JsonStreamingParser;

  private: 
    int index;
    char key[20];

  public:
    int getIndex() const;
    
    const char* getKey() const;
    
    bool isObject() const;
    
    /*
      Builds the string representation of this node position within 
      its parent.
    */
    int toString(char* buffer, int maxLength) const;

  private:    
    void reset();
    
    void set(int index);
    
    void set(char* key);
    
    /*
      Advances to next index.
    */
    void step();
};

/*
  Hierarchical path to currently parsed element.
  It eases element filtering, keeping track of the current node
  position.  
*/
class ElementPath {
  friend class JsonStreamingParser;

  private:
    int count = 0;
    ElementSelector* current;
    ElementSelector selectors[20];

  public:
    /*
      Gets the element selector at the given level.
    */
    const ElementSelector* get(int index) const;
    
    int getCount() const {
      return count;
    }
    
    /*
      Gets current element selector.
    */
    const ElementSelector* getCurrent() const {
      return current;
    }

    ElementSelector* getCurrent() {
      return current;
    } 
    /*
      Gets current element's index (in case of array).
    */
    int getIndex() const;

    int getIndex(int index) const;

    /*
      Gets current element's key (in case of object).
    */
    const char* getKey() const;

    const char* getKey(int index) const;

    /*
      Gets parent element selector.
    */
    const ElementSelector* getParent() const;
    
    /*
      Builds the full path corresponding to the current node position.

      For example, "weather[0].id" corresponds to a 3-level hierarchy:
      {
        "weather" : [
          {
            "id" : ..., <===== HERE IT IS
            ... : ...
          },
          { ... }
        ],
        ...
      }
    */    
    int toString(char* buffer, int maxLength) const;

  private:
    static int getIndex(const ElementSelector* selector);
    
    static const char* getKey(const ElementSelector* selector);

    void pop();
    
    void push();
};
