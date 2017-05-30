#include <unordered_map>

#define DIAMETER 85
#define HEIGHT 20.
// Note! Below in centimeters!!!
const float CRT_PMT[] = { 0.248695, 0.314308, 0.435673 };
const float CRT_SI[] = { 0.178523, 0.235196, 0.365183 };
#define FWHM_WLS .5

//! All geometries written in a nested map
//! TODO: DRY for lengths???
typedef std::unordered_map <
            std::string,
            std::unordered_map <
                float,
                std::unordered_map <
                    std::string,
                    std::unordered_map <
                        float,
                        std::unordered_map <
                            std::string, float
                        >
                    >
                >
            >
        > NEMAGeometries;
inline NEMAGeometries PMBGeometries() {
    NEMAGeometries geometry = {{"DIAMETER", {
             {75., {
                    {"WIDTH", {
                                 {4., {
                                        {"RADIUS", 385.7},
                                        {"NO_OF_STRIPS", 590}
                                      }
                                 },
                                 {7., {
                                        {"RADIUS", 384.4},
                                        {"NO_OF_STRIPS", 336}
                                     }
                                 }
                              }
                    },
                    {"LENGTH", {
                         {200., {{"RINGS", 53}}},
                         {500., {{"RINGS", 132}}},
                         {1000., {{"RINGS", 264}}}
                    }}
                }
             },
             {85., {
                    {"WIDTH", {
                                 {4., {
                                        {"RADIUS", 435.3},
                                        {"NO_OF_STRIPS", 668}
                                      }
                                 },
                                 {7., {
                                        {"RADIUS", 435.6},
                                        {"NO_OF_STRIPS", 382}
                                      }
                                 }
                              }
                    },
                    {"LENGTH", {
                        {200., {{"RINGS", 53}}},
                        {500., {{"RINGS", 132}}},
                        {1000., {{"RINGS", 264}}}
                    }}
                }
             },
             {95., {
                    {"WIDTH", {
                                 {4., {
                                        {"RADIUS", 485.0},
                                        {"NO_OF_STRIPS", 746}
                                      }
                                 },
                                 {7., {
                                        {"RADIUS", 484.6},
                                        {"NO_OF_STRIPS", 426}
                                      }
                                 }
                              }
                    },
                    {"LENGTH", {
                        {200., {{"RINGS", 53}}},
                        {500., {{"RINGS", 132}}},
                        {1000., {{"RINGS", 264}}}
                    }}
                }
             }
         }
    }};
    return geometry;
}
