#ifndef PTK_COMMON_TIME
#define PTK_COMMON_TIME

#include <iostream>
#include <chrono>

namespace ptk {


    /**
     * @brief Represents a time object with seconds and nano seconds. Makes it easy to
     * convert between this and other representations (eg ROS)
     * 
     */
    class Time {

        public:
            Time() {
                // std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
                // auto duration = now.time_since_epoch();
                
            }

            Time(uint32_t sec_, uint32_t nsec_)
                :   sec(sec_), nsec(nsec_) {}

            Time(const Time& t)
                :   sec(t.sec), nsec(t.nsec) {}


            /**
             * @brief Time in seconds
             * 
             * @return double 
             */
            inline double to_sec() {
                return static_cast<double>(sec) + static_cast<double>(nsec) / 1e9;
            };
             
            /**
             * @brief Time in nano-seconds
             * 
             * @return double 
             */
            inline double to_nsec() {
                return static_cast<double>(sec) * 1e9 + static_cast<double>(nsec);
            };

            /**
             * @brief TODO: check that this actually works...
             * 
             * @param t1 
             * @param t2 
             * @return Time 
             */
            inline static Time difference(const Time& t1, const Time& t2) {
                return Time(t1.getSecs() - t2.getSecs(), t1.getNSecs() - t2.getNSecs());
            }

            template<class T>
            inline static Time create(T& t);

            template<class T>
            inline T convert();


            friend std::ostream &operator << (std::ostream& output, const Time& object) {
                output << "sec: " << object.sec << " nsec: " << object.nsec << std::endl;
                return output;
            }

            inline const uint32_t getSecs() const {return sec;}
            inline const uint32_t getNSecs() const {return nsec;}

        private:
            uint32_t sec, nsec;

    };


    


} //ptk

#endif
