#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <clocale>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

class CommandLine {
public:
    CommandLine(int argn, char** argc) {
        std::setlocale(LC_ALL, "en_US.UTF-8");
        for (size_t i = 0; i < size_t(argn); ++i) {
            params.emplace_back(argc[i]);
            flags.push_back(ParamCodes::unused);
        }
    }

    std::string strArg(const std::string& name, const std::string def = "") {
        auto ind = findName(name);
        if (ind + 1 < params.size()) {
            flags[ind + 1] = ParamCodes::used;
            return params[ind + 1];
        } else if (ind < params.size()) {
            flags[ind] = ParamCodes::error;
            std::cout << "found argument \"" << name << "\" without following integer!"
                << std::endl;
        }
        return def;
    }

    int intArg(const std::string& name, int def = 0) {
        auto ind = findName(name);
        if (ind + 1 < params.size()) {
            flags[ind + 1] = ParamCodes::used;
            int  r = 0;
            try {
                r = std::stoi(params[ind + 1]);
            }
            catch (std::invalid_argument& e) {
                flags[ind + 1] = ParamCodes::error;
                r = def;
                std::cout << "error reading int argument \"" << name
                    << "\" from console, got \"invalid_argument exception\""
                    << std::endl;
            }
            return r;
        } else if (ind < params.size()) {
            flags[ind] = ParamCodes::error;
            std::cout << "found argument \"" << name << "\" without following integer!"
                << std::endl;
        }
        return def;

    }

    ads_robert::Number longArg(const std::string& name, int def = 0) {
        auto ind = findName(name);
        if (ind + 1 < params.size()) {
            flags[ind + 1] = ParamCodes::used;
            ads_robert::Number r = 0;
            try {
                r = std::stol(params[ind + 1]);
            }
            catch (std::invalid_argument& e) {
                flags[ind + 1] = ParamCodes::error;
                r = def;
                std::cout << "error reading int argument \"" << name
                    << "\" from console, got \"invalid_argument exception\""
                    << std::endl;
            }
            return r;
        } else if (ind < params.size()) {
            flags[ind] = ParamCodes::error;
            std::cout << "found argument \"" << name << "\" without following integer!"
                << std::endl;
        }
        return def;

    }

    double doubleArg(const std::string& name, double def = 0.) {
        std::setlocale(LC_ALL, "en_US.UTF-8");
        auto ind = findName(name);
        if (ind + 1 < params.size()) {
            flags[ind + 1] = ParamCodes::used;
            double  r = 0;
            try {
                r = std::stod(params[ind + 1]);
            }
            catch (std::invalid_argument& e) {
                flags[ind + 1] = ParamCodes::error;
                r = def;
                std::cout << "error reading double argument \"" << name
                    << "\" from console, got \"invalid-argument exception\"!"
                    << std::endl;
            }
            return r;
        } else if (ind < params.size()) {
            flags[ind] = ParamCodes::error;
            std::cout << "found argument \"" << name << "\" without following double!"
                << std::endl;
        }
        return def;
    }

    bool boolArg(const std::string& name) {
        return (findName(name)) < params.size();
    }

    bool report() {
        bool un = true;
        for (size_t i = 1; i < params.size(); ++i) {
            if (flags[i] != ParamCodes::used) {
                if (flags[i] == ParamCodes::unused) {
                    std::cout << "parameter " << i << " = \"" << params[i]
                        << "\" was unused!" << std::endl;
                } else if (flags[i] == ParamCodes::error) {
                    std::cout << "error reading parameter " << i
                        << " = \"" << params[i] << "\"" << std::endl;
                }
                un = false;
            }
        }
        return un;
    }

private:
    enum class ParamCodes {
        unused,
        used,
        error
    };

    std::vector<std::string> params;
    std::vector<ParamCodes>  flags;

    size_t findName(const std::string& name) {
        for (size_t i = 0; i < params.size(); ++i) {
            if (params[i] == name) {
                flags[i] = ParamCodes::used;
                return i;
            }
        }
        return params.size();
    }

};


#endif // COMMANDLINE_H
