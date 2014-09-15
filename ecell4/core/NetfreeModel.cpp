#include <algorithm>

#include "exceptions.hpp"
#include "NetfreeModel.hpp"


namespace ecell4
{

std::vector<ReactionRule> NetfreeModel::query_reaction_rules(
    const Species& sp) const
{
    std::vector<ReactionRule> retval;
    for (reaction_rule_container_type::const_iterator i(reaction_rules_.begin());
        i != reaction_rules_.end(); ++i)
    {
        ReactionRuleExpressionMatcher rrexp(*i);
        if (!rrexp.match(sp))
        {
            continue;
        }

        do
        {
            const std::vector<Species> products(rrexp.generate());
            retval.push_back(ReactionRule(rrexp.reactants(), products, (*i).k()));
        }
        while (rrexp.next());
    }
    return retval;
}

std::vector<ReactionRule> NetfreeModel::query_reaction_rules(
    const Species& sp1, const Species& sp2) const
{
    std::vector<ReactionRule> retval;
    for (reaction_rule_container_type::const_iterator i(reaction_rules_.begin());
        i != reaction_rules_.end(); ++i)
    {
        ReactionRuleExpressionMatcher rrexp(*i);

        if (rrexp.match(sp1, sp2))
        {
            do
            {
                const std::vector<Species> products(rrexp.generate());
                retval.push_back(ReactionRule(rrexp.reactants(), products, (*i).k()));
            }
            while (rrexp.next());
        }

        if (rrexp.match(sp2, sp1))
        {
            do
            {
                const std::vector<Species> products(rrexp.generate());
                retval.push_back(ReactionRule(rrexp.reactants(), products, (*i).k()));
            }
            while (rrexp.next());
        }
    }
    return retval;

    // std::vector<ReactionRule> retval;
    // const std::pair<Species::serial_type, Species::serial_type>
    //     key(sp1.serial() < sp2.serial()?
    //         std::make_pair(sp1.serial(), sp2.serial()):
    //         std::make_pair(sp2.serial(), sp1.serial()));

    // second_order_reaction_rules_map_type::const_iterator
    //     i(second_order_reaction_rules_map_.find(key));
    // if (i != second_order_reaction_rules_map_.end())
    // {
    //     retval.reserve((*i).second.size());
    //     for (second_order_reaction_rules_map_type::mapped_type::const_iterator
    //              j((*i).second.begin()); j != (*i).second.end(); ++j)
    //     {
    //         retval.push_back(reaction_rules_[*j]);
    //     }
    // }
    // return retval;
}

Integer NetfreeModel::apply(const Species& pttrn, const Species& sp) const
{
    return sp.count(pttrn);
    // return SpeciesExpressionMatcher(pttrn).count(sp);
}

std::vector<ReactionRule> NetfreeModel::apply(
    const ReactionRule& rr, const ReactionRule::reactant_container_type& reactants) const
{
    return rr.generate(reactants);
    // const std::vector<std::vector<Species> > possibles(rrgenerate(rr, reactants));
    // std::vector<ReactionRule> retval;
    // retval.reserve(possibles.size());
    // for (std::vector<std::vector<Species> >::const_iterator i(possibles.begin());
    //     i != possibles.end(); ++i)
    // {
    //     retval.push_back(ReactionRule(reactants, *i, rr.k()));
    // }
    // return retval;
}

void NetfreeModel::add_species_attribute(const Species& sp)
{
    if (has_species_attribute_exact(sp))
    {
        throw AlreadyExists("species already exists");
    }
    species_attributes_.push_back(sp);
}

void NetfreeModel::remove_species_attribute(const Species& sp)
{
    species_container_type::iterator i(
        std::find(species_attributes_.begin(), species_attributes_.end(), sp));
    if (i == species_attributes_.end())
    {
        std::ostringstream message;
        message << "Speices [" << sp.serial() << "] not found";
        throw NotFound(message.str()); // use boost::format if it's allowed
    }
    species_attributes_.erase(i);
}

bool NetfreeModel::has_species_attribute(const Species& sp) const
{
    return has_species_attribute_exact(sp);
}

bool NetfreeModel::has_species_attribute_exact(const Species& sp) const
{
    species_container_type::const_iterator i(
        std::find(species_attributes_.begin(), species_attributes_.end(), sp));
    return (i != species_attributes_.end());
}

void NetfreeModel::add_reaction_rule(const ReactionRule& rr)
{
    reaction_rule_container_type::const_iterator
        i(std::find(reaction_rules_.begin(), reaction_rules_.end(), rr));
    if (i != reaction_rules_.end())
    {
        throw AlreadyExists("reaction rule already exists");
    }

    const reaction_rule_container_type::size_type idx(reaction_rules_.size());
    reaction_rules_.push_back(rr);

    // if (rr.reactants().size() == 1)
    // {
    //     first_order_reaction_rules_map_[rr.reactants()[0].serial()].push_back(idx);
    // }
    // else if (rr.reactants().size() == 2)
    // {
    //     const Species::serial_type
    //         serial1(rr.reactants()[0].serial()),
    //         serial2(rr.reactants()[1].serial());
    //     const std::pair<Species::serial_type, Species::serial_type>
    //         key(serial1 < serial2?
    //             std::make_pair(serial1, serial2):
    //             std::make_pair(serial2, serial1));
    //     second_order_reaction_rules_map_[key].push_back(idx);
    // }
    // else
    // {
    //     ;
    // }
}

void NetfreeModel::remove_reaction_rule(const ReactionRule& rr)
{
    reaction_rule_container_type::iterator
        i(std::find(reaction_rules_.begin(), reaction_rules_.end(), rr));
    if (i == reaction_rules_.end())
    {
        throw NotFound("reaction rule not found");
    }
    reaction_rules_.erase(i);

    // reaction_rule_container_type::size_type const
    //     idx(i - reaction_rules_.begin()), last_idx(reaction_rules_.size() - 1);
    // if (rr.reactants().size() == 1)
    // {
    //     first_order_reaction_rules_map_type::iterator
    //         j(first_order_reaction_rules_map_.find(rr.reactants()[0].serial()));
    //     if (j == first_order_reaction_rules_map_.end())
    //     {
    //         throw IllegalState("no corresponding map key found");
    //     }

    //     first_order_reaction_rules_map_type::mapped_type::iterator
    //         k(std::remove((*j).second.begin(), (*j).second.end(), idx));
    //     if (k == (*j).second.end())
    //     {
    //         throw IllegalState("no corresponding map value found");
    //     }
    //     else
    //     {
    //         (*j).second.erase(k, (*j).second.end());
    //     }
    // }
    // else if (rr.reactants().size() == 2)
    // {
    //     second_order_reaction_rules_map_type::iterator
    //         j(second_order_reaction_rules_map_.find(std::make_pair(
    //             rr.reactants()[0].serial(), rr.reactants()[1].serial())));
    //     if (j == second_order_reaction_rules_map_.end())
    //     {
    //         throw IllegalState("no corresponding map key found");
    //     }

    //     second_order_reaction_rules_map_type::mapped_type::iterator
    //         k(std::remove((*j).second.begin(), (*j).second.end(), idx));
    //     if (k == (*j).second.end())
    //     {
    //         throw IllegalState("no corresponding map value found");
    //     }
    //     else
    //     {
    //         (*j).second.erase(k, (*j).second.end());
    //     }
    // }

    // if (idx < last_idx)
    // {
    //     reaction_rule_container_type::value_type const
    //         last_value(reaction_rules_[last_idx]);
    //     (*i) = last_value;

    //     if (last_value.reactants().size() == 1)
    //     {
    //         first_order_reaction_rules_map_type::iterator
    //             j(first_order_reaction_rules_map_.find(
    //                 last_value.reactants()[0].serial()));
    //         if (j == first_order_reaction_rules_map_.end())
    //         {
    //             throw IllegalState("no corresponding map key for the last found");
    //         }

    //         first_order_reaction_rules_map_type::mapped_type::iterator
    //             k(std::remove((*j).second.begin(), (*j).second.end(), last_idx));
    //         if (k == (*j).second.end())
    //         {
    //             throw IllegalState("no corresponding map value found");
    //         }
    //         else
    //         {
    //             (*j).second.erase(k, (*j).second.end());
    //         }
    //         (*j).second.push_back(idx);
    //     }
    //     else if (last_value.reactants().size() == 2)
    //     {
    //         second_order_reaction_rules_map_type::iterator
    //             j(second_order_reaction_rules_map_.find(std::make_pair(
    //                 last_value.reactants()[0].serial(),
    //                 last_value.reactants()[1].serial())));
    //         if (j == second_order_reaction_rules_map_.end())
    //         {
    //             throw IllegalState("no corresponding map key for the last found");
    //         }
    //         second_order_reaction_rules_map_type::mapped_type::iterator
    //             k(std::remove((*j).second.begin(), (*j).second.end(), last_idx));
    //         if (k == (*j).second.end())
    //         {
    //             throw IllegalState("no corresponding map value found");
    //         }
    //         else
    //         {
    //             (*j).second.erase(k, (*j).second.end());
    //         }
    //         (*j).second.push_back(idx);
    //     }
    // }

    // reaction_rules_.pop_back();
}

bool NetfreeModel::has_reaction_rule(const ReactionRule& rr) const
{
    reaction_rule_container_type::const_iterator
        i(std::find(reaction_rules_.begin(), reaction_rules_.end(), rr));
    return (i != reaction_rules_.end());
}

} // ecell4