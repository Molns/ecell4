#include <numeric>
#include "SubvolumeSpace.hpp"
#include "Context.hpp"

namespace ecell4
{

Integer SubvolumeSpaceVectorImpl::num_molecules(const Species& sp) const
{
    SpeciesExpressionMatcher sexp(sp);
    Integer retval(0);
    for (matrix_type::const_iterator i(matrix_.begin());
        i != matrix_.end(); ++i)
    {
        if (sexp.match((*i).first))
        {
            do
            {
                retval += std::accumulate((*i).second.begin(), (*i).second.end(), 0);
            } while (sexp.next());
        }
    }
    return retval;
}

Integer SubvolumeSpaceVectorImpl::num_molecules_exact(const Species& sp) const
{
    matrix_type::const_iterator i(matrix_.find(sp));
    if (i == matrix_.end())
    {
        return 0;
    }
    return std::accumulate((*i).second.begin(), (*i).second.end(), 0);
}

Integer SubvolumeSpaceVectorImpl::num_molecules(
    const Species& sp, const coordinate_type& c) const
{
    SpeciesExpressionMatcher sexp(sp);
    Integer retval(0);
    for (matrix_type::const_iterator i(matrix_.begin());
        i != matrix_.end(); ++i)
    {
        if (sexp.match((*i).first))
        {
            do
            {
                retval += (*i).second[c];
            } while (sexp.next());
        }
    }
    return retval;
}

Integer SubvolumeSpaceVectorImpl::num_molecules_exact(
    const Species& sp, const coordinate_type& c) const
{
    matrix_type::const_iterator i(matrix_.find(sp));
    if (i == matrix_.end())
    {
        return 0;
    }
    return (*i).second[c];
}

void SubvolumeSpaceVectorImpl::reserve_species(
    const Species& sp, const coordinate_type& c)
{
    matrix_type::const_iterator i(matrix_.find(sp));
    if (i != matrix_.end())
    {
        throw AlreadyExists("Species already exists");
    }
    matrix_.insert(std::make_pair(sp, std::vector<Integer>(num_subvolumes())));
    species_.push_back(sp);
}

void SubvolumeSpaceVectorImpl::add_molecules(
    const Species& sp, const Integer& num, const coordinate_type& c)
{
    matrix_type::iterator i(matrix_.find(sp));
    if (i == matrix_.end())
    {
        reserve_species(sp, c);
        i = matrix_.find(sp);
    }
    (*i).second[c] += num;
}

void SubvolumeSpaceVectorImpl::remove_molecules(
    const Species& sp, const Integer& num, const coordinate_type& c)
{
    matrix_type::iterator i(matrix_.find(sp));
    if (i == matrix_.end())
    {
        std::ostringstream message;
        message << "Speices [" << sp.serial() << "] not found";
        throw NotFound(message.str());
    }

    if ((*i).second[c] < num)
    {
        std::ostringstream message;
        message << "The number of molecules cannot be negative. [" << sp.serial() << "]";
        throw std::invalid_argument(message.str());
    }

    (*i).second[c] -= num;
}

SubvolumeSpaceVectorImpl::coordinate_type SubvolumeSpaceVectorImpl::get_neighbor(
    const coordinate_type& c, const Integer rnd) const
{
    Integer3 g(coord2global(c));

    switch (rnd)
    {
    case 0:
        return global2coord(g.east());
    case 1:
        return global2coord(g.west());
    case 2:
        return global2coord(g.south());
    case 3:
        return global2coord(g.north());
    case 4:
        return global2coord(g.dorsal());
    case 5:
        return global2coord(g.ventral());
    }

    throw IllegalState("the number of neighbors is less than 6.");
}

} // ecell4
