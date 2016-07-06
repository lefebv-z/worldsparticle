/* 
 * File:   zoneData.hpp
 * Author: marie
 *
 * Created on 6 juillet 2016, 19:58
 */

#ifndef ZONEDATA_HPP
#define ZONEDATA_HPP

#include "elementData.hpp"
#include "climateData.hpp"
//#include "../map/zone.hpp"
#include "../param/boolvalue.hpp"
#include "../param/intvalue.hpp"

namespace GenData
{

struct ZoneData
{
    //ctor
    ZoneData(map::Biome const& t, bool a = false)
    : type(t), active("active"), density("density"),
    elements(), climate()
    {
        active.setValue(a);

        density.setMinValue(1);
        density.setMaxValue(100);
        density.setValue(50);
//        params().push_back(&density);
    }


    //type
    map::Biome    type;
    
    inline const std::string    &name() const
    { return map::BiomeName::getBiomeName(type); }


    //params
    gen::BoolValue   active;
    gen::IntValue   density;

    std::vector<ElementData>    elements;
    
    ClimateData climate;
    

private:
    //std::vector<Zone>   voronoiZones;//useful?
    
    //std::list<Param *>  m_params;
};
}

#endif /* ZONEDATA_HPP */

