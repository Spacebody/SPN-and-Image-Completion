#include "decomposition.hpp"
#include <string>
#include <cstring>

std::map<std::string, Decomposition> Decomposition::id_decomp = std::map<std::string, Decomposition>();
Decomposition Decomposition::blank_decomp = Decomposition("", -1, -1, -1, -1);

Decomposition Decomposition::get_decomposition(int region_id_1, int region_id_2, int type_id_1, int type_id_2)
{
    std::string id = Decomposition::get_id_str(region_id_1, region_id_2, type_id_1, type_id_2);
    Decomposition d;
    if (Decomposition::get_id_decomp().count(id) == 0)
        Decomposition::add_id_decomp(std::pair<std::string, Decomposition>(id, Decomposition()));
    else
        d = Decomposition::get_id_decomp()[id];
    return d;
}

Decomposition Decomposition::get_decomposition(std::string id)
{
    Decomposition d;
    if (Decomposition::get_id_decomp().count(id) == 0)
    {
        if (id == "")
            return Decomposition::get_blank_decomp();
        char *str = strdup(id.c_str());
        char *token = str, *rest = str;
        strsep(&rest, " ");
        int region_id_1 = strtol(token, NULL, 10);
        token = rest;
        strsep(&rest, " ");
        int region_id_2 = strtol(token, NULL, 10);
        token = rest;
        strsep(&rest, " ");
        int type_id_1 = strtol(token, NULL, 10);
        token = rest;
        strsep(&rest, " ");
        int type_id_2 = strtol(token, NULL, 10);
        token = rest;
        d = Decomposition(id, region_id_1, region_id_2, type_id_1, type_id_2);
    }
    else
        d = Decomposition::get_id_decomp()[id];
    return d;
}

std::string Decomposition::get_id()
{
    return this->id;
}

std::map<std::string, Decomposition> Decomposition::get_id_decomp()
{
    return Decomposition::id_decomp;
}

Decomposition Decomposition::get_blank_decomp()
{
    return Decomposition::blank_decomp;
}

void Decomposition::add_id_decomp(std::pair<std::string, Decomposition> new_decomp)
{
    Decomposition::id_decomp.insert(new_decomp);
}

void Decomposition::remove(std::string id)
{
    std::map<std::string, Decomposition>::iterator iter = Decomposition::id_decomp.find(id);
    Decomposition::id_decomp.erase(iter);
}

std::string Decomposition::get_id_str(int region_id_1, int region_id_2, int type_id_1, int type_id_2)
{
    std::string id = std::to_string(region_id_1) + " " + std::to_string(region_id_2) +
                     " " + std::to_string(type_id_1) + " " + std::to_string(type_id_2);
    return id;
}
