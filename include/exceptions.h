#ifndef HW_02_HUFFMAN_EXCEPTIONS_H
#define HW_02_HUFFMAN_EXCEPTIONS_H

#include <string>
#include <iostream>

class ExceptionOutput {
protected:
    const char* msg;
public:
    ExceptionOutput(const char* message) {
        msg = message;
    }
    virtual void out() {
        std::cerr << msg << "\n";
    }
};

class FOpenException: public ExceptionOutput {
private:
    const char* f;
public:
    FOpenException(const char* file_name) : ExceptionOutput("Cannot open file: ") {
        f = file_name;
    }
    void out() {
        std::cerr << msg << f << "\n";
    }
};

class FIOException: public ExceptionOutput {
private:
    const char* act;
public:
    FIOException(const char* action) : ExceptionOutput("Error: ") {
        act = action;
    }
    void out() {
        std::cerr << act << msg << "\n";
    }
};

class MemException: public ExceptionOutput {
private:
    const char* obj;
public:
    MemException(const char* object) : ExceptionOutput("Cannot create an object: ") {
        obj = object;
    }
    void out() {
        std::cerr << msg << obj << "\n";
    }
};

class ArgNumException: public ExceptionOutput {
private:
    int expctd;
    int Got;
public:
    ArgNumException(int expected, int got) : ExceptionOutput("Invalid number of arguments: ") {
        expctd = expected;
        Got = got;
    }
    void out() {
        std::cerr << msg << "expected " << expctd << ", got " << Got << "\n";
    }
};

#endif //HW_02_HUFFMAN_EXCEPTIONS_H