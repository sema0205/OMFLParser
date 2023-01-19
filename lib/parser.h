#pragma once

#include <filesystem>
#include <istream>
#include <iostream>


namespace omfl {

    class Argument {
    public:

        Argument& operator[] (int index);
        Argument& Get(std::vector<std::string>& keys_names, Argument& key, int iterator) const;
        Argument& Get(const std::string& currect_section);
        std::vector<Argument*> GetSectionArguments();

        bool IsInt();
        int AsInt();
        int AsIntOrDefault(int default_value);

        bool IsFloat();
        double AsFloat();
        double AsFloatOrDefault(double default_value);

        bool IsString();
        std::string AsString();
        std::string AsStringOrDefault(std::string default_value);

        bool IsBool();
        bool AsBool();

        bool IsArray();

        void SetName(std::string name) {
            name_ = name;
        }

        void SetType(std::string type) {
            type_ = type;
        }

        std::string GetName() const {

            return name_;
        }

        std::string GetType() const {

            return type_;
        }

    private:
        std::string name_;
        std::string type_;
    };

    class IntArgument : public Argument {
    public:

        int GetValue() {
            return int_value_;
        }

        void SetValue(int value) {
            int_value_ = value;
        }

    private:
        int int_value_;
    };

    class FloatArgument : public Argument {
    public:

        double GetValue() {
            return float_value_;
        }

        void SetValue(double value) {
            float_value_ = value;
        }


    private:
        double float_value_;
    };

    class StringArgument : public Argument {
    public:

        std::string GetValue() {
            return string_value_;
        }

        void SetValue(std::string value) {
            string_value_ = value;
        }

    private:
        std::string string_value_;
    };

    class BoolArgument : public Argument {
    public:

        bool GetValue() {
            return bool_value_;
        }

        void SetValue(bool value) {
            bool_value_ = value;
        }

    private:
        bool bool_value_;
    };

    class MassiveArgument : public Argument {
    public:

        std::vector<Argument*> GetValue() {

            return massive_value_;
        }

        void SetValue(Argument* value) {
            massive_value_.push_back(value);
        }

        Argument& operator[] (int index) {

            return *massive_value_[index];
        }

    private:
        std::vector<Argument*> massive_value_;
    };

    class SectionArgument : public Argument {
    public:

        std::vector<Argument*> GetSectionArguments () {

            return section_arguments_;
        }

        void SetSectionArgument (Argument* temp) {

            section_arguments_.push_back(temp);
        }

    private:
        std::vector<Argument*> section_arguments_;
    };

    class OMFLParser {
    public:

        OMFLParser(bool is_valid = true) : is_valid_(is_valid) {};

        bool valid() const;
        Argument& Get(const std::string& key) const;
        Argument& FindArgument(const std::string& key) const;

        void SetIsValid(bool value) {
            is_valid_ = value;
        }

        bool GetIsValid() {

            return is_valid_;
        }

        std::vector<Argument*> GetBasicArguments() {

            return basic_arguments;
        }

        void SetBasicArguments(Argument* temp) {

            basic_arguments.push_back(temp);
        }


    private:
        std::vector<Argument*> basic_arguments;
        bool is_valid_;
    };


OMFLParser parse(const std::filesystem::path& path);
OMFLParser parse(const std::string& str);
std::string CheckType(std::string key);
MassiveArgument* ParseMassive(std::string key);

}// namespace