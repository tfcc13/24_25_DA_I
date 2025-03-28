//
// Created by micael on 28-03-2025.
//

#ifndef MULTISTREAM_H
#define MULTISTREAM_H

#include <iostream>
#include <fstream>

class MultiStream {
    std::ostream& stream1;
    std::ostream& stream2;

public:
    MultiStream(std::ostream& s1, std::ostream& s2) : stream1(s1), stream2(s2) {}

    template <typename T>
    MultiStream& operator<<(const T& data) {
        stream1 << data;
        stream2 << data;
        return *this;
    }

    MultiStream& operator<<(std::ostream& (*func)(std::ostream&)) {
        stream1 << func;
        stream2 << func;
        return *this;
    }
};



#endif //MULTISTREAM_H
