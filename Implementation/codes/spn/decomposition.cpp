#include "decomposition.hpp"
#include <string>
#include <vector>
#include <sstream>

std::map<std::string, Decomposition> Decomposition::id_decomp = std::map<std::string, Decomposition>();
Decomposition Decomposition::blank_decomp = Decomposition("", -1, -1, -1, -1);

Decomposition Decomposition::get_decomposition(int region_id_1, int region_id_2, int type_id_1, int type_id_2)
{
    std::string id = Decomposition::get_id_str(region_id_1, region_id_2, type_id_1, type_id_2);
    Decomposition d;
    if (Decomposition::id_decomp.count(id) == 0)
        Decomposition::id_decomp.insert(std::pair<std::string, Decomposition>(id, Decomposition()));
    else
        d = Decomposition::id_decomp[id];
    return d;
}

Decomposition Decomposition::get_decomposition(std::string id)
{
    Decomposition d;
    if (Decomposition::id_decomp.count(id) == 0)
    {
        if (id == "")
            return Decomposition::blank_decomp;
        std::vector<std::string> ts;
        std::stringstream ss(id); // split by ' '
        ts = std::vector<std::string>((std::istream_iterator<std::string>(ss)), std::istream_iterator<std::string>());
        int region_id_1 = std::stoi(ts[0]);
        int region_id_2 = std::stoi(ts[1]);
        int type_id_1 = std::stoi(ts[2]);
        int type_id_2 = std::stoi(ts[3]);
        d = Decomposition(id, region_id_1, region_id_2, type_id_1, type_id_2);
    }
    else
        d = Decomposition::id_decomp[id];
    return d;
}

std::string Decomposition::get_id()
{
    return this->id;
}

void Decomposition::remove(std::string id)
{
    Decomposition::id_decomp.erase(id);
}

std::string Decomposition::get_id_str(int region_id_1, int region_id_2, int type_id_1, int type_id_2)
{
    std::string id = std::to_string(region_id_1) + " " + std::to_string(region_id_2) +
                     " " + std::to_string(type_id_1) + " " + std::to_string(type_id_2);
    return id;
}
