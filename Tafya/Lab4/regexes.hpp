#ifndef HPP_REGEXES_PROTECTION
#define HPP_REGEXES_PROTECTION

#include <regex>
#define DEFAULT_ATTR_VA   "top"
#define DEFAULT_ATTR_HA   "left"
#define DEFAULT_ATTR_TC   "15"
#define DEFAULT_ATTR_BC   "0"
#define DEFAULT_ATTR_W    #error define me
#define DEFAULT_ATTR_H    #error define me
#define DEFAULT_ATTR_COL  #error define me
#define DEFAULT_ATTR_ROW  #error define me

const std::regex R_END_OF_TAG        (R"#(>)#");

const std::regex R_BLOCK_START       (R"#(<\s*block)#");
const std::regex R_BLOCK_ATTR_ROW    (R"#(\s*rows=(\d+))#");
const std::regex R_BLOCK_ATTR_COL    (R"#(\s*column=(\d+))#");
const std::regex R_BLOCK_END         (R"#(</\s*block\s*>)#");

const std::regex R_COLUMN_START      (R"#(<\s*column)#");
const std::regex R_COLUMN_END        (R"#(</\s*column\s*>)#");

const std::regex R_ROW_START         (R"#((<\s*row)#");
const std::regex R_ROW_END           (R"#(</\s*row\s*>)#");

const std::regex R_ATTR_VA           (R"#(\s*valign=(top|center|bottom))#");  
const std::regex R_ATTR_HA           (R"#(\s*halign=(left|center|right))#");  
const std::regex R_ATTR_TC           (R"#(\s*textcolor=(\d{1,2}))#");  
const std::regex R_ATTR_BC           (R"#(\s*bgcolor=(\d{1,2}))#");  
const std::regex R_ATTR_W            (R"#(\s*width=(\d+))#");  
const std::regex R_ATTR_H            (R"#(\s*height=(\d+))#");  
#endif
