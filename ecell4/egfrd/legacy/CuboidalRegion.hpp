#ifndef CUBOIDAL_REGION_HPP
#define CUBOIDAL_REGION_HPP

#include <boost/bind.hpp>
#include "Region.hpp"
#include "Box.hpp"

template<typename Ttraits_>
class CuboidalRegion
    : public BasicRegionImpl<Ttraits_, Box >
{
public:
    typedef BasicRegionImpl<Ttraits_, Box> base_type;
    typedef typename base_type::traits_type traits_type;
    typedef typename base_type::identifier_type identifier_type;
    typedef typename base_type::shape_type shape_type;
    typedef typename base_type::rng_type rng_type;
    typedef typename base_type::position_type position_type;
    typedef typename base_type::length_type length_type;

    identifier_type const& id() const
    {
        return base_type::id_;
    }

    virtual position_type random_position(rng_type& rng) const
    {
        return ::random_position(base_type::shape(),
                boost::bind(&rng_type::uniform, rng, -1., 1.));
    }

    virtual position_type random_vector(length_type const& r, rng_type& rng) const
    {
        return normalize(
            create_vector<position_type>(
                rng.uniform(-1., 1.),
                rng.uniform(-1., 1.),
                rng.uniform(-1., 1.)),
            r);
    }

    virtual position_type bd_displacement(length_type const& sigma, rng_type& rng) const
    {
        return create_vector<position_type>(
            rng.gaussian(sigma), rng.gaussian(sigma), rng.gaussian(sigma));
    }

    virtual void accept(ImmutativeStructureVisitor<traits_type> const& visitor) const
    {
        visitor(*this);
    }

    virtual void accept(MutativeStructureVisitor<traits_type> const& visitor)
    {
        visitor(*this);
    }

    CuboidalRegion(identifier_type const& id, shape_type const& shape)
        : base_type(id, shape) {}
};

#endif /* CUBOIDAL_REGION_HPP */
