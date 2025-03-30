#ifndef MULTISTREAM_H
#define MULTISTREAM_H

#include <iostream>
#include <fstream>

/**
 * @brief A utility class that allows output to be written to two streams simultaneously.
 */
class MultiStream {
    std::ostream& stream1; ///< First output stream.
    std::ostream& stream2; ///< Second output stream.

public:

    /**
    * @brief Constructor of MultiStream object that writes to two streams.
    * @param s1 The first output stream.
    * @param s2 The second output stream.
    */
    MultiStream(std::ostream& s1, std::ostream& s2) : stream1(s1), stream2(s2) {}

    /**
    * @brief Overloaded stream insertion operator for writing data to both streams.
    * @tparam T The type of data to write.
    * @param data The data to be written to both streams.
    * @return Reference to the current MultiStream object.
    */
    template <typename T>
    MultiStream& operator<<(const T& data) {
        stream1 << data;
        stream2 << data;
        return *this;
    }

    /**
    * @brief Overloaded stream insertion operator for handling manipulators (e.g., std::endl).
    * @param func A function pointer to a manipulator like std::endl.
    * @return Reference to the current MultiStream object.
    */
    MultiStream& operator<<(std::ostream& (*func)(std::ostream&)) {
        stream1 << func;
        stream2 << func;
        return *this;
    }
};



#endif //MULTISTREAM_H
