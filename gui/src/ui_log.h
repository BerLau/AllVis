#pragma once
#ifndef UI_LOG_H
#define UI_LOG_H

#include <string>
#include <vector>

namespace GUI
{
    struct Log
    {
        std::vector<std::string> messages;
        int max_lines = 1000;
        void log(const std::string &msg)
        {
            messages.push_back(msg);
            if (messages.size() > max_lines)
            {
                messages.erase(messages.begin());
            }
        }

        void warn(const std::string &msg)
        {
            log("[WARN] " + msg);
        }

        void error(const std::string &msg)
        {
            log("[ERROR] " + msg);
        }

        void info(const std::string &msg)
        {
            log("[INFO] " + msg);
        }
        
        void clear()
        {
            messages.clear();
        }

        static Log &get()
        {
            static Log instance;
            return instance;
        }
    };
};

#endif