#pragma once
#ifndef GUI_CONFIG_H
#define GUI_CONFIG_H

#include <string>
#include <fstream>
#include <sstream>

namespace GUI
{
    const std::string& CONFIG_FILE = "gui.config";
    struct Config
    {
        std::string layout_files_dir = "layouts/";
        std::string layout_file = "default.layout";

        void save(){
            std::ofstream file(CONFIG_FILE);
            file << "GUI_CONFIG" << std::endl;
            file << "layout_files_dir=" << layout_files_dir << std::endl;
            file << "layout_file=" << layout_file << std::endl;
            file.close();
        }

        static Config load(){
            std::ifstream file(CONFIG_FILE);
            std::string line;
            std::string key;
            Config config;
            while(std::getline(file, line)){
                std::stringstream ss(line);
                ss >> key;
                if(key == "layout_files_dir="){
                    ss >> config.layout_files_dir;
                }else if(key == "layout_file="){
                    ss >> config.layout_file;
                }
            }
            file.close();
            return config;
        }

        static Config& get(){
            static Config instance = load();
            return instance;
        }

    };
}; // namespace GUI


#endif // GUI_CONFIG_H