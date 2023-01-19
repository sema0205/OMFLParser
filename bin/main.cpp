#include "lib/parser.h"

void PrintYaml(omfl::OMFLParser root);


int main(int, char**) {

    std::string data = R"(
        [section1]
        key2 = true
        key241 = -631
        keyds = 431
        keyfdds = "test"
        [section2]
        key3 = "value"
        key532 = "fdsf"
        key41 = -3151.531")";



    auto root = omfl::parse(data);
    PrintYaml(root);
    std::cout << "\n";



    return 0;
}



void PrintYaml(omfl::OMFLParser root) {

    for (auto &basic_argument : root.GetBasicArguments()) {
        std::cout << "- " << basic_argument->GetName() << "\n";
        for (auto &test : basic_argument->GetSectionArguments()) {

            if (test->GetType() == "bool") {
                std::cout << "\t" << test->GetName() << ": " << test->AsBool() << '\n';
            } else if (test->GetType() == "float") {
                std::cout << "\t" << test->GetName() << ": " << test->AsFloat() << '\n';
            } else if (test->GetType() == "int") {
                std::cout << "\t" << test->GetName() << ": " << test->AsInt() << '\n';
            } else if (test->GetType() == "string") {
                std::cout << "\t" << test->GetName() << ": " << test->AsString() << '\n';
            }

        }
    }

}
