#include "parser.h"

std::string omfl::CheckType(std::string key) {

    if (key == "false" || key == "true") {
        return "bool";
    }

    bool temp_bool = true;

    if (key[0] == '"') {
        if (key[key.size()-1] == '"') {
            for (int i = 1; i < key.size() - 1; i++) {
                if (key[i] == '"') {
                    temp_bool = false;
                }
            }

            if (temp_bool) {
                return "string";
            } else {
                return "error";
            }

        } else {
            return "error";
        }
    } else {

        if (key[0] == '-' || key[0] == '+') {
            if (key.size() == 1) {
                return "error";
            } else {
                for (int i = 1; i < key.size(); i++) {
                    if (!(isdigit(key[i])) && key[i] != '.') {
                        return "error";
                    }
                }
            }
        } else {
            for (int i = 0; i < key.size(); i++) {
                if (!(isdigit(key[i])) && key[i] != '.') {
                    return "error";
                }
            }
        }

        if (key.find('.') == std::string::npos) {

            return "int";

        } else {

            if (key[0] == '+' || key[0] == '-') {

                if (key.find('.') > 1 && key.find('.') < key.size() - 1 && key.size() >= 3) {
                    return "float";
                } else {
                    return "error";
                }

            } else {

                if (key.find('.') > 0 && key.find('.') < key.size() - 1 && key.size() >= 3) {
                    return "float";
                } else {
                    return "error";
                }
            }
        }
    }

}


omfl::MassiveArgument* omfl::ParseMassive(std::string key) {

    MassiveArgument* result = new MassiveArgument;
    Argument* temp;

    MassiveArgument* current_values = new MassiveArgument;

    std::string current_value;
    int arrays_amount = 0;

    for (int i = 0; i < key.size(); i++) {

        if (key[i] == '[') {

            arrays_amount++;
            MassiveArgument* current_values = new MassiveArgument;

        } else if (key[i] == ',' || key[i] == ']') {

            Argument* temp_main;

            if (CheckType(current_value) == "bool") {

                BoolArgument* temp = new BoolArgument;
                temp->SetType("bool");
                temp->SetName(current_value);

                if (current_value == "true") {
                    temp->SetValue(true);
                } else if (current_value == "false") {
                    temp->SetValue(false);
                }

                temp_main = temp;

            } else if (CheckType(current_value) == "string") {

                StringArgument* temp = new StringArgument;
                temp->SetType("string");
                temp->SetValue(current_value);
                temp_main = temp;

            } else if (CheckType(current_value) == "int") {

                IntArgument* temp = new IntArgument;
                temp->SetType("int");
                temp->SetValue(stoi(current_value));
                temp_main = temp;

            } else if (CheckType(current_value) == "float") {

                IntArgument* temp = new IntArgument;
                temp->SetType("float");
                temp->SetValue(::atof(current_value.c_str()));
                temp_main = temp;

            } else if (CheckType(current_value) == "error") {
                return nullptr;
            }

            current_values->SetValue(temp_main);
            current_value = "";
        } else if (key[i] != ' ') {
            current_value += key[i];
        }

        if (key[i] == ']') {
            if (arrays_amount == 1) {
                result->SetValue(current_values);
            } else {
                if (current_values->GetValue().size() == 1) {
                    reinterpret_cast<MassiveArgument*>(result->GetValue().back())->SetValue(current_values);
                } else {
                    Argument* temp = current_values;
                    if (result->GetValue().size() == 0) {
                        result->SetValue(temp);
                    } else {
                        reinterpret_cast<MassiveArgument*>(result->GetValue().back())->SetValue(temp);
                    }
                }
                current_values = new MassiveArgument;
            }
            arrays_amount--;
        }

    }


    return result;
}

bool CheckSectionName(std::string key) {

    return true;
}

omfl::OMFLParser omfl::parse(const std::string& str) {

    OMFLParser result;

    std::string work = str + '\n';
    std::string current_section;
    std::string current_key;
    std::string current_value;

    std::vector<std::string> current_keys;
    std::vector<std::string> current_values;
    std::vector<std::string> current_sections;

    int status = 0;

    std::string temp_str;
    std::string test_str;

    for (int i = 0; i < work.size(); i++) {

        char test = work[i];

        if (status == 0) {

            if (work[i] == '#') {
                status = 1;
            } else if (work[i] == '[') {
                status = 2;
            } else if (!(work[i] == ' ' || work[i] == '\n' || work[i] == '\t')) {
                current_key += work[i];
                status = 3;
            }


        } else if (status == 1) {

            if (work[i] == '\n') {
                status = 0;
            }

        } else if (status == 2) {

            if (work[i] == ']') {

                // Check section CheckSectionName(); TODO

                current_sections.push_back(current_section);
                current_section = "";
                status = 0;
            } else if (work[i] == '\n') {
                result.SetIsValid(false);
                return result;
            } else {
                current_section += work[i];
            }

        } else if (status == 3) {


            if (work[i] == ' ' || work[i] == '\t' || work[i] == '=') {
                current_keys.push_back(current_key);
                current_key = "";
                status = 4;
            } else if (work[i] == '\n') {
                result.SetIsValid(false);
                return result;
            } else {
                current_key += work[i];
            }


        } else if (status == 4) {

            if (work[i] == '=' || work[i] == ' ') {
                status = 5;
            } else if (work[i] == '\n') {
                result.SetIsValid(false);
                return result;
            } else if (work[i] != ' '  && work[i] != '\t') {
                result.SetIsValid(false);
                return result;
            }

        } else if (status == 5) {

            if (work[i] == '\n') {
                result.SetIsValid(false);
                return result;
            } else if (work[i] != ' '  && work[i] != '\t') {
                current_value += work[i];
                status = 6;
            }

        } else if (status == 6) {


            if (work[i] == '\n') {
                current_values.push_back(current_value);
                current_value = "";


                Argument* temp_main;

                // CHECK WHAT TYPE BEFORE ADDING

                if (current_values.back()[0] == '[') {

                    MassiveArgument* temp = new MassiveArgument;
                    temp = ParseMassive(current_values.back());
                    if (temp == nullptr) {
                        result.SetIsValid(false);
                        return result;
                    }
                    temp->SetType("massive");
                    temp->SetName(current_keys.back());
                    temp_main = temp;

                } else if (CheckType(current_values.back()) == "bool") {

                    BoolArgument* temp = new BoolArgument;
                    temp->SetType("bool");
                    temp->SetName(current_keys.back());

                    if (current_values.back() == "true") {
                        temp->SetValue(true);
                    } else if (current_values.back() == "false") {
                        temp->SetValue(false);
                    }

                    temp_main = temp;

                } else if (CheckType(current_values.back()) == "string") {

                    StringArgument* temp = new StringArgument;
                    temp->SetType("string");
                    temp->SetName(current_keys.back());
                    temp->SetValue(current_values.back());
                    temp_main = temp;

                } else if (CheckType(current_values.back()) == "int") {

                    IntArgument* temp = new IntArgument;
                    temp->SetName(current_keys.back());
                    temp->SetType("int");
                    temp->SetValue(stoi(current_values.back()));
                    temp_main = temp;

                } else if (CheckType(current_values.back()) == "float") {

                    FloatArgument* temp = new FloatArgument;
                    temp->SetName(current_keys.back());
                    temp->SetType("float");
                    temp->SetValue(::atof(current_values.back().c_str()));
                    temp_main = temp;

                } else if (CheckType(current_values.back()) == "error") {
                    result.SetIsValid(false);
                    return result;
                }


                if (current_sections.empty()) {
                    result.SetBasicArguments(temp_main);
                } else {

                    bool k = true;

                    for (int j = 0; j < result.GetBasicArguments().size(); j++) {
                        if (std::string(result.GetBasicArguments()[j]->GetName()) == std::string(current_sections.back())) {
                            k = false;
                        }
                    }

                    if (k) {

                        SectionArgument* temp = new SectionArgument;
                        temp->SetSectionArgument(temp_main);
                        temp->SetName(current_sections.back());
                        result.SetBasicArguments(temp);

                    } else {

                        for (int j = 0; j < result.GetBasicArguments().size(); j++) {
                            if (result.GetBasicArguments()[j]->GetName() == current_sections.back()) {
                                reinterpret_cast<SectionArgument*>(result.GetBasicArguments()[j])->SetSectionArgument(temp_main);
                            }
                        }

                    }


                }

                status = 0;

            } else if (work[i] != ' '  && work[i] != '\t'){
                current_value += work[i];
            }
        }
    }


    return result;
}




bool omfl::OMFLParser::valid() const {

    return is_valid_;
}

omfl::Argument& omfl::Argument::Get(const std::string& currect_section) {
    for (auto& argument : this->GetSectionArguments()) {
        if (std::string(currect_section) == std::string(argument->name_)) {
            return *argument;
        }
    }
}

omfl::Argument& omfl::Argument::Get(std::vector<std::string>& keys_names, Argument& key, int iterator) const {

    if (keys_names.size() == iterator) {
        return key;
    }

    for (int i = 0; i < key.GetSectionArguments().size(); i++) {
        if (keys_names[iterator] == key[i].name_) {
            Get(keys_names, key[i], iterator++);
        }
    }

}

omfl::Argument& omfl::OMFLParser::Get(const std::string& key) const {

    char point = '.';
    size_t first = 0, last = 0;
    std::vector<std::string> keys_names;

    while ((first = (key.find_first_not_of(point, last))) != std::string::npos) {
        last = key.find(point, first);
        keys_names.push_back(key.substr(first, last - first));
    }

    if (keys_names.size() == 1) {
        return FindArgument(keys_names[0]);
    } else {
        Argument temp = FindArgument(keys_names[0]);
        temp.Get(keys_names, temp, 1);
    }

}

omfl::Argument& omfl::OMFLParser::FindArgument(const std::string& key) const {
    for (const auto& argument : basic_arguments) {
        if (key == argument->GetName()) {
            return *argument;
        }
    }
}

bool omfl::Argument::IsInt() {

    if (this->type_ == "int") {
        return true;
    }

    return false;
}

bool omfl::Argument::IsString() {

    if (this->type_ == "string") {
        return true;
    }

    return false;
}

bool omfl::Argument::IsFloat() {

    if (this->type_ == "float") {
        return true;
    }

    return false;
}

bool omfl::Argument::IsBool() {

    if (this->type_ == "bool") {
        return true;
    }

    return false;
}

bool omfl::Argument::IsArray() {

    if (this->type_ == "array") {
        return true;
    }

    return false;
}

int omfl::Argument::AsInt() {

    return reinterpret_cast<IntArgument*>(this)->GetValue();
}

std::string omfl::Argument::AsString() {

    return reinterpret_cast<StringArgument*>(this)->GetValue();
}

double omfl::Argument::AsFloat() {

    return reinterpret_cast<FloatArgument*>(this)->GetValue();
}

bool omfl::Argument::AsBool() {

    return reinterpret_cast<BoolArgument*>(this)->GetValue();
}

int omfl::Argument::AsIntOrDefault(int default_value) {

    if (this->type_ != "int") {
        return default_value;
    }

    return reinterpret_cast<IntArgument*>(this)->GetValue();
}

double omfl::Argument::AsFloatOrDefault(double default_value) {

    if (this->type_ != "float") {
        return default_value;
    }

    return reinterpret_cast<FloatArgument*>(this)->GetValue();
}

std::string omfl::Argument::AsStringOrDefault(std::string default_value) {

    if (this->type_ != "string") {
        return default_value;
    }

    return reinterpret_cast<StringArgument*>(this)->GetValue();
}

omfl::Argument& omfl::Argument::operator[](int index) {

    return reinterpret_cast<MassiveArgument*>(this)->operator[](index);
}

std::vector<omfl::Argument*> omfl::Argument::GetSectionArguments() {

    return reinterpret_cast<SectionArgument*>(this)->GetSectionArguments();
}
