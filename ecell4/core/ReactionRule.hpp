#ifndef __ECELL4_REACTION_RULE_HPP
#define __ECELL4_REACTION_RULE_HPP

// #include <set>
#include <stdexcept>

#include "types.hpp"
#include "Species.hpp"
#include "Ratelaw.hpp"


namespace ecell4
{

class ReactionRule
{
public:

    /**
     * a type of the container of reactants
     * std::multiset allows multiple keys with equal values,
     * but looses the original order at the registration.
     * when changing this type into the ordered one,
     * please modify NetworkModel too.
     */
    typedef std::vector<Species> reactant_container_type;
    typedef std::vector<Species> product_container_type;

public:

    ReactionRule()
        : k_(0), reactants_(), products_()
    {
        ;
    }

    ReactionRule(
        const reactant_container_type& reactants,
        const product_container_type& products)
        : k_(0), reactants_(reactants), products_(products)
    {
        ;
    }

    ReactionRule(
        const reactant_container_type& reactants,
        const product_container_type& products,
        const Real& k)
        : k_(k), reactants_(reactants), products_(products)
    {
        ;
    }

    Real k() const
    {
        return k_;
    }

    const reactant_container_type& reactants() const
    {
        return reactants_;
    }

    const product_container_type& products() const
    {
        return products_;
    }

    void set_k(const Real& k)
    {
        if (k < 0)
        {
            throw std::invalid_argument("a kinetic rate must be positive.");
        }
        k_ = k;
    }

    void add_reactant(const Species& sp)
    {
        reactants_.push_back(sp);
    }

    void add_product(const Species& sp)
    {
        products_.push_back(sp);
    }

    // Ratelaw related functions.
    void set_ratelaw(const boost::shared_ptr<Ratelaw> ratelaw)
    {
        this->ratelaw_ = ratelaw;
    }
    boost::shared_ptr<Ratelaw> get_ratelaw() const
    {
        return this->ratelaw_.lock();
    }
    bool has_ratelaw() const
    {
        return !(this->ratelaw_.expired());
    }

    const std::string as_string() const;
    Integer count(const reactant_container_type& reactants) const;
    std::vector<ReactionRule> generate(const reactant_container_type& reactants) const;

protected:

    Real k_;
    reactant_container_type reactants_;
    product_container_type products_;
    boost::weak_ptr<Ratelaw> ratelaw_;
};

inline bool operator<(const ReactionRule& lhs, const ReactionRule& rhs)
{
    if (lhs.reactants() < rhs.reactants())
    {
        return true;
    }
    else if (lhs.reactants() > rhs.reactants())
    {
        return false;
    }
    return (lhs.products() < rhs.products());
}

inline bool operator==(const ReactionRule& lhs, const ReactionRule& rhs)
{
    return ((lhs.reactants() == rhs.reactants())
            && (lhs.products() == rhs.products()));
}

inline bool operator!=(const ReactionRule& lhs, const ReactionRule& rhs)
{
    return !(lhs == rhs);
}

} // ecell4

#endif /* __ECELL4_REACTION_RULE_HPP */
