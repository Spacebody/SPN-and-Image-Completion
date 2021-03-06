#ifndef DECOMPOSITION_HPP
#define DECOMPOSITION_HPP
#include <map>
#include <string>

// represent a decomposition of a region into two sub-regions
class Decomposition
{
  private:
    std::string id;

  public:
    Decomposition() : id(""), region_id_1(-1), region_id_2(-1), type_id_1(-1), type_id_2(-1) {} //default constructor
    // constructor
    Decomposition(std::string id, int region_id_1, int region_id_2, int type_id_1, int type_id_2) : id(id), region_id_1(region_id_1), region_id_2(region_id_2), type_id_1(type_id_1), type_id_2(type_id_2) {}
    ~Decomposition() {} // deconstructor

    static std::map<std::string, Decomposition> id_decomp;
    static Decomposition blank_decomp;
    int region_id_1, region_id_2, type_id_1, type_id_2;

    static Decomposition &get_decomposition(int region_id_1, int region_id_2, int type_id_1, int type_id_2);
    static Decomposition get_decomposition(std::string id);
    std::string get_id();
    static void remove(std::string id);
    static std::string get_id_str(int region_id_1, int region_id_2, int type_id_1, int type_id_2);
};

#endif
