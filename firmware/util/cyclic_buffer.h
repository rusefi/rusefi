/**
 * @file	cyclic_buffer.h
 *
 * @date Dec 8, 2013
 * @author Andrey Belomutskiy, Daniel Hill
 *
 * Daniel Hill - Modified to use C++ - Mar 2, 2014
*/

#ifndef CYCLIC_BUFFER_H
#define CYCLIC_BUFFER_H

static const short CB_MAX_SIZE = 16;

class cyclic_buffer
{
  public:
  //ctor
    cyclic_buffer();
  //cpctor
    cyclic_buffer(const cyclic_buffer& cb);
  //dtor
    ~cyclic_buffer();

  public:
  //overloaded =operator
    cyclic_buffer& operator=(const cyclic_buffer& rhCb);

  public:
    void add(int value);
    int sum(int length);


  private:
    volatile int elements[CB_MAX_SIZE];
    volatile int currentIndex;
    volatile int count;
};

#endif //CYCLIC_BUFFER_H
