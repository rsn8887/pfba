//
// Created by cpasjuste on 05/12/16.
//

#include "option.h"

Option::Option(const std::string &text,
               const std::vector<std::string> &options,
               int defaultValue, Index idx, Type type) {
    this->type = type;
    this->text = text;
    this->options = options;
    this->value = defaultValue;
    this->index = idx;
}

const char *Option::GetName() {
    return text.c_str();
}

const char *Option::GetValue() {
    if(value >= options.size()) {
        return "<NA>";
    }
    return options[value].c_str();
}

void Option::Next() {
    value++;
    if (value >= options.size())
        value = 0;
}

void Option::Prev() {
    value--;
    if (value < 0)
        value = (int) (options.size() - 1);
}
