#pragma once

#include <unordered_map>
#include <string>

class AsciiArt {
public:
    enum Id {
        FireBalls,
        MaxHealth,
        LowCut,
        LowCutUpgradeFireRate,
        LowCutUpgradeFireRate2,
        HealthRegenerateFaster,
        HealthRegenerateMore,
        RunFaster
    };

    AsciiArt(){
        asciiArt[Id::FireBalls] = trim(R"(
(X) ` `___'  Fire balls           
     '/   \`                      
     |     | damage     ▓▓▓▒▒▒▒▒▒▒
      \___/  fire rate  ▓▓▓▓▓▓▓▒▒▒
)");
        asciiArt[Id::LowCut] = trim(R"(
(X)  \\      Low cut              
      \\                          
       \\*   damage     ▓▓▓▓▓▓▓▓▒▒
        \|   fire rate  ▓▓▒▒▒▒▒▒▒▒
)");
        asciiArt[Id::LowCutUpgradeFireRate] = trim(R"(
(X)  \\      Cut faster           
      \\                          
       \\*   damage     ▓▓▓▓▓▓▓▓▒▒
        \|   fire rate  ▓▓▓▓▒▒▒▒▒▒
)");
        asciiArt[Id::LowCutUpgradeFireRate2] = trim(R"(
(X)  \\      Cut even faster      
      \\                          
       \\*   damage     ▓▓▓▓▓▓▓▓▒▒
        \|   fire rate  ▓▓▓▓▓▓▒▒▒▒
)");
        asciiArt[Id::MaxHealth] = trim(R"(
(X)   _  _    Max health          
     / \/ \   +10%                
     \    /                       
      \__/max                     
)");
        asciiArt[Id::HealthRegenerateFaster] = trim(R"(
(X)  ,_  _.   Regenerate faster   
    // \/ \\                      
    \\    //                      
     \\__//                       
)");
        asciiArt[Id::HealthRegenerateMore] = trim(R"(
(X)  +_ +_+   Regenerate more     
    +/ \/ \+                      
    +\    /+                      
     +\__/+                     
)");
        asciiArt[Id::RunFaster] = trim(R"(
(X) - //\\    Run faster          
   - // //_                       
  - //_ ====  speed     ▓▓▓▒▒▒▒▒▒▒
  - ====                          
)");

    }

    std::string& get(Id id){
        return asciiArt[id];
    }

private:
    std::unordered_map<Id, std::string> asciiArt;

    std::string trim(std::string str) {
        size_t startIndex = 0;
        while (startIndex < str.size() && (str[startIndex] == ' ' || str[startIndex] == '\n')) {
            startIndex += 1;
        }

        if (startIndex == str.size()) return "";

        size_t endIndex = str.size() - 1;
        while (endIndex > startIndex && (str[endIndex] == ' ' || str[endIndex] == '\n')) {
            endIndex -= 1;
        }

        return str.substr(startIndex, endIndex - startIndex + 1);
    }
};
