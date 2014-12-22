#ifndef __ECELL4_BD_BD_SIMULATOR_HPP
#define __ECELL4_BD_BD_SIMULATOR_HPP

#include <stdexcept>
#include <boost/shared_ptr.hpp>

#include <ecell4/core/Model.hpp>
#include <ecell4/core/SimulatorBase.hpp>

#include "BDWorld.hpp"
#include "BDPropagator.hpp"


namespace ecell4
{

namespace bd
{

class BDSimulator
    : public SimulatorBase<Model, BDWorld>
{
public:

    typedef SimulatorBase<Model, BDWorld> base_type;

public:

    BDSimulator(boost::shared_ptr<Model> model,
        boost::shared_ptr<BDWorld> world)
        : base_type(model, world), dt_(0)
    {
        initialize();
    }

    BDSimulator(boost::shared_ptr<BDWorld> world)
        : base_type(world), dt_(0)
    {
        initialize();
    }

    // SimulatorTraits

    void initialize()
    {
        last_reactions_.clear();
    }

    Real dt() const
    {
        return dt_;
    }

    void step();
    bool step(const Real& upto);

    // Optional members

    std::vector<ReactionRule> last_reactions() const
    {
        return last_reactions_;
    }

    void set_dt(const Real& dt)
    {
        if (dt <= 0)
        {
            throw std::invalid_argument("The step size must be positive.");
        }
        dt_ = dt;
    }

    inline boost::shared_ptr<RandomNumberGenerator> rng()
    {
        return (*world_).rng();
    }

protected:

    /**
     * the protected internal state of BDSimulator.
     * they are needed to be saved/loaded with Visitor pattern.
     */
    Real dt_;
    std::vector<ReactionRule> last_reactions_;
};

} // bd

} // ecell4

#endif /* __ECELL4_BD_BD_SIMULATOR_HPP */
