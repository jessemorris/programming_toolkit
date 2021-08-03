#pragma once

#include <opencv2/viz/types.hpp>
#include <string>
#include <unordered_map>
#include <iostream>


namespace ptk {
namespace common {

    //https://github.com/MIT-SPARK/Kimera-Semantics/blob/master/kimera_semantics/include/kimera_semantics/color.h
    class HashableColor : public cv::viz::Color {
        public:
            HashableColor();
            HashableColor(const cv::viz::Color& color);
            HashableColor(uint8_t r_, uint8_t g_, uint8_t b_);

            bool operator==(const HashableColor& other) const;
            bool equal(const HashableColor& color) const;
            bool equal(const cv::viz::Color& color) const;

            inline friend std::ostream &operator << (std::ostream& output, const HashableColor& color);

            inline bool is_valid();

            static void set_random_seed(const int seed);
            static HashableColor get_random_color();


        protected:
            uint8_t r;
            uint8_t g;
            uint8_t b;
            
        private:
            static cv::RNG rng;

    };

    using HashableColors = std::vector<HashableColor>;

    // For unordered map using Color as a Key.
    // struct ColorHasher {
    //     size_t operator()(const HashableColor& k) const;
    // };

    // class ColourManager {

    //     using ColourLabel = std::string;
    //     using ColourId = int;

    //     using LabelToColour = std::unordered_map<ColourLabel, HashableColor>;

    //     public:
    //         ColourManager(const std::string& filename);

    //         HashableColor get_colour_for_class_label(const std::string& label);
    //         HashableColor get_colour_for_tracking_id(const int tracking_id);

    //         inline size_t number_of_classes() {return classes.size();}

    //     private:
    //         LabelToColour label_to_color;

    // };

}


}