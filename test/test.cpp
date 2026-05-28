#include "md5.h"

#include <iostream>
#include <string>
#include <vector>

struct MD5_Case {
    std::string input;
    std::string expect;
};

int main() {

    std::vector<MD5_Case> test_cases = {
        {"", "d41d8cd98f00b204e9800998ecf8427e"},
        {"Hello, World!", "65a8e27d8879283831b664bd8b7f0ad4"},
        {"a", "0cc175b9c0f1b6a831c399e269772661"},
        {"123456", "e10adc3949ba59abbe56e057f20f883e"},
        {"Hello MD5", "e5dadf6524624f79c3127e247f04b548"},
        {"!@#$%^&*()", "05b28d17a7b6e7024b6e5d8cc43a8bf7"},
        {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "5eca9bd3eb07c006cd43ae48dfde7fd3"},
        {"你好，世界", "dbefd3ada018615b35588a01e216ae6e"}
    };

    for(auto& md5_case: test_cases) {
        MD5 md5_obj;
        md5_obj.update(md5_case.input);
        if(md5_obj.hexdigest() == md5_case.expect) {
            std::cout << "Pass" << std::endl;
        } else {
            std::cout << "No, md5 value: " << md5_obj.hexdigest() << std::endl;
        }
    }
    return 0;
}